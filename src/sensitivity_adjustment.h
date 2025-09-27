// Copyright 2025 Wentzel
#ifndef SENSITIVITY_ADJUSTMENT_H
#define SENSITIVITY_ADJUSTMENT_H

#include "navigation.h"

struct SensitivityAdjustmentResult {
  int newSensitivity;
  int buzzerFrequency;
};

SensitivityAdjustmentResult adjustSensitivity(int currentSensitivity,
                                                bool portPressed,
                                                bool starboardPressed,
                                                int step) {
  SensitivityAdjustmentResult result;
  result.newSensitivity = currentSensitivity;

  if (portPressed) {
    result.newSensitivity = clampSensitivity(currentSensitivity - step);
    result.buzzerFrequency = result.newSensitivity;
  } else if (starboardPressed) {
    result.newSensitivity = clampSensitivity(currentSensitivity + step);
    result.buzzerFrequency = result.newSensitivity;
  } else {
    result.buzzerFrequency = 0;
  }

  return result;
}

#endif