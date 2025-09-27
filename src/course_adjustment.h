// Copyright 2025 Wentzel
#ifndef COURSE_ADJUSTMENT_H
#define COURSE_ADJUSTMENT_H

#include "navigation.h"

struct CourseAdjustmentResult {
  float newCourse;
  bool wasTack;
};

CourseAdjustmentResult applyCourseAdjustment(float currentCourse,
                                               float adjustmentDegrees) {
  CourseAdjustmentResult result;
  result.newCourse = currentCourse + adjustmentDegrees;
  result.newCourse = normalizeCourse(result.newCourse);
  result.wasTack = (adjustmentDegrees >= 90.0 || adjustmentDegrees <= -90.0);
  return result;
}

#endif