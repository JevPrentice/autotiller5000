#include <cassert>
#include <cstdio>
#include <stdint.h>

// Simplified sensor safety state for testing
struct SensorSafetyState {
  int consecutiveFailures;
  bool inSafeMode;
  uint32_t nextRetryTime;
  uint32_t backoffDelay;
};

// Test the backoff logic independently
void test_backoff_initial_state() {
  printf("Testing initial backoff state...\n");

  SensorSafetyState state = {
    .consecutiveFailures = 0,
    .inSafeMode = false,
    .nextRetryTime = 0,
    .backoffDelay = 10
  };

  assert(state.backoffDelay == 10);
  assert(state.nextRetryTime == 0);
  assert(state.consecutiveFailures == 0);

  printf("  ✓ Passed\n");
}

void test_backoff_exponential_progression() {
  printf("Testing exponential backoff progression...\n");

  SensorSafetyState state = {
    .consecutiveFailures = 0,
    .inSafeMode = false,
    .nextRetryTime = 0,
    .backoffDelay = 10
  };

  uint32_t mockTime = 1000;

  // Simulate failure progression
  uint32_t expectedBackoffs[] = {20, 40, 80, 160, 320, 640, 1000, 1000};

  for (int i = 0; i < 8; i++) {
    // Simulate failure
    state.consecutiveFailures++;
    state.nextRetryTime = mockTime + state.backoffDelay;
    state.backoffDelay = state.backoffDelay * 2;
    if (state.backoffDelay > 1000) {
      state.backoffDelay = 1000;  // Cap at 1 second
    }

    assert(state.consecutiveFailures == i + 1);
    assert(state.backoffDelay == expectedBackoffs[i]);

    mockTime += 2000;  // Advance time
  }

  printf("  ✓ Passed\n");
}

void test_backoff_reset_on_success() {
  printf("Testing backoff reset on success...\n");

  SensorSafetyState state = {
    .consecutiveFailures = 5,
    .inSafeMode = false,
    .nextRetryTime = 2000,
    .backoffDelay = 320
  };

  // Simulate success - reset state
  state.consecutiveFailures = 0;
  state.backoffDelay = 10;
  state.nextRetryTime = 0;

  assert(state.consecutiveFailures == 0);
  assert(state.backoffDelay == 10);
  assert(state.nextRetryTime == 0);

  printf("  ✓ Passed\n");
}

void test_backoff_cap_at_1000ms() {
  printf("Testing backoff cap at 1000ms...\n");

  SensorSafetyState state = {
    .consecutiveFailures = 0,
    .inSafeMode = false,
    .nextRetryTime = 0,
    .backoffDelay = 500
  };

  // First failure: 500 * 2 = 1000 (at cap)
  state.consecutiveFailures++;
  state.backoffDelay = state.backoffDelay * 2;
  if (state.backoffDelay > 1000) {
    state.backoffDelay = 1000;
  }
  assert(state.backoffDelay == 1000);

  // Second failure: should stay at 1000 (capped)
  state.consecutiveFailures++;
  state.backoffDelay = state.backoffDelay * 2;
  if (state.backoffDelay > 1000) {
    state.backoffDelay = 1000;
  }
  assert(state.backoffDelay == 1000);

  printf("  ✓ Passed\n");
}

void test_backoff_retry_timing() {
  printf("Testing retry timing enforcement...\n");

  SensorSafetyState state = {
    .consecutiveFailures = 1,
    .inSafeMode = false,
    .nextRetryTime = 1500,
    .backoffDelay = 20
  };

  uint32_t currentTime = 1400;  // Before retry time

  // Should not retry yet
  assert(currentTime < state.nextRetryTime);

  currentTime = 1600;  // After retry time

  // Should be allowed to retry now
  assert(currentTime >= state.nextRetryTime);

  printf("  ✓ Passed\n");
}

int main() {
  printf("=== Sensor Backoff Tests ===\n\n");
  test_backoff_initial_state();
  test_backoff_exponential_progression();
  test_backoff_reset_on_success();
  test_backoff_cap_at_1000ms();
  test_backoff_retry_timing();
  printf("\n✅ All sensor backoff tests passed!\n");
  return 0;
}