// Copyright 2025 Wentzel
#ifndef TACK_LOGIC_H_
#define TACK_LOGIC_H_

#include <stdint.h>

struct TackWarningResult {
  bool shouldCancel;
  int iterationsCompleted;
};

inline TackWarningResult evaluateTackWarning(
    int currentIteration,
    int totalIterations,
    bool cancelButtonPressed) {

  TackWarningResult result;
  result.iterationsCompleted = currentIteration;

  if (cancelButtonPressed) {
    result.shouldCancel = true;
    result.iterationsCompleted = totalIterations;
  } else {
    result.shouldCancel = false;
  }

  return result;
}

#endif  // TACK_LOGIC_H_