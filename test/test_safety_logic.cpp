#include <cassert>
#include <cstdio>
#include "../src/safety_logic.h"

const uint32_t SAVE_HALFWAY_BEEP_TIME_MS = 5000;
const uint32_t SAVE_BUTTON_HOLD_TIME_MS = 10000;
const int MAX_SENSOR_FAILURES = 5;

void test_save_command() {
  printf("Testing save command logic...\n");

  SaveCommandResult result = evaluateSaveCommand(5000, 0, true, true, false, true,
      SAVE_HALFWAY_BEEP_TIME_MS, SAVE_BUTTON_HOLD_TIME_MS);
  assert(result.playHalfwayBeep == true && result.shouldSave == false);

  result = evaluateSaveCommand(10000, 0, true, true, true, true,
      SAVE_HALFWAY_BEEP_TIME_MS, SAVE_BUTTON_HOLD_TIME_MS);
  assert(result.shouldSave == true && result.playSuccessBeep == true);

  result = evaluateSaveCommand(10000, 0, true, true, true, false,
      SAVE_HALFWAY_BEEP_TIME_MS, SAVE_BUTTON_HOLD_TIME_MS);
  assert(result.shouldSave == false && result.playNoChangesBeep == true);

  printf("  ✓ Passed\n");
}

void test_i2c_timeout() {
  printf("Testing I2C timeout logic...\n");

  I2CTimeoutResult result = evaluateI2CTimeout(false, MAX_SENSOR_FAILURES - 1,
      false, MAX_SENSOR_FAILURES);
  assert(result.shouldEnterSafeMode == true);

  result = evaluateI2CTimeout(true, 5, true, MAX_SENSOR_FAILURES);
  assert(result.sensorRecovered == true && result.newFailureCount == 0);

  printf("  ✓ Passed\n");
}

void test_settings_change_detection() {
  printf("Testing settings change detection...\n");
  assert(detectSettingsChanges(150, 100, 1, 1) == true);
  assert(detectSettingsChanges(100, 100, -1, 1) == true);
  assert(detectSettingsChanges(100, 100, 1, 1) == false);
  printf("  ✓ Passed\n");
}

void test_eeprom_verification() {
  printf("Testing EEPROM verification...\n");
  assert(verifyEEPROMWrite(100, 100, 1, 1, 0xA5, 0xA5) == true);
  assert(verifyEEPROMWrite(100, 99, 1, 1, 0xA5, 0xA5) == false);
  assert(verifyEEPROMWrite(100, 100, 1, -1, 0xA5, 0xA5) == false);
  printf("  ✓ Passed\n");
}

int main() {
  printf("=== Safety Logic Tests ===\n\n");
  test_save_command();
  test_i2c_timeout();
  test_settings_change_detection();
  test_eeprom_verification();
  printf("\n✅ All safety logic tests passed!\n");
  return 0;
}