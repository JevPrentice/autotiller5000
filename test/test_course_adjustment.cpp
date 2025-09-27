#include <cassert>
#include <cmath>
#include <iostream>
#include "../src/course_adjustment.h"

const float EPSILON = 0.001;

bool floatEqual(float a, float b) {
  return fabs(a - b) < EPSILON;
}

void test_small_course_adjustment_port() {
  CourseAdjustmentResult result = applyCourseAdjustment(90.0, -2.0);
  assert(floatEqual(result.newCourse, 88.0));
  assert(result.wasTack == false);
  std::cout << "test_small_course_adjustment_port: PASS\n";
}

void test_small_course_adjustment_starboard() {
  CourseAdjustmentResult result = applyCourseAdjustment(90.0, 2.0);
  assert(floatEqual(result.newCourse, 92.0));
  assert(result.wasTack == false);
  std::cout << "test_small_course_adjustment_starboard: PASS\n";
}

void test_tack_port() {
  CourseAdjustmentResult result = applyCourseAdjustment(90.0, -90.0);
  assert(floatEqual(result.newCourse, 0.0));
  assert(result.wasTack == true);
  std::cout << "test_tack_port: PASS\n";
}

void test_tack_starboard() {
  CourseAdjustmentResult result = applyCourseAdjustment(90.0, 90.0);
  assert(floatEqual(result.newCourse, 180.0));
  assert(result.wasTack == true);
  std::cout << "test_tack_starboard: PASS\n";
}

void test_wraparound_forward() {
  CourseAdjustmentResult result = applyCourseAdjustment(350.0, 20.0);
  assert(floatEqual(result.newCourse, 10.0));
  assert(result.wasTack == false);
  std::cout << "test_wraparound_forward: PASS\n";
}

void test_wraparound_backward() {
  CourseAdjustmentResult result = applyCourseAdjustment(10.0, -20.0);
  assert(floatEqual(result.newCourse, 350.0));
  assert(result.wasTack == false);
  std::cout << "test_wraparound_backward: PASS\n";
}

void test_tack_with_wraparound_port() {
  CourseAdjustmentResult result = applyCourseAdjustment(45.0, -90.0);
  assert(floatEqual(result.newCourse, 315.0));
  assert(result.wasTack == true);
  std::cout << "test_tack_with_wraparound_port: PASS\n";
}

void test_tack_with_wraparound_starboard() {
  CourseAdjustmentResult result = applyCourseAdjustment(315.0, 90.0);
  assert(floatEqual(result.newCourse, 45.0));
  assert(result.wasTack == true);
  std::cout << "test_tack_with_wraparound_starboard: PASS\n";
}

void test_360_degrees_wraparound() {
  CourseAdjustmentResult result = applyCourseAdjustment(358.0, 2.0);
  assert(floatEqual(result.newCourse, 0.0));
  assert(result.wasTack == false);
  std::cout << "test_360_degrees_wraparound: PASS (FIXED)\n";
}

void test_multiple_wraparound() {
  CourseAdjustmentResult result = applyCourseAdjustment(350.0, 370.0);
  assert(floatEqual(result.newCourse, 0.0));
  assert(result.wasTack == true);
  std::cout << "test_multiple_wraparound: PASS (FIXED)\n";
}

void test_zero_adjustment() {
  CourseAdjustmentResult result = applyCourseAdjustment(180.0, 0.0);
  assert(floatEqual(result.newCourse, 180.0));
  assert(result.wasTack == false);
  std::cout << "test_zero_adjustment: PASS\n";
}

void test_exact_360_boundary() {
  CourseAdjustmentResult result = applyCourseAdjustment(0.0, 0.0);
  assert(floatEqual(result.newCourse, 0.0));
  assert(result.wasTack == false);
  std::cout << "test_exact_360_boundary: PASS\n";
}

int main() {
  std::cout << "=== Course Adjustment Tests ===\n\n";

  test_small_course_adjustment_port();
  test_small_course_adjustment_starboard();
  test_tack_port();
  test_tack_starboard();
  test_wraparound_forward();
  test_wraparound_backward();
  test_tack_with_wraparound_port();
  test_tack_with_wraparound_starboard();
  test_360_degrees_wraparound();
  test_multiple_wraparound();
  test_zero_adjustment();
  test_exact_360_boundary();

  std::cout << "\n✅ All course adjustment tests passed!\n";
  return 0;
}