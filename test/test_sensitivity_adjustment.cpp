#include <cassert>
#include <iostream>
#include "../src/sensitivity_adjustment.h"

void test_no_button_pressed() {
  SensitivityAdjustmentResult result = adjustSensitivity(100, false, false, 10);
  assert(result.newSensitivity == 100);
  assert(result.buzzerFrequency == 0);
  std::cout << "test_no_button_pressed: PASS\n";
}

void test_port_decrease_sensitivity() {
  SensitivityAdjustmentResult result = adjustSensitivity(100, true, false, 10);
  assert(result.newSensitivity == 90);
  assert(result.buzzerFrequency == 90);
  std::cout << "test_port_decrease_sensitivity: PASS\n";
}

void test_starboard_increase_sensitivity() {
  SensitivityAdjustmentResult result = adjustSensitivity(100, false, true, 10);
  assert(result.newSensitivity == 110);
  assert(result.buzzerFrequency == 110);
  std::cout << "test_starboard_increase_sensitivity: PASS\n";
}

void test_decrease_to_minimum() {
  SensitivityAdjustmentResult result = adjustSensitivity(30, true, false, 10);
  assert(result.newSensitivity == 30);
  assert(result.buzzerFrequency == 30);
  std::cout << "test_decrease_to_minimum: PASS\n";
}

void test_decrease_below_minimum() {
  SensitivityAdjustmentResult result = adjustSensitivity(35, true, false, 10);
  assert(result.newSensitivity == 30);
  assert(result.buzzerFrequency == 30);
  std::cout << "test_decrease_below_minimum: PASS\n";
}

void test_increase_to_maximum() {
  SensitivityAdjustmentResult result = adjustSensitivity(1000, false, true, 10);
  assert(result.newSensitivity == 1000);
  assert(result.buzzerFrequency == 1000);
  std::cout << "test_increase_to_maximum: PASS\n";
}

void test_increase_above_maximum() {
  SensitivityAdjustmentResult result = adjustSensitivity(995, false, true, 10);
  assert(result.newSensitivity == 1000);
  assert(result.buzzerFrequency == 1000);
  std::cout << "test_increase_above_maximum: PASS\n";
}

void test_custom_step_size() {
  SensitivityAdjustmentResult result = adjustSensitivity(100, false, true, 50);
  assert(result.newSensitivity == 150);
  assert(result.buzzerFrequency == 150);
  std::cout << "test_custom_step_size: PASS\n";
}

void test_large_decrease_step() {
  SensitivityAdjustmentResult result = adjustSensitivity(100, true, false, 100);
  assert(result.newSensitivity == 30);
  assert(result.buzzerFrequency == 30);
  std::cout << "test_large_decrease_step: PASS\n";
}

void test_large_increase_step() {
  SensitivityAdjustmentResult result = adjustSensitivity(100, false, true, 1000);
  assert(result.newSensitivity == 1000);
  assert(result.buzzerFrequency == 1000);
  std::cout << "test_large_increase_step: PASS\n";
}

void test_buzzer_frequency_matches_sensitivity() {
  SensitivityAdjustmentResult result1 = adjustSensitivity(500, true, false, 10);
  assert(result1.newSensitivity == 490);
  assert(result1.buzzerFrequency == 490);

  SensitivityAdjustmentResult result2 = adjustSensitivity(500, false, true, 10);
  assert(result2.newSensitivity == 510);
  assert(result2.buzzerFrequency == 510);

  std::cout << "test_buzzer_frequency_matches_sensitivity: PASS\n";
}

void test_boundary_30_decrease() {
  SensitivityAdjustmentResult result = adjustSensitivity(40, true, false, 10);
  assert(result.newSensitivity == 30);
  assert(result.buzzerFrequency == 30);
  std::cout << "test_boundary_30_decrease: PASS\n";
}

void test_boundary_1000_increase() {
  SensitivityAdjustmentResult result = adjustSensitivity(990, false, true, 10);
  assert(result.newSensitivity == 1000);
  assert(result.buzzerFrequency == 1000);
  std::cout << "test_boundary_1000_increase: PASS\n";
}

int main() {
  std::cout << "=== Sensitivity Adjustment Tests ===\n\n";

  test_no_button_pressed();
  test_port_decrease_sensitivity();
  test_starboard_increase_sensitivity();
  test_decrease_to_minimum();
  test_decrease_below_minimum();
  test_increase_to_maximum();
  test_increase_above_maximum();
  test_custom_step_size();
  test_large_decrease_step();
  test_large_increase_step();
  test_buzzer_frequency_matches_sensitivity();
  test_boundary_30_decrease();
  test_boundary_1000_increase();

  std::cout << "\n✅ All sensitivity adjustment tests passed!\n";
  return 0;
}