// Copyright 2025 Wentzel
#ifndef STATE_WORKFLOWS_H_
#define STATE_WORKFLOWS_H_

#include <Servo.h>
#include <avr/wdt.h>
#include "config.h"
#include "navigation.h"
#include "sensitivity_adjustment.h"
#include "state_machine.h"

void LEDOFF(const HardwarePins& pins) {
  digitalWrite(pins.led1, LOW);
  digitalWrite(pins.led2, LOW);
  digitalWrite(pins.led3, LOW);
}

void altercourse(Servo& servo,
                 const NavigationState& navState,
                 const TimingConfig& timing,
                 float& servoposition) {
  float adjustment = calculateServoAdjustment(
      navState.heading_adjustment, navState.sensitivity, navState.reverse);
  servoposition = servoposition + adjustment;
  delay(timing.servo_adjust_delay_ms);
  servoposition = constrainServoPosition(servoposition);
  servo.write(servoposition);
}

void setsensitive(const HardwarePins& pins,
                  const SensitivityConfig& sensConfig,
                  const TimingConfig& timing,
                  int& sensitivity) {
  LEDOFF(pins);
  int pushed = HIGH;
  while (pushed == HIGH) {
    wdt_reset();
    Serial.println(F("Set sensitivity level"));
    SensitivityAdjustmentResult result = adjustSensitivity(
        sensitivity,
        digitalRead(pins.port_button) == LOW,
        digitalRead(pins.starboard_button) == LOW,
        sensConfig.step);

    if (result.buzzerFrequency > 0) {
      tone(pins.buzzer, result.buzzerFrequency);
    }
    sensitivity = result.newSensitivity;

    Serial.print(F("Sensitivity: "));
    Serial.println(sensitivity);
    delay(timing.settings_debounce_ms);
    pushed = digitalRead(pins.main_button);
  }
}

void checktime(const HardwarePins& pins,
               const TimingConfig& timing,
               const SensitivityConfig& sensConfig,
               int& sensitivity,
               int8_t& reverse,
               int& pushed,
               int& release) {
  LEDOFF(pins);
  delay(timing.startup_delay_ms);
  pushed = LOW;
  int32_t pastpress = millis();
  while (pushed == LOW) {
    wdt_reset();
    Serial.println(pastpress);
    if (millis() - pastpress < timing.sensitivity_threshold_ms) {
      digitalWrite(pins.led1, HIGH);
      delay(timing.led_flash_delay_ms);
    }
    if ((millis() - pastpress > timing.sensitivity_threshold_ms) &&
        (millis() - pastpress < timing.reverse_threshold_ms)) {
      digitalWrite(pins.led1, HIGH);
      digitalWrite(pins.led2, HIGH);
      delay(timing.led_flash_delay_ms);
    }
    if (millis() - pastpress > timing.reverse_threshold_ms) {
      digitalWrite(pins.led1, HIGH);
      digitalWrite(pins.led2, HIGH);
      digitalWrite(pins.led3, HIGH);
      delay(timing.led_flash_delay_ms);
    }
    pushed = digitalRead(pins.main_button);
    LEDOFF(pins);
  }
  int32_t newpress = millis();
  uint32_t elapsedtime = newpress - pastpress;

  ButtonHoldAction action = determineButtonHoldAction(
      elapsedtime, timing.sensitivity_threshold_ms,
      timing.reverse_threshold_ms);

  if (action == ACTION_EXIT_COURSE_MODE) {
    pushed = HIGH;
    LEDOFF(pins);
  } else if (action == ACTION_ADJUST_SENSITIVITY) {
    setsensitive(pins, sensConfig, timing, sensitivity);
    release = HIGH;
    delay(timing.settings_debounce_ms);
    LEDOFF(pins);
  } else if (action == ACTION_TOGGLE_REVERSE) {
    reverse = reverse * -1;
    Serial.print(F("Reverse toggled to: "));
    Serial.println(reverse);
    delay(timing.settings_debounce_ms);
    release = HIGH;
    LEDOFF(pins);
  }

  LEDOFF(pins);
}

#endif  // STATE_WORKFLOWS_H_
