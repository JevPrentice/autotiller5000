#include <cassert>
#include <cstdio>
#include "../src/button_logic.h"

const int SHORT_PRESS_MS = 3000;
const int COURSE_ADJUST_DEGREES = 2;
const int TACK_ANGLE_DEGREES = 90;

void test_button_press_short() {
  printf("Testing button press (short press)...\n");
  ButtonPressResult result = evaluateButtonPress(
      1000, SHORT_PRESS_MS, COURSE_ADJUST_DEGREES, TACK_ANGLE_DEGREES, true);
  assert(result.isTack == false);
  assert(result.courseAdjustment == -COURSE_ADJUST_DEGREES);

  result = evaluateButtonPress(
      2999, SHORT_PRESS_MS, COURSE_ADJUST_DEGREES, TACK_ANGLE_DEGREES, false);
  assert(result.isTack == false);
  assert(result.courseAdjustment == COURSE_ADJUST_DEGREES);
  printf("  ✓ Passed\n");
}

void test_button_press_long() {
  printf("Testing button press (long press/tack)...\n");
  ButtonPressResult result = evaluateButtonPress(
      3000, SHORT_PRESS_MS, COURSE_ADJUST_DEGREES, TACK_ANGLE_DEGREES, true);
  assert(result.isTack == true);
  assert(result.courseAdjustment == -TACK_ANGLE_DEGREES);

  result = evaluateButtonPress(
      5000, SHORT_PRESS_MS, COURSE_ADJUST_DEGREES, TACK_ANGLE_DEGREES, false);
  assert(result.isTack == true);
  assert(result.courseAdjustment == TACK_ANGLE_DEGREES);
  printf("  ✓ Passed\n");
}

void test_button_press_edge_cases() {
  printf("Testing button press (edge cases)...\n");
  ButtonPressResult result = evaluateButtonPress(
      3000, SHORT_PRESS_MS, COURSE_ADJUST_DEGREES, TACK_ANGLE_DEGREES, true);
  assert(result.isTack == true);

  result = evaluateButtonPress(
      0, SHORT_PRESS_MS, COURSE_ADJUST_DEGREES, TACK_ANGLE_DEGREES, false);
  assert(result.isTack == false);
  printf("  ✓ Passed\n");
}

int main() {
  printf("=== Button Logic Tests ===\n\n");
  test_button_press_short();
  test_button_press_long();
  test_button_press_edge_cases();
  printf("\n✅ All button logic tests passed!\n");
  return 0;
}