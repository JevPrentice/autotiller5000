// Copyright 2025 Wentzel
#ifndef BUTTON_LOGIC_H_
#define BUTTON_LOGIC_H_

#include <stdint.h>

struct ButtonPressResult {
  int courseAdjustment;
  bool isTack;
};

inline ButtonPressResult evaluateButtonPress(
    uint32_t pressDuration,
    int shortPressThreshold,
    int courseAdjustDegrees,
    int tackAngleDegrees,
    bool isPortButton) {

  ButtonPressResult result;

  if (pressDuration < shortPressThreshold) {
    result.isTack = false;
    result.courseAdjustment = isPortButton ? -courseAdjustDegrees : courseAdjustDegrees;
  } else {
    result.isTack = true;
    result.courseAdjustment = isPortButton ? -tackAngleDegrees : tackAngleDegrees;
  }

  return result;
}

#endif  // BUTTON_LOGIC_H_