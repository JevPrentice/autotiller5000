// Copyright 2025 Wentzel
#ifndef BUTTON_DEBOUNCE_H_
#define BUTTON_DEBOUNCE_H_

#include "config.h"

bool debouncedDigitalRead(int pin, ButtonState& state, uint16_t debounceMs) {
  bool rawReading = digitalRead(pin) == LOW;
  uint32_t currentTime = millis();

  if (rawReading != state.lastState) {
    state.lastChangeTime = currentTime;
  }

  if ((currentTime - state.lastChangeTime) >= debounceMs) {
    if (rawReading != state.currentState) {
      state.currentState = rawReading;
      state.debounced = true;
    }
  } else {
    state.debounced = false;
  }

  state.lastState = rawReading;
  return state.currentState;
}

#endif  // BUTTON_DEBOUNCE_H_