#include <cassert>
#include <cstdio>
#include "../src/tack_logic.h"

const int TACK_WARNING_BEEP_COUNT = 10;

void test_tack_not_cancelled() {
  printf("Testing tack warning (no cancellation)...\n");
  TackWarningResult result = evaluateTackWarning(
      5, TACK_WARNING_BEEP_COUNT, false);
  assert(result.shouldCancel == false);
  assert(result.iterationsCompleted == 5);

  result = evaluateTackWarning(
      1, TACK_WARNING_BEEP_COUNT, false);
  assert(result.shouldCancel == false);
  assert(result.iterationsCompleted == 1);
  printf("  ✓ Passed\n");
}

void test_tack_cancelled() {
  printf("Testing tack warning (cancelled)...\n");
  TackWarningResult result = evaluateTackWarning(
      3, TACK_WARNING_BEEP_COUNT, true);
  assert(result.shouldCancel == true);
  assert(result.iterationsCompleted == TACK_WARNING_BEEP_COUNT);

  result = evaluateTackWarning(
      1, TACK_WARNING_BEEP_COUNT, true);
  assert(result.shouldCancel == true);
  assert(result.iterationsCompleted == TACK_WARNING_BEEP_COUNT);
  printf("  ✓ Passed\n");
}

void test_tack_edge_cases() {
  printf("Testing tack warning (edge cases)...\n");
  TackWarningResult result = evaluateTackWarning(
      TACK_WARNING_BEEP_COUNT, TACK_WARNING_BEEP_COUNT, false);
  assert(result.shouldCancel == false);

  result = evaluateTackWarning(
      TACK_WARNING_BEEP_COUNT, TACK_WARNING_BEEP_COUNT, true);
  assert(result.shouldCancel == true);
  printf("  ✓ Passed\n");
}

int main() {
  printf("=== Tack Logic Tests ===\n\n");
  test_tack_not_cancelled();
  test_tack_cancelled();
  test_tack_edge_cases();
  printf("\n✅ All tack logic tests passed!\n");
  return 0;
}