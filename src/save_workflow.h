// Copyright 2025 Wentzel
#ifndef SAVE_WORKFLOW_H_
#define SAVE_WORKFLOW_H_

#include <avr/wdt.h>
#include "settings_persistence.h"
#include "config.h"

void checkSaveCommand(const SaveCommandContext& ctx,
                      const HardwarePins& pins,
                      const TimingConfig& timing,
                      const AudioConfig& audio) {
  static uint32_t buttonPressStart = 0;
  static bool buttonWasPressed = false;
  static bool halfwayBeepPlayed = false;

  bool buttonPressed = (digitalRead(pins.main_button) == LOW);

  if (buttonPressed && !buttonWasPressed && ctx.pushed == HIGH) {
    buttonPressStart = millis();
    buttonWasPressed = true;
    halfwayBeepPlayed = false;
  }

  if (buttonPressed && buttonWasPressed && ctx.pushed == HIGH) {
    uint32_t holdTime = millis() - buttonPressStart;

    if (holdTime >= ctx.halfway_beep_time_ms && !halfwayBeepPlayed) {
      tone(pins.buzzer, audio.save_no_changes.frequency_hz, 100);
      delay(100);
      halfwayBeepPlayed = true;
      Serial.println(F("Hold for 5 more seconds to save settings..."));
    }

    if (holdTime >= ctx.save_hold_time_ms) {
      bool hasChanges = settingsHaveChanged(ctx.settings, ctx.lastSaved);

      if (hasChanges) {
        if (saveUserSettings(ctx.settings, ctx.lastSaved)) {
          for (int i = 0; i < 3; i++) {
            wdt_reset();
            tone(pins.buzzer, audio.save_success.frequency_hz,
                 audio.save_success.duration_ms);
            delay(audio.save_success.duration_ms + 50);
          }
        } else {
          tone(pins.buzzer, audio.error.frequency_hz,
               audio.error.duration_ms);
          delay(audio.error.duration_ms);
        }
      } else {
        tone(pins.buzzer, audio.save_no_changes.frequency_hz,
             audio.save_no_changes.duration_ms);
        delay(audio.save_no_changes.duration_ms);
        Serial.println(F("No changes to save"));
      }

      buttonWasPressed = false;
      while (digitalRead(pins.main_button) == LOW) {
        wdt_reset();
        delay(10);
      }
      delay(timing.settings_debounce_ms);
    }
  }

  if (!buttonPressed && buttonWasPressed) {
    buttonWasPressed = false;
  }
}

#endif  // SAVE_WORKFLOW_H_