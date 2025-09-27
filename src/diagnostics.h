// Copyright 2025 Wentzel
#ifndef DIAGNOSTICS_H_
#define DIAGNOSTICS_H_

#include <Adafruit_BNO055.h>
#include <avr/wdt.h>
#include "constants.h"

extern Adafruit_BNO055 bno;

inline void LEDOFF_DIAG(int led1, int led2, int led3) {
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
}

void displaySensorDetails(void) {
  sensor_t sensor;
  bno.getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.print(F("Sensor:       "));
  Serial.println(sensor.name);
  Serial.print(F("Driver Ver:   "));
  Serial.println(sensor.version);
  Serial.print(F("Unique ID:    "));
  Serial.println(sensor.sensor_id);
  Serial.print(F("Max Value:    "));
  Serial.print(sensor.max_value);
  Serial.println(F(" xxx"));
  Serial.print(F("Min Value:    "));
  Serial.print(sensor.min_value);
  Serial.println(F(" xxx"));
  Serial.print(F("Resolution:   "));
  Serial.print(sensor.resolution);
  Serial.println(F(" xxx"));
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));
  delay(500);
}

void displaySensorStatus(void) {
  uint8_t system_status, self_test_results, system_error;
  system_status = self_test_results = system_error = 0;
  bno.getSystemStatus(&system_status, &self_test_results, &system_error);

  Serial.println(F(""));
  Serial.print(F("System Status: 0x"));
  Serial.println(system_status, HEX);
  Serial.print(F("Self Test:     0x"));
  Serial.println(self_test_results, HEX);
  Serial.print(F("System Error:  0x"));
  Serial.println(system_error, HEX);
  Serial.println(F(""));
  digitalWrite(LED3, HIGH);

  // Safe 5-second delay with watchdog protection
  for (int i = 0; i < 50; i++) {
    wdt_reset();
    delay(WDT_SAFE_DELAY_CHUNK_MS);
  }

  LEDOFF_DIAG(LED1, LED2, LED3);
}

void displayCalStatus(void) {
  uint8_t system, gyro, accel, mag;
  system = gyro = accel = mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);

  Serial.print(F("\t"));
  if (!system) {
    Serial.print(F("! "));
  }

  Serial.print(F("Sys:"));
  Serial.print(system, DEC);
  if (system >= 1) {
    digitalWrite(LED1, HIGH);
    delay(10);
  }
  if (system >= 2) {
    digitalWrite(LED2, HIGH);
    delay(10);
  }
  if (system >= 3) {
    digitalWrite(LED3, HIGH);
    delay(10);
  }
  Serial.print(F(" G:"));
  Serial.print(gyro, DEC);
  Serial.print(F(" A:"));
  Serial.print(accel, DEC);
  Serial.print(F(" M:"));
  Serial.print(mag, DEC);
}

void displayWatchdogHealth(const WatchdogHealthMonitor& wdtMonitor) {
  Serial.println(F("=== Watchdog Health Status ==="));
  Serial.print(F("Reset Count: "));
  Serial.println(wdtMonitor.reset_count);

  if (wdtMonitor.reset_count > 0) {
    Serial.print(F("Last Reset: "));
    Serial.print(wdtMonitor.last_reset_time);
    Serial.println(F("ms ago"));

    Serial.print(F("Uptime at Reset: "));
    Serial.print(wdtMonitor.uptime_at_last_reset);
    Serial.println(F("ms"));
  }

  Serial.print(F("Current Uptime: "));
  Serial.print(millis());
  Serial.println(F("ms"));

  Serial.print(F("Monitoring: "));
  Serial.println(wdtMonitor.monitoring_enabled ? F("ENABLED") : F("DISABLED"));
  Serial.println(F("============================="));
  Serial.println(F(""));
}

void displaySensorOffsets(const adafruit_bno055_offsets_t &calibData) {
  Serial.print(F("Accelerometer: "));
  Serial.print(calibData.accel_offset_x);
  Serial.print(F(" "));
  Serial.print(calibData.accel_offset_y);
  Serial.print(F(" "));
  Serial.print(calibData.accel_offset_z);
  Serial.print(F(" "));

  Serial.print(F("\nGyro: "));
  Serial.print(calibData.gyro_offset_x);
  Serial.print(F(" "));
  Serial.print(calibData.gyro_offset_y);
  Serial.print(F(" "));
  Serial.print(calibData.gyro_offset_z);
  Serial.print(F(" "));

  Serial.print(F("\nMag: "));
  Serial.print(calibData.mag_offset_x);
  Serial.print(F(" "));
  Serial.print(calibData.mag_offset_y);
  Serial.print(F(" "));
  Serial.print(calibData.mag_offset_z);
  Serial.print(F(" "));

  Serial.print(F("\nAccel Radius: "));
  Serial.print(calibData.accel_radius);

  Serial.print(F("\nMag Radius: "));
  Serial.print(calibData.mag_radius);
}

#endif  // DIAGNOSTICS_H_
