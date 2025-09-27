#include <cassert>
#include <cstdio>
#include <cmath>
#include "../src/watchdog_safety.h"

const uint32_t WDT_TIMEOUT_MS = 2000;
const uint32_t SAFE_CHUNK_SIZE_MS = 100;

void test_delay_chunking_zero_delay() {
  printf("Testing delay chunking with zero delay...\n");

  DelayChunkResult result = calculateSafeDelayChunks(0, SAFE_CHUNK_SIZE_MS, WDT_TIMEOUT_MS);
  assert(result.chunkCount == 0);
  assert(result.chunkSizeMs == 0);
  assert(result.remainderMs == 0);
  assert(result.exceedsTimeout == false);

  printf("  ✓ Passed\n");
}

void test_delay_chunking_within_chunk_size() {
  printf("Testing delay chunking within chunk size...\n");

  DelayChunkResult result = calculateSafeDelayChunks(50, SAFE_CHUNK_SIZE_MS, WDT_TIMEOUT_MS);
  assert(result.chunkCount == 1);
  assert(result.chunkSizeMs == 50);
  assert(result.remainderMs == 0);
  assert(result.exceedsTimeout == false);

  printf("  ✓ Passed\n");
}

void test_delay_chunking_exactly_chunk_size() {
  printf("Testing delay chunking exactly chunk size...\n");

  DelayChunkResult result = calculateSafeDelayChunks(SAFE_CHUNK_SIZE_MS, SAFE_CHUNK_SIZE_MS, WDT_TIMEOUT_MS);
  assert(result.chunkCount == 1);
  assert(result.chunkSizeMs == SAFE_CHUNK_SIZE_MS);
  assert(result.remainderMs == 0);
  assert(result.exceedsTimeout == false);

  printf("  ✓ Passed\n");
}

void test_delay_chunking_multiple_chunks() {
  printf("Testing delay chunking with multiple chunks...\n");

  DelayChunkResult result = calculateSafeDelayChunks(350, SAFE_CHUNK_SIZE_MS, WDT_TIMEOUT_MS);
  assert(result.chunkCount == 4);
  assert(result.chunkSizeMs == SAFE_CHUNK_SIZE_MS);
  assert(result.remainderMs == 50);
  assert(result.exceedsTimeout == false);

  printf("  ✓ Passed\n");
}

void test_delay_chunking_exact_multiple() {
  printf("Testing delay chunking with exact multiple...\n");

  DelayChunkResult result = calculateSafeDelayChunks(300, SAFE_CHUNK_SIZE_MS, WDT_TIMEOUT_MS);
  assert(result.chunkCount == 3);
  assert(result.chunkSizeMs == SAFE_CHUNK_SIZE_MS);
  assert(result.remainderMs == 0);
  assert(result.exceedsTimeout == false);

  printf("  ✓ Passed\n");
}

void test_delay_chunking_unsafe_chunk_size() {
  printf("Testing delay chunking with unsafe chunk size...\n");

  DelayChunkResult result = calculateSafeDelayChunks(1000, WDT_TIMEOUT_MS, WDT_TIMEOUT_MS);
  assert(result.exceedsTimeout == true);
  assert(result.chunkCount == 0);

  printf("  ✓ Passed\n");
}

void test_watchdog_risk_safe_operation() {
  printf("Testing watchdog risk assessment for safe operation...\n");

  WatchdogRiskAssessment result = evaluateWatchdogRisk(800, WDT_TIMEOUT_MS, 0.8f);
  assert(result.isSafe == true);
  assert(result.maxSafeDelay == 1600);
  assert(result.riskFactor == 0.4f);

  printf("  ✓ Passed\n");
}

void test_watchdog_risk_unsafe_operation() {
  printf("Testing watchdog risk assessment for unsafe operation...\n");

  WatchdogRiskAssessment result = evaluateWatchdogRisk(1800, WDT_TIMEOUT_MS, 0.8f);
  assert(result.isSafe == false);
  assert(result.maxSafeDelay == 1600);
  assert(result.riskFactor == 0.9f);

  printf("  ✓ Passed\n");
}

void test_watchdog_risk_zero_duration() {
  printf("Testing watchdog risk assessment for zero duration...\n");

  WatchdogRiskAssessment result = evaluateWatchdogRisk(0, WDT_TIMEOUT_MS);
  assert(result.isSafe == true);
  assert(result.riskFactor == 0.0f);

  printf("  ✓ Passed\n");
}

void test_watchdog_reset_timing_should_reset() {
  printf("Testing watchdog reset timing - should reset...\n");

  WatchdogResetTiming result = shouldResetWatchdog(1500, 0, 1000, WDT_TIMEOUT_MS);
  assert(result.shouldReset == true);
  assert(result.timeSinceLastReset == 1500);

  printf("  ✓ Passed\n");
}

void test_watchdog_reset_timing_not_yet() {
  printf("Testing watchdog reset timing - not yet...\n");

  WatchdogResetTiming result = shouldResetWatchdog(700, 0, 1000, WDT_TIMEOUT_MS);
  assert(result.shouldReset == false);
  assert(result.timeSinceLastReset == 700);
  assert(result.timeUntilRequired == 300);

  printf("  ✓ Passed\n");
}

