// Copyright 2025 Wentzel
#ifndef LOOP_WORKFLOWS_H_
#define LOOP_WORKFLOWS_H_

#include <Adafruit_BNO055.h>
#include <Servo.h>
#include <avr/wdt.h>
#include "calibration.h"
#include "navigation.h"
#include "sensor_safety.h"
#include "state_workflows.h"
#include "button_workflows.h"
#include "config.h"

bool handleSensorRead(Adafruit_BNO055& bno,
                      const LoopContext& ctx,
                      SensorReadContext& sensorCtx) {
  if (!getBNO055EventWithTimeout(bno, sensorCtx.event, sensorCtx.sensorState, ctx.pins, ctx.timing,
                                  ctx.audio, ctx.safetyConfig)) {
    Serial.println(F("ERROR: Sensor timeout in main loop"));
    if (sensorCtx.sensorState.consecutiveFailures >= ctx.safetyConfig.max_failures) {
      bool pushTemp = sensorCtx.pushed;
      enterSensorSafeMode(sensorCtx.sensorState, sensorCtx.servo, pushTemp, ctx.pins, ctx.audio,
                          ctx.safetyConfig);
      sensorCtx.pushed = pushTemp;
    }
    return false;
  }
  return true;
}

void handleAutoCalibrationSave(Adafruit_BNO055& bno, bool& calibSaved) {
  if (bno.isFullyCalibrated() && !calibSaved) {
    if (saveCalibToEEPROM(bno)) {
      Serial.println(F("Calibration saved to EEPROM."));
      calibSaved = true;
    }
  }
}

void runCourseHoldMode(sensors_event_t& initial_event,
                       Adafruit_BNO055& bno,
                       const LoopContext& ctx,
                       const CourseAdjustmentConfig& courseConfig,
                       SensorReadContext& sensorCtx,
                       NavigationState& navState,
                       int sensitivity_step) {
  LEDOFF(ctx.pins);
  digitalWrite(ctx.pins.led2, HIGH);
  navState.servo_position = ctx.safetyConfig.servo_center_position;
  delay(ctx.timing.wait_time_ms);
  navState.course = initial_event.orientation.x;

  while (sensorCtx.pushed == LOW) {
    wdt_reset();
    noTone(ctx.pins.buzzer);
    sensorCtx.pushed = LOW;
    sensors_event_t event;
    if (!getBNO055EventWithTimeout(bno, &event, sensorCtx.sensorState, ctx.pins, ctx.timing,
                                    ctx.audio, ctx.safetyConfig)) {
      Serial.println(F("ERROR: Sensor timeout in course hold loop"));
      break;
    }
    navState.heading = event.orientation.x;
    navState.heading_adjustment = calculateHeadingAdjustment(navState.course,
                                                              navState.heading);
    altercourse(sensorCtx.servo, navState, ctx.timing, navState.servo_position);
    const ButtonContext btnCtx = {
      .pins = ctx.pins,
      .audio = ctx.audio,
      .courseConfig = courseConfig,
      .timing = ctx.timing
    };
    checkbuttonpressed(btnCtx, navState.course);
    Serial.print(navState.heading, 0);
    Serial.print(F("       "));
    Serial.print(navState.course, 0);
    Serial.print(F("        "));
    Serial.print(navState.heading_adjustment, 0);
    Serial.print(F("         "));
    Serial.print(navState.servo_position);
    Serial.print(F("         "));
    Serial.println(calculateServoAdjustment(navState.heading_adjustment,
                                            navState.sensitivity,
                                            navState.reverse));

    int release = digitalRead(ctx.pins.main_button);
    if (release == LOW) {
      SensitivityConfig sensConfig = {
        .step = sensitivity_step,
        .default_value = 0,
        .default_reverse = 0
      };
      checktime(ctx.pins, ctx.timing, sensConfig, navState.sensitivity,
                navState.reverse, sensorCtx.pushed, release);
    }
  }
}

void displayStandbyInfo(const sensors_event_t& event) {
  Serial.print(F("X: "));
  Serial.print(event.orientation.x, 0);
  Serial.print(F("\tY: "));
  Serial.print(event.orientation.y, 0);
  Serial.print(F("\tZ: "));
  Serial.print(event.orientation.z, 0);
}

#endif  // LOOP_WORKFLOWS_H_
