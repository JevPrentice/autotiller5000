#include <cassert>
#include <cmath>
#include <iostream>
#include "../src/servo_control.h"

const float EPSILON = 0.001;

bool floatEqual(float a, float b) {
  return fabs(a - b) < EPSILON;
}

void test_servo_update_center_no_adjustment() {
  ServoUpdateResult result = updateServoPosition(90.0, 0.0, 100, 1);
  assert(floatEqual(result.newPosition, 90.0));
  assert(floatEqual(result.adjustmentApplied, 0.0));
  std::cout << "test_servo_update_center_no_adjustment: PASS\n";
}

void test_servo_update_small_adjustment_positive() {
  ServoUpdateResult result = updateServoPosition(90.0, 10.0, 100, 1);
  assert(result.newPosition > 90.0);
  assert(result.newPosition < 100.0);
  assert(result.adjustmentApplied > 0.0);
  std::cout << "test_servo_update_small_adjustment_positive: PASS\n";
}

void test_servo_update_small_adjustment_negative() {
  ServoUpdateResult result = updateServoPosition(90.0, -10.0, 100, 1);
  assert(result.newPosition < 90.0);
  assert(result.newPosition > 80.0);
  assert(result.adjustmentApplied < 0.0);
  std::cout << "test_servo_update_small_adjustment_negative: PASS\n";
}

void test_servo_update_reverse_direction() {
  ServoUpdateResult result = updateServoPosition(90.0, 10.0, 100, -1);
  assert(result.newPosition < 90.0);
  assert(result.adjustmentApplied < 0.0);
  std::cout << "test_servo_update_reverse_direction: PASS\n";
}

void test_servo_update_constrain_upper_limit() {
  ServoUpdateResult result = updateServoPosition(170.0, 90.0, 1000, 1);
  assert(floatEqual(result.newPosition, 180.0));
  std::cout << "test_servo_update_constrain_upper_limit: PASS\n";
}

void test_servo_update_constrain_lower_limit() {
  ServoUpdateResult result = updateServoPosition(10.0, -90.0, 1000, 1);
  assert(floatEqual(result.newPosition, 0.0));
  std::cout << "test_servo_update_constrain_lower_limit: PASS\n";
}

void test_servo_update_high_sensitivity() {
  ServoUpdateResult result1 = updateServoPosition(90.0, 10.0, 100, 1);
  ServoUpdateResult result2 = updateServoPosition(90.0, 10.0, 500, 1);
  assert(result2.adjustmentApplied > result1.adjustmentApplied);
  std::cout << "test_servo_update_high_sensitivity: PASS\n";
}

void test_servo_update_180_degree_error() {
  ServoUpdateResult result = updateServoPosition(90.0, 180.0, 100, 1);
  assert(fabs(result.newPosition - 90.0) < 0.1);
  assert(fabs(result.adjustmentApplied) < 0.1);
  std::cout << "test_servo_update_180_degree_error: PASS\n";
}

void test_servo_update_90_degree_error() {
  ServoUpdateResult result = updateServoPosition(90.0, 90.0, 100, 1);
  assert(result.newPosition > 90.0);
  assert(floatEqual(result.adjustmentApplied, 10.0));
  std::cout << "test_servo_update_90_degree_error: PASS\n";
}

void test_servo_update_negative_90_degree_error() {
  ServoUpdateResult result = updateServoPosition(90.0, -90.0, 100, 1);
  assert(result.newPosition < 90.0);
  assert(floatEqual(result.adjustmentApplied, -10.0));
  std::cout << "test_servo_update_negative_90_degree_error: PASS\n";
}

void test_servo_update_minimum_sensitivity() {
  ServoUpdateResult result = updateServoPosition(90.0, 90.0, 30, 1);
  assert(result.newPosition > 90.0);
  assert(floatEqual(result.adjustmentApplied, 3.0));
  std::cout << "test_servo_update_minimum_sensitivity: PASS\n";
}

void test_servo_update_maximum_sensitivity() {
  ServoUpdateResult result = updateServoPosition(90.0, 90.0, 1000, 1);
  assert(result.newPosition > 90.0);
  assert(floatEqual(result.adjustmentApplied, 100.0));
  std::cout << "test_servo_update_maximum_sensitivity: PASS\n";
}

void test_servo_update_from_boundary_zero() {
  ServoUpdateResult result = updateServoPosition(0.0, 10.0, 100, 1);
  assert(result.newPosition > 0.0);
  std::cout << "test_servo_update_from_boundary_zero: PASS\n";
}

void test_servo_update_from_boundary_180() {
  ServoUpdateResult result = updateServoPosition(180.0, -10.0, 100, 1);
  assert(result.newPosition < 180.0);
  std::cout << "test_servo_update_from_boundary_180: PASS\n";
}

int main() {
  std::cout << "=== Servo Control Tests ===\n\n";

  test_servo_update_center_no_adjustment();
  test_servo_update_small_adjustment_positive();
  test_servo_update_small_adjustment_negative();
  test_servo_update_reverse_direction();
  test_servo_update_constrain_upper_limit();
  test_servo_update_constrain_lower_limit();
  test_servo_update_high_sensitivity();
  test_servo_update_180_degree_error();
  test_servo_update_90_degree_error();
  test_servo_update_negative_90_degree_error();
  test_servo_update_minimum_sensitivity();
  test_servo_update_maximum_sensitivity();
  test_servo_update_from_boundary_zero();
  test_servo_update_from_boundary_180();

  std::cout << "\n✅ All servo control tests passed!\n";
  return 0;
}