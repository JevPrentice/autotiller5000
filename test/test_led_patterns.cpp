#include <cassert>
#include <iostream>
#include "../src/led_patterns.h"

void test_button_hold_short_press() {
  LedState state = calculateButtonHoldLeds(1000, 2000, 4000);
  assert(state.led1 == true);
  assert(state.led2 == false);
  assert(state.led3 == false);
  std::cout << "test_button_hold_short_press: PASS\n";
}

void test_button_hold_sensitivity_mode() {
  LedState state = calculateButtonHoldLeds(3000, 2000, 4000);
  assert(state.led1 == true);
  assert(state.led2 == true);
  assert(state.led3 == false);
  std::cout << "test_button_hold_sensitivity_mode: PASS\n";
}

void test_button_hold_reverse_mode() {
  LedState state = calculateButtonHoldLeds(5000, 2000, 4000);
  assert(state.led1 == true);
  assert(state.led2 == true);
  assert(state.led3 == true);
  std::cout << "test_button_hold_reverse_mode: PASS\n";
}

void test_button_hold_at_sensitivity_threshold() {
  LedState state = calculateButtonHoldLeds(2000, 2000, 4000);
  assert(state.led1 == true);
  assert(state.led2 == true);
  assert(state.led3 == false);
  std::cout << "test_button_hold_at_sensitivity_threshold: PASS\n";
}

void test_button_hold_at_reverse_threshold() {
  LedState state = calculateButtonHoldLeds(4000, 2000, 4000);
  assert(state.led1 == true);
  assert(state.led2 == true);
  assert(state.led3 == true);
  std::cout << "test_button_hold_at_reverse_threshold: PASS\n";
}

void test_button_hold_zero_elapsed() {
  LedState state = calculateButtonHoldLeds(0, 2000, 4000);
  assert(state.led1 == true);
  assert(state.led2 == false);
  assert(state.led3 == false);
  std::cout << "test_button_hold_zero_elapsed: PASS\n";
}

int main() {
  test_button_hold_short_press();
  test_button_hold_sensitivity_mode();
  test_button_hold_reverse_mode();
  test_button_hold_at_sensitivity_threshold();
  test_button_hold_at_reverse_threshold();
  test_button_hold_zero_elapsed();
  std::cout << "\nAll LED pattern tests passed!\n";
  return 0;
}