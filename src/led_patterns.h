// Copyright 2025 Wentzel
#ifndef LED_PATTERNS_H
#define LED_PATTERNS_H

#include <stdint.h>

struct LedState {
  bool led1;
  bool led2;
  bool led3;
};

LedState calculateButtonHoldLeds(uint32_t elapsed_ms,
                                  uint16_t sensitivity_threshold_ms,
                                  uint16_t reverse_threshold_ms) {
  LedState state = {
    .led1 = false,
    .led2 = false,
    .led3 = false
  };

  if (elapsed_ms < sensitivity_threshold_ms) {
    state.led1 = true;
  } else if (elapsed_ms < reverse_threshold_ms) {
    state.led1 = true;
    state.led2 = true;
  } else {
    state.led1 = true;
    state.led2 = true;
    state.led3 = true;
  }

  return state;
}

#endif