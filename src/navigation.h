// Copyright 2025 Wentzel
#ifndef NAVIGATION_H_
#define NAVIGATION_H_

#include <math.h>

#define DEG_TO_RAD 0.01746032

float calculateHeadingAdjustment(float course, float heading) {
  float adj = course - heading;
  if (adj < -180) {
    adj = adj + 360;
  }
  if (adj > 180) {
    adj = adj - 360;
  }
  if (adj == -180) {
    adj = 180;
  }
  return adj;
}

float normalizeCourse(float course) {
  float result = fmod(course, 360.0);
  if (result < 0) {
    result += 360.0;
  }
  return result;
}

float calculateServoAdjustment(float headingAdj, int sensitivity,
                                int8_t reverse) {
  return (sensitivity / 10.0) * sin(DEG_TO_RAD * headingAdj) * reverse;
}

float constrainServoPosition(float position) {
  if (position < 0) {
    return 0;
  }
  if (position > 180) {
    return 180;
  }
  return position;
}

int clampSensitivity(int value) {
  if (value < 30) {
    return 30;
  }
  if (value > 1000) {
    return 1000;
  }
  return value;
}

#endif  // NAVIGATION_H_
