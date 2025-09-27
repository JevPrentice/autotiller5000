// Copyright 2025 Wentzel
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <EEPROM.h>
#include <Servo.h>
#include <avr/wdt.h>

#include "src/constants.h"
#include "src/config.h"
#include "src/button_debounce.h"
#include "src/settings_persistence.h"
#include "src/sensor_safety.h"
#include "src/setup_workflows.h"
#include "src/loop_workflows.h"
#include "src/save_workflow.h"
#include "src/diagnostics.h"

NavigationState navState = {
  .course = 0.0f,
  .heading = 0.0f,
  .heading_adjustment = 0.0f,
  .servo_position = SERVO_CENTER_POSITION,
  .sensitivity = DEFAULT_SENSITIVITY,
  .reverse = DEFAULT_REVERSE
};

UserSettings settings = {
  .sensitivity = DEFAULT_SENSITIVITY,
  .reverse = DEFAULT_REVERSE
};
UserSettings lastSaved = {
  .sensitivity = DEFAULT_SENSITIVITY,
  .reverse = DEFAULT_REVERSE
};

int release;
int pushed;

ButtonState mainButtonState = {
  .currentState = false,
  .lastState = false,
  .lastChangeTime = 0,
  .debounced = false
};
ButtonState portButtonState = {
  .currentState = false,
  .lastState = false,
  .lastChangeTime = 0,
  .debounced = false
};
ButtonState starboardButtonState = {
  .currentState = false,
  .lastState = false,
  .lastChangeTime = 0,
  .debounced = false
};

SensorSafetyState sensorState = {
  .consecutiveFailures = 0,
  .inSafeMode = false,
  .nextRetryTime = 0,
  .backoffDelay = 10
};
bool calibSaved = false;

WatchdogHealthMonitor wdtMonitor = {
  .reset_count = 0,
  .last_reset_time = 0,
  .uptime_at_last_reset = 0,
  .monitoring_enabled = true
};

Servo myservo;
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

void setup(void) {
  initializeSerial(sensorConfig, timing);

  uint8_t resetReason = MCUSR;
  MCUSR = 0;

  handleWatchdogReset(resetReason, pins, audio, wdtMonitor);

  initializeHardware(pins, myservo);

  loadUserSettings(settings, lastSaved, sensConfig.default_value,
                   sensConfig.default_reverse);

  navState.sensitivity = settings.sensitivity;
  navState.reverse = settings.reverse;

  wdt_enable(WDTO_2S);
  Serial.println(F("Watchdog enabled (2s timeout)"));

  initializeSensor(bno, sensorState, pins, sensorConfig, timing, audio,
                   safetyConfig, wdtMonitor);
}

void loop() {
  wdt_reset();

  sensors_event_t event;
  const LoopContext loopCtx = {
    .pins = pins,
    .timing = timing,
    .audio = audio,
    .safetyConfig = safetyConfig
  };
  SensorReadContext sensorCtx = {
    .event = &event,
    .sensorState = sensorState,
    .servo = myservo,
    .pushed = pushed
  };

  if (!handleSensorRead(bno, loopCtx, sensorCtx)) {
    return;
  }

  handleAutoCalibrationSave(bno, calibSaved);

  digitalWrite(pins.led1, HIGH);

  const SaveCommandContext saveCtx = {
    .pushed = pushed,
    .settings = settings,
    .lastSaved = lastSaved,
    .save_hold_time_ms = SAVE_BUTTON_HOLD_TIME_MS,
    .halfway_beep_time_ms = SAVE_HALFWAY_BEEP_TIME_MS
  };
  checkSaveCommand(saveCtx, pins, timing, audio);

  bool mainPressed = debouncedDigitalRead(pins.main_button, mainButtonState,
                                          timing.button_debounce_ms);
  pushed = mainPressed ? LOW : HIGH;
  if (mainPressed) {
    settings.sensitivity = navState.sensitivity;
    settings.reverse = navState.reverse;
    runCourseHoldMode(event, bno, loopCtx, courseConfig, sensorCtx, navState,
                      SENSITIVITY_STEP);
    settings.sensitivity = navState.sensitivity;
    settings.reverse = navState.reverse;
  }

  displayStandbyInfo(event);
  myservo.write(safetyConfig.servo_center_position);
  displayCalStatus();
  Serial.println(F(""));
  delay(sensorConfig.samplerate_delay_ms);
}
