// Copyright 2025 Wentzel
#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdint.h>

enum ButtonHoldAction {
  ACTION_EXIT_COURSE_MODE,
  ACTION_ADJUST_SENSITIVITY,
  ACTION_TOGGLE_REVERSE
};

ButtonHoldAction determineButtonHoldAction(uint32_t elapsed_ms,
                                             uint16_t sensitivity_threshold_ms,
                                             uint16_t reverse_threshold_ms) {
  if (elapsed_ms <= sensitivity_threshold_ms) {
    return ACTION_EXIT_COURSE_MODE;
  } else if ((elapsed_ms > sensitivity_threshold_ms) &&
             (elapsed_ms < reverse_threshold_ms)) {
    return ACTION_ADJUST_SENSITIVITY;
  } else {
    return ACTION_TOGGLE_REVERSE;
  }
}

#endif