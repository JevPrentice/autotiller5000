// Copyright 2025 Wentzel
#ifndef BUTTON_WORKFLOWS_H_
#define BUTTON_WORKFLOWS_H_

#include <avr/wdt.h>
#include "config.h"
#include "button_logic.h"
#include "button_debounce.h"
#include "navigation.h"
#include "tack_logic.h"

extern ButtonState mainButtonState;
extern ButtonState portButtonState;
extern ButtonState starboardButtonState;

void buzz(const ButtonContext& ctx,
          float& course,
          float oldcourse) {
  for (int counter = 1; counter <= ctx.courseConfig.tack_beep_count; counter++) {
    wdt_reset();
    tone(ctx.pins.buzzer, ctx.audio.tack_warning.frequency_hz);
    delay(ctx.timing.tack_beep_delay_ms);
    bool mainPressed = debouncedDigitalRead(ctx.pins.main_button, mainButtonState, ctx.timing.button_debounce_ms);
    int pushed = mainPressed ? LOW : HIGH;
    TackWarningResult result = evaluateTackWarning(
        counter, ctx.courseConfig.tack_beep_count, pushed == LOW);
    if (result.shouldCancel) {
      delay(ctx.timing.wait_time_ms);
      course = oldcourse;
      counter = result.iterationsCompleted;
      noTone(ctx.pins.buzzer);
    }
    delay(ctx.timing.wait_time_ms);
  }
}

void portadj(const ButtonContext& ctx,
             float& course) {
  int32_t pastpress = millis();
  bool portPressed = debouncedDigitalRead(ctx.pins.port_button, portButtonState, ctx.timing.button_debounce_ms);
  int portpush = portPressed ? LOW : HIGH;
  while (portpush == LOW) {
    wdt_reset();
    bool portReleased = !debouncedDigitalRead(ctx.pins.port_button, portButtonState, ctx.timing.button_debounce_ms);
    int portrelease = portReleased ? HIGH : LOW;
    if (portrelease == HIGH) {
      noTone(ctx.pins.buzzer);
      int32_t newpress = millis();
      portpush = HIGH;
      int pressedtime = newpress - pastpress;
      ButtonPressResult result = evaluateButtonPress(
          pressedtime, ctx.timing.short_press_threshold_ms,
          ctx.courseConfig.course_adjust_degrees,
          ctx.courseConfig.tack_angle_degrees, true);
      float oldcourse = course;
      if (result.isTack) {
        oldcourse = course;
      }
      course = course + result.courseAdjustment;
      if (result.isTack) {
        buzz(ctx, course, oldcourse);
      } else {
        delay(ctx.timing.wait_time_ms);
      }
      course = normalizeCourse(course);
    }
  }
}

void staradj(const ButtonContext& ctx,
             float& course) {
  int32_t pastpress = millis();
  bool starPressed = debouncedDigitalRead(ctx.pins.starboard_button, starboardButtonState, ctx.timing.button_debounce_ms);
  int starpush = starPressed ? LOW : HIGH;
  while (starpush == LOW) {
    wdt_reset();
    bool starReleased = !debouncedDigitalRead(ctx.pins.starboard_button, starboardButtonState, ctx.timing.button_debounce_ms);
    int starrelease = starReleased ? HIGH : LOW;
    if (starrelease == HIGH) {
      int32_t newpress = millis();
      noTone(ctx.pins.buzzer);
      starpush = HIGH;
      int pressedtime = newpress - pastpress;
      ButtonPressResult result = evaluateButtonPress(
          pressedtime, ctx.timing.short_press_threshold_ms,
          ctx.courseConfig.course_adjust_degrees,
          ctx.courseConfig.tack_angle_degrees, false);
      float oldcourse = course;
      if (result.isTack) {
        oldcourse = course;
      }
      course = course + result.courseAdjustment;
      if (result.isTack) {
        buzz(ctx, course, oldcourse);
      } else {
        delay(ctx.timing.wait_time_ms);
      }
      course = normalizeCourse(course);
    }
  }
}

void checkbuttonpressed(const ButtonContext& ctx,
                        float& course) {
  bool portPressed = debouncedDigitalRead(ctx.pins.port_button, portButtonState, ctx.timing.button_debounce_ms);
  bool starPressed = debouncedDigitalRead(ctx.pins.starboard_button, starboardButtonState, ctx.timing.button_debounce_ms);
  if (portPressed) {
    tone(ctx.pins.buzzer, ctx.audio.port_button.frequency_hz);
    portadj(ctx, course);
  }
  if (starPressed) {
    tone(ctx.pins.buzzer, ctx.audio.starboard_button.frequency_hz);
    staradj(ctx, course);
  }
}

#endif  // BUTTON_WORKFLOWS_H_
