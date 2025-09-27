// Copyright 2025 Wentzel
#ifndef WATCHDOG_SAFETY_H_
#define WATCHDOG_SAFETY_H_

#include <stdint.h>

struct DelayChunkResult {
  int chunkCount;
  uint32_t chunkSizeMs;
  uint32_t remainderMs;
  bool exceedsTimeout;
};

struct WatchdogRiskAssessment {
  bool isSafe;
  uint32_t maxSafeDelay;
  float riskFactor;
};

struct WatchdogResetTiming {
  bool shouldReset;
  uint32_t timeSinceLastReset;
  uint32_t timeUntilRequired;
};

inline DelayChunkResult calculateSafeDelayChunks(
    uint32_t totalDelayMs,
    uint32_t maxChunkSizeMs,
    uint32_t watchdogTimeoutMs) {

  DelayChunkResult result = {
    .chunkCount = 0,
    .chunkSizeMs = 0,
    .remainderMs = 0,
    .exceedsTimeout = false
  };

  if (totalDelayMs == 0) {
    return result;
  }

  if (maxChunkSizeMs >= watchdogTimeoutMs) {
    result.exceedsTimeout = true;
    return result;
  }

  if (totalDelayMs <= maxChunkSizeMs) {
    result.chunkCount = 1;
    result.chunkSizeMs = totalDelayMs;
    return result;
  }

  result.chunkCount = totalDelayMs / maxChunkSizeMs;
  result.chunkSizeMs = maxChunkSizeMs;
  result.remainderMs = totalDelayMs % maxChunkSizeMs;

  if (result.remainderMs > 0) {
    result.chunkCount++;
  }

  return result;
}

inline WatchdogRiskAssessment evaluateWatchdogRisk(
    uint32_t operationDurationMs,
    uint32_t watchdogTimeoutMs,
    float safetyMargin = 0.8f) {

  WatchdogRiskAssessment result = {
    .isSafe = true,
    .maxSafeDelay = static_cast<uint32_t>(watchdogTimeoutMs * safetyMargin),
    .riskFactor = 0.0f
  };

  if (operationDurationMs == 0) {
    return result;
  }

  result.riskFactor = static_cast<float>(operationDurationMs) / watchdogTimeoutMs;
  result.isSafe = (operationDurationMs <= result.maxSafeDelay);

  return result;
}

inline WatchdogResetTiming shouldResetWatchdog(
    uint32_t currentTimeMs,
    uint32_t lastResetTimeMs,
    uint32_t resetIntervalMs,
    uint32_t watchdogTimeoutMs) {

  WatchdogResetTiming result = {
    .shouldReset = false,
    .timeSinceLastReset = 0,
    .timeUntilRequired = 0
  };

  result.timeSinceLastReset = currentTimeMs - lastResetTimeMs;

  if (result.timeSinceLastReset >= resetIntervalMs) {
    result.shouldReset = true;
  } else {
    result.timeUntilRequired = resetIntervalMs - result.timeSinceLastReset;
  }

  return result;
}

inline uint32_t calculateSafeIntervalMs(
    uint32_t watchdogTimeoutMs,
    float safetyMarginPercent = 50.0f) {
  return static_cast<uint32_t>(watchdogTimeoutMs * (safetyMarginPercent / 100.0f));
}

inline bool isOperationSafeForWatchdog(
    uint32_t operationDurationMs,
    uint32_t timeUntilWatchdogReset,
    uint32_t safetyBufferMs = 100) {
  return (operationDurationMs + safetyBufferMs) <= timeUntilWatchdogReset;
}

inline uint32_t getRecommendedChunkSize(
    uint32_t watchdogTimeoutMs,
    float utilizationPercent = 10.0f) {
  return static_cast<uint32_t>(watchdogTimeoutMs * (utilizationPercent / 100.0f));
}

#endif  // WATCHDOG_SAFETY_H_