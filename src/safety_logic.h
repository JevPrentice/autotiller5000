// Copyright 2025 Wentzel
#ifndef SAFETY_LOGIC_H_
#define SAFETY_LOGIC_H_

#include <stdint.h>

struct SaveCommandResult {
  bool shouldSave;
  bool playHalfwayBeep;
  bool playSuccessBeep;
  bool playNoChangesBeep;
  bool playErrorBeep;
};

struct I2CTimeoutResult {
  bool shouldEnterSafeMode;
  bool sensorRecovered;
  int newFailureCount;
};

inline SaveCommandResult evaluateSaveCommand(
    uint32_t currentTime,
    uint32_t pressStartTime,
    bool buttonCurrentlyPressed,
    bool buttonWasPressedBefore,
    bool halfwayBeepAlreadyPlayed,
    bool hasChanges,
    uint32_t halfwayBeepTime,
    uint32_t saveHoldTime) {

  SaveCommandResult result = {
    .shouldSave = false,
    .playHalfwayBeep = false,
    .playSuccessBeep = false,
    .playNoChangesBeep = false,
    .playErrorBeep = false
  };

  if (!buttonCurrentlyPressed || !buttonWasPressedBefore) {
    return result;
  }

  uint32_t holdTime = currentTime - pressStartTime;

  if (holdTime >= halfwayBeepTime && !halfwayBeepAlreadyPlayed) {
    result.playHalfwayBeep = true;
  }

  if (holdTime >= saveHoldTime) {
    if (hasChanges) {
      result.shouldSave = true;
      result.playSuccessBeep = true;
    } else {
      result.playNoChangesBeep = true;
    }
  }

  return result;
}

inline I2CTimeoutResult evaluateI2CTimeout(
    bool sensorReadSuccess,
    int currentFailureCount,
    bool currentlyInSafeMode,
    int maxFailures) {

  I2CTimeoutResult result = {
    .shouldEnterSafeMode = false,
    .sensorRecovered = false,
    .newFailureCount = currentFailureCount
  };

  if (sensorReadSuccess) {
    result.newFailureCount = 0;
    if (currentlyInSafeMode) {
      result.sensorRecovered = true;
    }
  } else {
    result.newFailureCount = currentFailureCount + 1;
    if (result.newFailureCount >= maxFailures && !currentlyInSafeMode) {
      result.shouldEnterSafeMode = true;
    }
  }

  return result;
}

inline bool detectSettingsChanges(
    int currentSensitivity,
    int savedSensitivity,
    int8_t currentReverse,
    int8_t savedReverse) {
  return (currentSensitivity != savedSensitivity) ||
         (currentReverse != savedReverse);
}

inline uint32_t calculateButtonHoldDuration(
    uint32_t pressStartTime,
    uint32_t releaseTime) {
  return releaseTime - pressStartTime;
}

inline bool verifyEEPROMWrite(
    int expectedSensitivity,
    int readSensitivity,
    int8_t expectedReverse,
    int8_t readReverse,
    uint8_t expectedMagic,
    uint8_t readMagic) {
  return (expectedSensitivity == readSensitivity) &&
         (expectedReverse == readReverse) &&
         (expectedMagic == readMagic);
}

#endif  // SAFETY_LOGIC_H_
