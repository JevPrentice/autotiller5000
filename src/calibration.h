// Copyright 2025 Wentzel
#ifndef CALIBRATION_H_
#define CALIBRATION_H_

#include <Adafruit_BNO055.h>
#include <EEPROM.h>

#define CALIB_MAGIC 0xB055

struct CalibBlob {
  uint16_t magic;
  adafruit_bno055_offsets_t offsets;
};

static const int CALIB_EEPROM_ADDR = 0;

bool loadCalibFromEEPROM(adafruit_bno055_offsets_t &out) {
  CalibBlob blob;
  EEPROM.get(CALIB_EEPROM_ADDR, blob);
  if (blob.magic != CALIB_MAGIC) return false;
  out = blob.offsets;
  return true;
}

bool saveCalibToEEPROM(Adafruit_BNO055& bno) {
  if (!bno.isFullyCalibrated()) return false;
  adafruit_bno055_offsets_t ofs;
  bno.getSensorOffsets(ofs);
  CalibBlob blob;
  blob.magic = CALIB_MAGIC;
  blob.offsets = ofs;
  EEPROM.put(CALIB_EEPROM_ADDR, blob);
  return true;
}

void applyCalibToBNO(Adafruit_BNO055& bno,
                     const adafruit_bno055_offsets_t &ofs,
                     int delay_ms) {
  bno.setSensorOffsets(ofs);
  delay(delay_ms);
}

#endif  // CALIBRATION_H_