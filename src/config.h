// Copyright 2025 Wentzel
#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdint.h>
#include <Adafruit_Sensor.h>
#include <Servo.h>

// Forward declarations
struct UserSettings;
struct SensorSafetyState;

struct HardwarePins {
  int main_button;
  int port_button;
  int starboard_button;
  int buzzer;
  int led1;
  int led2;
  int led3;
  int servo;
};

struct TimingConfig {
  uint32_t i2c_timeout_ms;
  uint16_t sensitivity_threshold_ms;
  uint16_t reverse_threshold_ms;
  uint16_t wait_time_ms;
  uint16_t settings_debounce_ms;
  uint16_t led_flash_delay_ms;
  uint16_t startup_delay_ms;
  uint16_t servo_adjust_delay_ms;
  uint16_t short_press_threshold_ms;
  uint16_t tack_beep_delay_ms;
  uint16_t button_debounce_ms;
};

struct ButtonState {
  bool currentState;
  bool lastState;
  uint32_t lastChangeTime;
  bool debounced;
};

struct ToneSettings {
  int frequency_hz;
  int duration_ms;
};

struct WatchdogAlarmSettings {
  int frequency_hz;
  int beep_duration_ms;
  int beep_pause_ms;
  int set_pause_ms;
  int beep_count;
  int set_count;
};

struct AudioConfig {
  ToneSettings port_button;
  ToneSettings starboard_button;
  ToneSettings tack_warning;
  ToneSettings recovery;
  ToneSettings error;
  ToneSettings save_success;
  ToneSettings save_no_changes;
  WatchdogAlarmSettings watchdog_reset;
  ToneSettings eeprom_write_error;
};

struct CourseAdjustmentConfig {
  int course_adjust_degrees;
  int tack_angle_degrees;
  int tack_beep_count;
};

struct NavigationState {
  float course;
  float heading;
  float heading_adjustment;
  float servo_position;
  int sensitivity;
  int8_t reverse;
};

struct SensorSafetyConfig {
  int max_failures;
  int servo_center_position;
};

struct SensorConfig {
  uint16_t samplerate_delay_ms;
  uint32_t serial_baud;
};

struct SensitivityConfig {
  int step;
  int default_value;
  int8_t default_reverse;
};

struct WatchdogHealthMonitor {
  uint16_t reset_count;
  uint32_t last_reset_time;
  uint32_t uptime_at_last_reset;
  bool monitoring_enabled;
};

// Context structs for parameter grouping
struct LoopContext {
  const HardwarePins& pins;
  const TimingConfig& timing;
  const AudioConfig& audio;
  const SensorSafetyConfig& safetyConfig;
};

struct SensorReadContext {
  sensors_event_t* event;
  SensorSafetyState& sensorState;
  Servo& servo;
  int& pushed;
};

struct SaveCommandContext {
  int pushed;
  UserSettings& settings;
  UserSettings& lastSaved;
  uint32_t save_hold_time_ms;
  uint32_t halfway_beep_time_ms;
};

struct ButtonContext {
  const HardwarePins& pins;
  const AudioConfig& audio;
  const CourseAdjustmentConfig& courseConfig;
  const TimingConfig& timing;
};

#endif  // CONFIG_H_