void test_watchdog_reset_timing_exactly_interval() {
  printf("Testing watchdog reset timing - exactly at interval...\n");

  WatchdogResetTiming result = shouldResetWatchdog(1000, 0, 1000, WDT_TIMEOUT_MS);
  assert(result.shouldReset == true);
  assert(result.timeSinceLastReset == 1000);

  printf("  ✓ Passed\n");
}

void test_calculate_safe_interval() {
  printf("Testing safe interval calculation...\n");

  uint32_t interval = calculateSafeIntervalMs(WDT_TIMEOUT_MS, 50.0f);
  assert(interval == 1000);

  interval = calculateSafeIntervalMs(WDT_TIMEOUT_MS, 25.0f);
  assert(interval == 500);

  printf("  ✓ Passed\n");
}

void test_operation_safety_check_safe() {
  printf("Testing operation safety check - safe...\n");

  bool isSafe = isOperationSafeForWatchdog(200, 500, 100);
  assert(isSafe == true);

  printf("  ✓ Passed\n");
}

void test_operation_safety_check_unsafe() {
  printf("Testing operation safety check - unsafe...\n");

  bool isSafe = isOperationSafeForWatchdog(450, 500, 100);
  assert(isSafe == false);

  printf("  ✓ Passed\n");
}

void test_recommended_chunk_size() {
  printf("Testing recommended chunk size calculation...\n");

  uint32_t chunkSize = getRecommendedChunkSize(WDT_TIMEOUT_MS, 10.0f);
  assert(chunkSize == 200);

  chunkSize = getRecommendedChunkSize(WDT_TIMEOUT_MS, 5.0f);
  assert(chunkSize == 100);

  printf("  ✓ Passed\n");
}

void test_critical_5_second_delay_scenario() {
  printf("Testing critical 5-second delay scenario...\n");

  // This tests the exact scenario from diagnostics.h:53
  DelayChunkResult result = calculateSafeDelayChunks(5000, 100, WDT_TIMEOUT_MS);
  assert(result.chunkCount == 50);
  assert(result.chunkSizeMs == 100);
  assert(result.remainderMs == 0);
  assert(result.exceedsTimeout == false);

  // Verify risk assessment shows it's unsafe without chunking
  WatchdogRiskAssessment risk = evaluateWatchdogRisk(5000, WDT_TIMEOUT_MS);
  assert(risk.isSafe == false);
  assert(risk.riskFactor == 2.5f);

  printf("  ✓ Passed\n");
}

void test_watchdog_alarm_timing_safety() {
  printf("Testing watchdog alarm timing safety...\n");

  // Current watchdog alarm settings
  const int BEEP_DURATION_MS = 150;
  const int BEEP_PAUSE_MS = 100;
  const int SET_PAUSE_MS = 200;
  const int BEEP_COUNT = 3;
  const int SET_COUNT = 2;

  // Calculate total alarm duration
  uint32_t single_set_duration = (BEEP_DURATION_MS + BEEP_PAUSE_MS) * BEEP_COUNT - BEEP_PAUSE_MS;
  uint32_t total_duration = single_set_duration * SET_COUNT + SET_PAUSE_MS * (SET_COUNT - 1);

  // Verify calculations
  assert(single_set_duration == 650);  // (150+100)*3 - 100 = 650ms per set
  assert(total_duration == 1500);      // 650*2 + 200*1 = 1500ms total

  // Verify alarm is safe for 2-second watchdog timeout
  WatchdogRiskAssessment risk = evaluateWatchdogRisk(total_duration, WDT_TIMEOUT_MS);
  assert(risk.isSafe == true);  // Should be safe (1500ms < 1600ms safety threshold)
  assert(risk.riskFactor == 0.75f);  // 1500/2000 = 0.75

  printf("  ✓ Alarm duration: %dms (SAFE for 2s watchdog)\n", total_duration);
  printf("  ✓ Passed\n");
}

int main() {
  printf("=== Watchdog Safety Tests ===\n\n");

  test_delay_chunking_zero_delay();
  test_delay_chunking_within_chunk_size();
  test_delay_chunking_exactly_chunk_size();
  test_delay_chunking_multiple_chunks();
  test_delay_chunking_exact_multiple();
  test_delay_chunking_unsafe_chunk_size();

  test_watchdog_risk_safe_operation();
  test_watchdog_risk_unsafe_operation();
  test_watchdog_risk_zero_duration();

  test_watchdog_reset_timing_should_reset();
  test_watchdog_reset_timing_not_yet();
  test_watchdog_reset_timing_exactly_interval();

  test_calculate_safe_interval();
  test_operation_safety_check_safe();
  test_operation_safety_check_unsafe();
  test_recommended_chunk_size();

  test_critical_5_second_delay_scenario();
  test_watchdog_alarm_timing_safety();

  printf("\n✅ All watchdog safety tests passed!\n");
  return 0;
}