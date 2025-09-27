#include <cassert>
#include <cmath>
#include <cstdio>
#include "../src/navigation.h"

void test_calculateHeadingAdjustment() {
  printf("Testing calculateHeadingAdjustment()...\n");
  assert(calculateHeadingAdjustment(10, 350) == 20);
  assert(calculateHeadingAdjustment(350, 10) == -20);
  assert(calculateHeadingAdjustment(180, 0) == 180);
  assert(calculateHeadingAdjustment(0, 180) == 180);
  assert(calculateHeadingAdjustment(90, 90) == 0);
  printf("  ✓ Passed\n");
}

void test_normalizeCourse() {
  printf("Testing normalizeCourse()...\n");
  assert(normalizeCourse(370) == 10);
  assert(normalizeCourse(-10) == 350);
  assert(normalizeCourse(180) == 180);
  assert(normalizeCourse(0) == 0);
  assert(normalizeCourse(360) == 0);
  assert(normalizeCourse(720) == 0);
  printf("  ✓ Passed\n");
}

void test_calculateServoAdjustment() {
  printf("Testing calculateServoAdjustment()...\n");
  assert(fabs(calculateServoAdjustment(0, 100, 1) - 0.0) < 0.01);
  assert(fabs(calculateServoAdjustment(10, 100, 1) - 1.736) < 0.01);
  assert(fabs(calculateServoAdjustment(90, 100, 1) - 10.0) < 0.01);
  assert(fabs(calculateServoAdjustment(10, 100, -1) + 1.736) < 0.01);
  printf("  ✓ Passed\n");
}

void test_constrainServoPosition() {
  printf("Testing constrainServoPosition()...\n");
  assert(constrainServoPosition(-10) == 0);
  assert(constrainServoPosition(0) == 0);
  assert(constrainServoPosition(90) == 90);
  assert(constrainServoPosition(180) == 180);
  assert(constrainServoPosition(200) == 180);
  printf("  ✓ Passed\n");
}

void test_clampSensitivity() {
  printf("Testing clampSensitivity()...\n");
  assert(clampSensitivity(20) == 30);
  assert(clampSensitivity(30) == 30);
  assert(clampSensitivity(500) == 500);
  assert(clampSensitivity(1000) == 1000);
  assert(clampSensitivity(1500) == 1000);
  printf("  ✓ Passed\n");
}

int main() {
  printf("=== Navigation Tests ===\n\n");
  test_calculateHeadingAdjustment();
  test_normalizeCourse();
  test_calculateServoAdjustment();
  test_constrainServoPosition();
  test_clampSensitivity();
  printf("\n✅ All navigation tests passed!\n");
  return 0;
}