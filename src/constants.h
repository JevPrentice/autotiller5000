// Copyright 2025 Wentzel
#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <stdint.h>
#include "config.h"

#define LED1 10
#define LED2 11
#define LED3 12

const int MAIN_BUTTON_PIN = 2;
const int PORT_BUTTON_PIN = 4;
const int STARBOARD_BUTTON_PIN = 5;
const int BUZZER_PIN = 8;
const int SERVO_PIN = 9;

const uint32_t I2C_TIMEOUT_MS = 100;
const int MAX_SENSOR_FAILURES = 5;
const int SENSOR_ERROR_TONE_HZ = 800;
const int SENSOR_ERROR_TONE_DURATION_MS = 1000;
const int SENSOR_RECOVERY_TONE_HZ = 1200;
const int SENSOR_RECOVERY_TONE_DURATION_MS = 200;
const int WATCHDOG_RESET_TONE_HZ = 1800;
const int WATCHDOG_RESET_BEEP_DURATION_MS = 150;
const int WATCHDOG_RESET_BEEP_PAUSE_MS = 100;
const int WATCHDOG_RESET_SET_PAUSE_MS = 200;
const int WATCHDOG_RESET_BEEP_COUNT = 3;
const int WATCHDOG_RESET_SET_COUNT = 2;

const int DEFAULT_SENSITIVITY = 100;
const int8_t DEFAULT_REVERSE = 1;
const uint32_t SAVE_BUTTON_HOLD_TIME_MS = 10000;
const uint32_t SAVE_HALFWAY_BEEP_TIME_MS = 5000;
const int SAVE_SUCCESS_TONE_HZ = 1500;
const int SAVE_SUCCESS_BEEP_DURATION_MS = 100;
const int SAVE_NO_CHANGES_TONE_HZ = 800;
const int SAVE_NO_CHANGES_TONE_DURATION_MS = 200;
const int EEPROM_WRITE_ERROR_TONE_HZ = 600;
const int EEPROM_WRITE_ERROR_TONE_DURATION_MS = 2000;

const int WAIT_TIME_MS = 300;
const int DEBOUNCE_DELAY_MS = 300;
const int SHORT_PRESS_MS = 3000;
const int SENSITIVITY_ADJUST_THRESHOLD_MS = 2000;
const int REVERSE_THRESHOLD_MS = 4000;
const int SERVO_ADJUST_DELAY_MS = 100;
const int LED_FLASH_DELAY_MS = 10;
const int TACK_WARNING_BEEP_DELAY_MS = 50;
const int SETTINGS_DEBOUNCE_DELAY_MS = 30;
const int BUTTON_DEBOUNCE_MS = 50;
const int STARTUP_DELAY_MS = 1000;
const int WDT_SAFE_DELAY_CHUNK_MS = 100;

const int PORT_BUTTON_TONE_HZ = 200;
const int STARBOARD_BUTTON_TONE_HZ = 400;
const int TACK_WARNING_TONE_HZ = 600;
const int SERVO_CENTER_POSITION = 90;
const int TACK_WARNING_BEEP_COUNT = 10;
const int SENSITIVITY_STEP = 10;
const int COURSE_ADJUST_DEGREES = 2;
const int TACK_ANGLE_DEGREES = 90;

#define BNO055_SAMPLERATE_DELAY_MS (100)

const HardwarePins pins = {
  .main_button = MAIN_BUTTON_PIN,
  .port_button = PORT_BUTTON_PIN,
  .starboard_button = STARBOARD_BUTTON_PIN,
  .buzzer = BUZZER_PIN,
  .led1 = LED1,
  .led2 = LED2,
  .led3 = LED3,
  .servo = SERVO_PIN
};

const TimingConfig timing = {
  .i2c_timeout_ms = I2C_TIMEOUT_MS,
  .sensitivity_threshold_ms = SENSITIVITY_ADJUST_THRESHOLD_MS,
  .reverse_threshold_ms = REVERSE_THRESHOLD_MS,
  .wait_time_ms = WAIT_TIME_MS,
  .settings_debounce_ms = SETTINGS_DEBOUNCE_DELAY_MS,
  .led_flash_delay_ms = LED_FLASH_DELAY_MS,
  .startup_delay_ms = STARTUP_DELAY_MS,
  .servo_adjust_delay_ms = SERVO_ADJUST_DELAY_MS,
  .short_press_threshold_ms = SHORT_PRESS_MS,
  .tack_beep_delay_ms = TACK_WARNING_BEEP_DELAY_MS,
  .button_debounce_ms = BUTTON_DEBOUNCE_MS
};

const AudioConfig audio = {
  .port_button = {.frequency_hz = PORT_BUTTON_TONE_HZ, .duration_ms = 100},
  .starboard_button = {.frequency_hz = STARBOARD_BUTTON_TONE_HZ, .duration_ms = 100},
  .tack_warning = {.frequency_hz = TACK_WARNING_TONE_HZ, .duration_ms = 100},
  .recovery = {.frequency_hz = SENSOR_RECOVERY_TONE_HZ, .duration_ms = SENSOR_RECOVERY_TONE_DURATION_MS},
  .error = {.frequency_hz = SENSOR_ERROR_TONE_HZ, .duration_ms = SENSOR_ERROR_TONE_DURATION_MS},
  .save_success = {.frequency_hz = SAVE_SUCCESS_TONE_HZ, .duration_ms = SAVE_SUCCESS_BEEP_DURATION_MS},
  .save_no_changes = {.frequency_hz = SAVE_NO_CHANGES_TONE_HZ, .duration_ms = SAVE_NO_CHANGES_TONE_DURATION_MS},
  .watchdog_reset = {
    .frequency_hz = WATCHDOG_RESET_TONE_HZ,
    .beep_duration_ms = WATCHDOG_RESET_BEEP_DURATION_MS,
    .beep_pause_ms = WATCHDOG_RESET_BEEP_PAUSE_MS,
    .set_pause_ms = WATCHDOG_RESET_SET_PAUSE_MS,
    .beep_count = WATCHDOG_RESET_BEEP_COUNT,
    .set_count = WATCHDOG_RESET_SET_COUNT
  },
  .eeprom_write_error = {.frequency_hz = EEPROM_WRITE_ERROR_TONE_HZ, .duration_ms = EEPROM_WRITE_ERROR_TONE_DURATION_MS}
};

const CourseAdjustmentConfig courseConfig = {
  .course_adjust_degrees = COURSE_ADJUST_DEGREES,
  .tack_angle_degrees = TACK_ANGLE_DEGREES,
  .tack_beep_count = TACK_WARNING_BEEP_COUNT
};

const SensorSafetyConfig safetyConfig = {
  .max_failures = MAX_SENSOR_FAILURES,
  .servo_center_position = SERVO_CENTER_POSITION
};

const SensorConfig sensorConfig = {
  .samplerate_delay_ms = BNO055_SAMPLERATE_DELAY_MS,
  .serial_baud = 115200
};

const SensitivityConfig sensConfig = {
  .step = SENSITIVITY_STEP,
  .default_value = DEFAULT_SENSITIVITY,
  .default_reverse = DEFAULT_REVERSE
};

#endif  // CONSTANTS_H_