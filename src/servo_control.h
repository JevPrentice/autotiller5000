// Copyright 2025 Wentzel
#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include "navigation.h"

struct ServoUpdateResult {
  float newPosition;
  float adjustmentApplied;
};

ServoUpdateResult updateServoPosition(float currentPosition,
                                       float headingAdjustment,
                                       int sensitivity,
                                       int8_t reverse) {
  ServoUpdateResult result;
  result.adjustmentApplied = calculateServoAdjustment(headingAdjustment,
                                                       sensitivity, reverse);
  result.newPosition = currentPosition + result.adjustmentApplied;
  result.newPosition = constrainServoPosition(result.newPosition);
  return result;
}

#endif