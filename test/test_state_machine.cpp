#include <cassert>
#include <iostream>
#include "../src/state_machine.h"

void test_exit_course_mode_short_press() {
  ButtonHoldAction action = determineButtonHoldAction(1000, 2000, 4000);
  assert(action == ACTION_EXIT_COURSE_MODE);
  std::cout << "test_exit_course_mode_short_press: PASS\n";
}

void test_exit_course_mode_at_threshold() {
  ButtonHoldAction action = determineButtonHoldAction(2000, 2000, 4000);
  assert(action == ACTION_EXIT_COURSE_MODE);
  std::cout << "test_exit_course_mode_at_threshold: PASS\n";
}

void test_adjust_sensitivity_mid_range() {
  ButtonHoldAction action = determineButtonHoldAction(3000, 2000, 4000);
  assert(action == ACTION_ADJUST_SENSITIVITY);
  std::cout << "test_adjust_sensitivity_mid_range: PASS\n";
}

void test_adjust_sensitivity_just_over_threshold() {
  ButtonHoldAction action = determineButtonHoldAction(2001, 2000, 4000);
  assert(action == ACTION_ADJUST_SENSITIVITY);
  std::cout << "test_adjust_sensitivity_just_over_threshold: PASS\n";
}

void test_adjust_sensitivity_just_under_reverse() {
  ButtonHoldAction action = determineButtonHoldAction(3999, 2000, 4000);
  assert(action == ACTION_ADJUST_SENSITIVITY);
  std::cout << "test_adjust_sensitivity_just_under_reverse: PASS\n";
}

void test_toggle_reverse_at_threshold() {
  ButtonHoldAction action = determineButtonHoldAction(4000, 2000, 4000);
  assert(action == ACTION_TOGGLE_REVERSE);
  std::cout << "test_toggle_reverse_at_threshold: PASS\n";
}

void test_toggle_reverse_long_press() {
  ButtonHoldAction action = determineButtonHoldAction(5000, 2000, 4000);
  assert(action == ACTION_TOGGLE_REVERSE);
  std::cout << "test_toggle_reverse_long_press: PASS\n";
}

void test_toggle_reverse_very_long_press() {
  ButtonHoldAction action = determineButtonHoldAction(10000, 2000, 4000);
  assert(action == ACTION_TOGGLE_REVERSE);
  std::cout << "test_toggle_reverse_very_long_press: PASS\n";
}

void test_exit_course_mode_instant_release() {
  ButtonHoldAction action = determineButtonHoldAction(0, 2000, 4000);
  assert(action == ACTION_EXIT_COURSE_MODE);
  std::cout << "test_exit_course_mode_instant_release: PASS\n";
}

void test_exit_course_mode_100ms() {
  ButtonHoldAction action = determineButtonHoldAction(100, 2000, 4000);
  assert(action == ACTION_EXIT_COURSE_MODE);
  std::cout << "test_exit_course_mode_100ms: PASS\n";
}

void test_custom_thresholds() {
  ButtonHoldAction action = determineButtonHoldAction(1500, 1000, 3000);
  assert(action == ACTION_ADJUST_SENSITIVITY);
  std::cout << "test_custom_thresholds: PASS\n";
}

int main() {
  std::cout << "=== State Machine Tests ===\n\n";

  test_exit_course_mode_short_press();
  test_exit_course_mode_at_threshold();
  test_adjust_sensitivity_mid_range();
  test_adjust_sensitivity_just_over_threshold();
  test_adjust_sensitivity_just_under_reverse();
  test_toggle_reverse_at_threshold();
  test_toggle_reverse_long_press();
  test_toggle_reverse_very_long_press();
  test_exit_course_mode_instant_release();
  test_exit_course_mode_100ms();
  test_custom_thresholds();

  std::cout << "\n✅ All state machine tests passed!\n";
  return 0;
}