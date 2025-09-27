#include <cassert>
#include <cstdio>
#include <stdint.h>

// Mock Arduino constants
#define HIGH 1
#define LOW 0

// Mock Arduino functions
bool mockDigitalReadValue = true;  // Start with button not pressed
uint32_t mockMillisValue = 0;

bool digitalRead(int pin) {
  return mockDigitalReadValue ? HIGH : LOW;
}

uint32_t millis() {
  return mockMillisValue;
}

// Simple button state struct for testing
struct ButtonState {
  bool currentState;
  bool lastState;
  uint32_t lastChangeTime;
  bool debounced;
};

// Include the debouncing function directly
bool debouncedDigitalRead(int pin, ButtonState& state, uint16_t debounceMs) {
  bool rawReading = digitalRead(pin) == LOW;  // LOW means pressed
  uint32_t currentTime = millis();

  if (rawReading != state.lastState) {
    state.lastChangeTime = currentTime;
  }

  if ((currentTime - state.lastChangeTime) >= debounceMs) {
    if (rawReading != state.currentState) {
      state.currentState = rawReading;
      state.debounced = true;
    }
  } else {
    state.debounced = false;
  }

  state.lastState = rawReading;
  return state.currentState;
}

void test_debounce_basic_functionality() {
  printf("Testing basic debounce functionality...\n");

  ButtonState state = {false, false, 0, false};

  // Start with button not pressed
  mockDigitalReadValue = true;  // HIGH = not pressed
  mockMillisValue = 0;
  assert(debouncedDigitalRead(2, state, 50) == false);

  // Press button but don't wait long enough
  mockDigitalReadValue = false;  // LOW = pressed
  mockMillisValue = 10;
  assert(debouncedDigitalRead(2, state, 50) == false);  // Not debounced yet

  // Wait long enough for debounce
  mockMillisValue = 60;  // 50ms after change
  assert(debouncedDigitalRead(2, state, 50) == true);   // Now debounced

  printf("  ✓ Passed\n");
}

void test_debounce_noise_rejection() {
  printf("Testing noise rejection...\n");

  ButtonState state = {false, false, 0, false};

  // Start not pressed
  mockDigitalReadValue = true;
  mockMillisValue = 0;
  debouncedDigitalRead(2, state, 50);

  // Simulate noise - rapid changes
  mockDigitalReadValue = false; mockMillisValue = 5;  debouncedDigitalRead(2, state, 50);
  mockDigitalReadValue = true;  mockMillisValue = 10; debouncedDigitalRead(2, state, 50);
  mockDigitalReadValue = false; mockMillisValue = 15; debouncedDigitalRead(2, state, 50);
  mockDigitalReadValue = true;  mockMillisValue = 20; debouncedDigitalRead(2, state, 50);

  // Should still be not pressed due to noise
  assert(debouncedDigitalRead(2, state, 50) == false);

  printf("  ✓ Passed\n");
}

void test_debounce_stable_press() {
  printf("Testing stable button press...\n");

  ButtonState state = {false, false, 0, false};

  // Not pressed initially
  mockDigitalReadValue = true;
  mockMillisValue = 100;
  debouncedDigitalRead(2, state, 50);

  // Press and hold
  mockDigitalReadValue = false;
  mockMillisValue = 110;
  assert(debouncedDigitalRead(2, state, 50) == false);  // Not yet

  mockMillisValue = 170;  // 60ms later
  assert(debouncedDigitalRead(2, state, 50) == true);   // Now registered

  // Continue holding
  mockMillisValue = 200;
  assert(debouncedDigitalRead(2, state, 50) == true);   // Still pressed

  printf("  ✓ Passed\n");
}

void test_debounce_release() {
  printf("Testing button release...\n");

  ButtonState state = {true, true, 0, false};  // Start pressed

  // Release button
  mockDigitalReadValue = true;  // HIGH = released
  mockMillisValue = 100;
  assert(debouncedDigitalRead(2, state, 50) == true);   // Not yet

  mockMillisValue = 160;  // 60ms later
  assert(debouncedDigitalRead(2, state, 50) == false);  // Now released

  printf("  ✓ Passed\n");
}

int main() {
  printf("=== Button Debounce Tests ===\n\n");
  test_debounce_basic_functionality();
  test_debounce_noise_rejection();
  test_debounce_stable_press();
  test_debounce_release();
  printf("\n✅ All button debounce tests passed!\n");
  return 0;
}