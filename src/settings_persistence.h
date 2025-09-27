// Copyright 2025 Wentzel
#ifndef SETTINGS_PERSISTENCE_H_
#define SETTINGS_PERSISTENCE_H_

#include <EEPROM.h>
#include <stdint.h>

const int EEPROM_BASE_ADDR = 250;
const int EEPROM_SETTINGS_MAGIC_ADDR = 252;
const byte EEPROM_SETTINGS_MAGIC = 0xA5;

struct UserSettings {
  int sensitivity;
  int8_t reverse;
};

void loadUserSettings(UserSettings& settings,
                      UserSettings& lastSaved,
                      int defaultSensitivity,
                      int8_t defaultReverse) {
  byte magic = EEPROM.read(EEPROM_SETTINGS_MAGIC_ADDR);
  if (magic == EEPROM_SETTINGS_MAGIC) {
    settings.sensitivity = EEPROM.read(EEPROM_BASE_ADDR + 1);
    settings.reverse = (int8_t)EEPROM.read(EEPROM_BASE_ADDR);
    lastSaved = settings;
    Serial.println(F("User settings loaded from EEPROM"));
    Serial.print(F("  Sensitivity: "));
    Serial.println(settings.sensitivity);
    Serial.print(F("  Reverse: "));
    Serial.println(settings.reverse);
  } else {
    settings.sensitivity = defaultSensitivity;
    settings.reverse = defaultReverse;
    lastSaved = settings;
    Serial.println(F("No saved settings found - using defaults"));
    Serial.print(F("  Sensitivity: "));
    Serial.println(defaultSensitivity);
    Serial.print(F("  Reverse: "));
    Serial.println(defaultReverse);
  }
}

bool saveUserSettings(UserSettings& settings, UserSettings& lastSaved) {
  Serial.println(F("Saving user settings to EEPROM..."));

  EEPROM.write(EEPROM_BASE_ADDR, settings.reverse);
  EEPROM.write(EEPROM_BASE_ADDR + 1, settings.sensitivity);
  EEPROM.write(EEPROM_SETTINGS_MAGIC_ADDR, EEPROM_SETTINGS_MAGIC);

  int8_t verifyReverse = (int8_t)EEPROM.read(EEPROM_BASE_ADDR);
  int verifySensitivity = EEPROM.read(EEPROM_BASE_ADDR + 1);
  byte verifyMagic = EEPROM.read(EEPROM_SETTINGS_MAGIC_ADDR);

  if (verifyReverse == settings.reverse &&
      verifySensitivity == settings.sensitivity &&
      verifyMagic == EEPROM_SETTINGS_MAGIC) {
    lastSaved = settings;
    Serial.println(F("Settings saved successfully"));
    Serial.print(F("  Sensitivity: "));
    Serial.println(settings.sensitivity);
    Serial.print(F("  Reverse: "));
    Serial.println(settings.reverse);
    return true;
  } else {
    Serial.println(F("ERROR: EEPROM write verification failed!"));
    return false;
  }
}

bool settingsHaveChanged(const UserSettings& current,
                         const UserSettings& lastSaved) {
  return (current.sensitivity != lastSaved.sensitivity ||
          current.reverse != lastSaved.reverse);
}

#endif  // SETTINGS_PERSISTENCE_H_