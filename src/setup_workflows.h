// Copyright 2025 Wentzel
#ifndef SETUP_WORKFLOWS_H_
#define SETUP_WORKFLOWS_H_

#include <Adafruit_BNO055.h>
#include <EEPROM.h>
#include <Servo.h>
#include <avr/wdt.h>
#include "config.h"
#include "calibration.h"
#include "diagnostics.h"
#include "sensor_safety.h"
#include "settings_persistence.h"
#include "state_workflows.h"

void initializeSerial(const SensorConfig& sensorConfig,
                      const TimingConfig& timing) {
  Serial.begin(sensorConfig.serial_baud);
  delay(timing.startup_delay_ms);
  Serial.println(F("Orientation Sensor Test"));
  Serial.println(F(""));
}

void handleWatchdogReset(uint8_t mcusr_value,
                         const HardwarePins& pins,
                         const AudioConfig& audio,
                         WatchdogHealthMonitor& wdtMonitor) {
  if (mcusr_value & (1 << WDRF)) {
    if (wdtMonitor.monitoring_enabled) {
      wdtMonitor.reset_count++;
      wdtMonitor.last_reset_time = millis();
      wdtMonitor.uptime_at_last_reset = millis();
    }

    Serial.print(F("WARNING: Watchdog reset detected! (Reset #"));
    Serial.print(wdtMonitor.reset_count);
    Serial.println(F(")"));

    // URGENT MARINE SAFETY ALARM: Triple beep pattern, repeated
    for (int set = 0; set < audio.watchdog_reset.set_count; set++) {
      wdt_reset();  // Safety: reset watchdog before each set

      for (int beep = 0; beep < audio.watchdog_reset.beep_count; beep++) {
        tone(pins.buzzer, audio.watchdog_reset.frequency_hz);
        delay(audio.watchdog_reset.beep_duration_ms);
        noTone(pins.buzzer);

        // Pause between beeps (except after last beep in set)
        if (beep < audio.watchdog_reset.beep_count - 1) {
          delay(audio.watchdog_reset.beep_pause_ms);
        }
      }

      // Pause between sets (except after last set)
      if (set < audio.watchdog_reset.set_count - 1) {
        delay(audio.watchdog_reset.set_pause_ms);
      }
    }
  }
}

void initializeHardware(const HardwarePins& pins, Servo& servo) {
  pinMode(pins.main_button, INPUT_PULLUP);
  pinMode(pins.port_button, INPUT_PULLUP);
  pinMode(pins.starboard_button, INPUT_PULLUP);
  pinMode(pins.buzzer, OUTPUT);
  servo.attach(pins.servo);
  pinMode(pins.led1, OUTPUT);
  pinMode(pins.led2, OUTPUT);
  pinMode(pins.led3, OUTPUT);
  LEDOFF(pins);
}

void handleCalibrationFromEEPROM(Adafruit_BNO055& bno,
                                  SensorSafetyState& sensorState,
                                  const HardwarePins& pins,
                                  const SensorConfig& sensorConfig,
                                  const TimingConfig& timing,
                                  const AudioConfig& audio,
                                  const SensorSafetyConfig& safetyConfig) {
  adafruit_bno055_offsets_t ofs;
  if (loadCalibFromEEPROM(ofs)) {
    Serial.println(F("Loaded BNO055 calibration from EEPROM."));
    applyCalibToBNO(bno, ofs, timing.led_flash_delay_ms);
    uint32_t t0 = millis();
    const uint32_t MAX_WAIT_MS = 3000;
    while (!bno.isFullyCalibrated() && (millis() - t0 < MAX_WAIT_MS)) {
      sensors_event_t tempEvent;
      if (!getBNO055EventWithTimeout(bno, &tempEvent, sensorState,
                                      pins, timing, audio, safetyConfig)) {
        Serial.println(F("WARNING: Sensor timeout during calibration refinement"));
        break;
      }
      delay(sensorConfig.samplerate_delay_ms);
    }
    if (bno.isFullyCalibrated()) {
      Serial.println(F("Sensor reports fully calibrated."));
    } else {
      Serial.println(
          F("Calibration applied; sensor will refine with gentle motion."));
    }
  } else {
    Serial.println(F("Please Calibrate Sensor: "));
  }
}

void initializeSensor(Adafruit_BNO055& bno,
                      SensorSafetyState& sensorState,
                      const HardwarePins& pins,
                      const SensorConfig& sensorConfig,
                      const TimingConfig& timing,
                      const AudioConfig& audio,
                      const SensorSafetyConfig& safetyConfig,
                      const WatchdogHealthMonitor& wdtMonitor) {
  if (!bno.begin()) {
    Serial.print(F(
        "Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!"));
    while (1) {}
  }

  int eeAddress = 0;
  int32_t bnoID;
  EEPROM.get(eeAddress, bnoID);

  adafruit_bno055_offsets_t calibrationData;
  sensor_t sensor;

  bno.getSensor(&sensor);
  if (bnoID != sensor.sensor_id) {
    Serial.println(F(
        "\nNo Calibration Data for this sensor exists in EEPROM"));
  } else {
    Serial.println(F("\nFound Calibration for this sensor in EEPROM."));
    eeAddress += sizeof(int32_t);
    EEPROM.get(eeAddress, calibrationData);

    displaySensorOffsets(calibrationData);

    Serial.println(F("\n\nRestoring Calibration data to the BNO055..."));
    bno.setSensorOffsets(calibrationData);

    Serial.println(F("\n\nCalibration data loaded into BNO055"));
  }

  displaySensorDetails();
  displaySensorStatus();
  displayWatchdogHealth(wdtMonitor);

  bno.setExtCrystalUse(true);

  sensors_event_t event;
  if (getBNO055EventWithTimeout(bno, &event, sensorState, pins, timing,
                                 audio, safetyConfig)) {
    handleCalibrationFromEEPROM(bno, sensorState, pins, sensorConfig, timing,
                                 audio, safetyConfig);
  } else {
    Serial.println(F("WARNING: Sensor timeout during initial calibration"));
  }
}

#endif  // SETUP_WORKFLOWS_H_
