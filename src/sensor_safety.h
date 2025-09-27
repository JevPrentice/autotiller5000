// Copyright 2025 Wentzel
#ifndef SENSOR_SAFETY_H_
#define SENSOR_SAFETY_H_

#include <Adafruit_BNO055.h>
#include <Servo.h>
#include <avr/wdt.h>
#include "config.h"

struct SensorSafetyState {
  int consecutiveFailures;
  bool inSafeMode;
  uint32_t nextRetryTime;
  uint32_t backoffDelay;
};

bool getBNO055EventWithTimeout(Adafruit_BNO055& bno,
                                sensors_event_t* event,
                                SensorSafetyState& state,
                                const HardwarePins& pins,
                                const TimingConfig& timing,
                                const AudioConfig& audio,
                                const SensorSafetyConfig& safetyConfig) {
  wdt_reset();
  uint32_t currentTime = millis();

  // Check if we're in backoff period
  if (currentTime < state.nextRetryTime) {
    return false;
  }

  uint32_t startTime = currentTime;
  while (millis() - startTime < timing.i2c_timeout_ms) {
    if (bno.getEvent(event)) {
      // Success - reset backoff state
      if (state.consecutiveFailures > 0) {
        state.consecutiveFailures = 0;
        state.backoffDelay = 10;  // Reset to initial delay
        state.nextRetryTime = 0;
        if (state.inSafeMode) {
          Serial.println(F("Sensor recovered from safe mode"));
          tone(pins.buzzer, audio.recovery.frequency_hz,
               audio.recovery.duration_ms);
          delay(audio.recovery.duration_ms + 50);
          state.inSafeMode = false;
        }
      }
      return true;
    }
    delay(state.backoffDelay);
    wdt_reset();
  }

  // Failure - implement exponential backoff
  state.consecutiveFailures++;
  state.nextRetryTime = millis() + state.backoffDelay;
  state.backoffDelay = state.backoffDelay * 2;
  if (state.backoffDelay > 1000) {
    state.backoffDelay = 1000;  // Cap at 1 second
  }

  Serial.print(F("Sensor timeout (failure "));
  Serial.print(state.consecutiveFailures);
  Serial.print(F("/"));
  Serial.print(safetyConfig.max_failures);
  Serial.print(F(", backoff: "));
  Serial.print(state.backoffDelay);
  Serial.println(F("ms)"));

  return false;
}

void enterSensorSafeMode(SensorSafetyState& state,
                         Servo& servo,
                         bool& pushed,
                         const HardwarePins& pins,
                         const AudioConfig& audio,
                         const SensorSafetyConfig& safetyConfig) {
  if (state.inSafeMode) return;

  Serial.println(F("CRITICAL: Entering sensor safe mode!"));
  state.inSafeMode = true;

  servo.write(safetyConfig.servo_center_position);
  pushed = HIGH;

  for (int i = 0; i < 3; i++) {
    wdt_reset();
    tone(pins.buzzer, audio.error.frequency_hz);
    delay(audio.error.duration_ms / 3);
    wdt_reset();
    noTone(pins.buzzer);
    delay(100);
  }
}

#endif  // SENSOR_SAFETY_H_
