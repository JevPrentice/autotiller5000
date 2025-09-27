// Copyright 2025 Wentzel
#include <cassert>
#include <iostream>
#include <stdint.h>

// Mock UserSettings struct (from settings_persistence.h)
struct UserSettings {
  int sensitivity;
  int8_t reverse;
};

// Pure function from settings_persistence.h
bool settingsHaveChanged(const UserSettings& current,
                         const UserSettings& lastSaved) {
  return (current.sensitivity != lastSaved.sensitivity ||
          current.reverse != lastSaved.reverse);
}

void testNoChanges() {
  UserSettings current = {.sensitivity = 100, .reverse = 1};
  UserSettings lastSaved = {.sensitivity = 100, .reverse = 1};
  assert(settingsHaveChanged(current, lastSaved) == false);
  std::cout << "Testing settingsHaveChanged (no changes)... PASS\n";
}

void testSensitivityChanged() {
  UserSettings current = {.sensitivity = 120, .reverse = 1};
  UserSettings lastSaved = {.sensitivity = 100, .reverse = 1};
  assert(settingsHaveChanged(current, lastSaved) == true);
  std::cout << "Testing settingsHaveChanged (sensitivity changed)... PASS\n";
}

void testReverseChanged() {
  UserSettings current = {.sensitivity = 100, .reverse = -1};
  UserSettings lastSaved = {.sensitivity = 100, .reverse = 1};
  assert(settingsHaveChanged(current, lastSaved) == true);
  std::cout << "Testing settingsHaveChanged (reverse changed)... PASS\n";
}

void testBothChanged() {
  UserSettings current = {.sensitivity = 150, .reverse = -1};
  UserSettings lastSaved = {.sensitivity = 100, .reverse = 1};
  assert(settingsHaveChanged(current, lastSaved) == true);
  std::cout << "Testing settingsHaveChanged (both changed)... PASS\n";
}

void testEdgeCaseMinSensitivity() {
  UserSettings current = {.sensitivity = 30, .reverse = 1};
  UserSettings lastSaved = {.sensitivity = 30, .reverse = 1};
  assert(settingsHaveChanged(current, lastSaved) == false);
  std::cout << "Testing settingsHaveChanged (min sensitivity, no change)... PASS\n";
}

void testEdgeCaseMaxSensitivity() {
  UserSettings current = {.sensitivity = 1000, .reverse = 1};
  UserSettings lastSaved = {.sensitivity = 1000, .reverse = 1};
  assert(settingsHaveChanged(current, lastSaved) == false);
  std::cout << "Testing settingsHaveChanged (max sensitivity, no change)... PASS\n";
}

void testEdgeCaseReverseToggle() {
  UserSettings current = {.sensitivity = 100, .reverse = 1};
  UserSettings lastSaved = {.sensitivity = 100, .reverse = -1};
  assert(settingsHaveChanged(current, lastSaved) == true);
  std::cout << "Testing settingsHaveChanged (reverse toggle only)... PASS\n";
}

void testEdgeCaseSmallSensitivityDifference() {
  UserSettings current = {.sensitivity = 101, .reverse = 1};
  UserSettings lastSaved = {.sensitivity = 100, .reverse = 1};
  assert(settingsHaveChanged(current, lastSaved) == true);
  std::cout << "Testing settingsHaveChanged (small sensitivity diff)... PASS\n";
}

void testEdgeCaseNegativeToPositiveReverse() {
  UserSettings current = {.sensitivity = 100, .reverse = 1};
  UserSettings lastSaved = {.sensitivity = 100, .reverse = -1};
  assert(settingsHaveChanged(current, lastSaved) == true);
  std::cout << "Testing settingsHaveChanged (negative to positive reverse)... PASS\n";
}

void testEdgeCaseZeroSensitivity() {
  // Edge case: what if sensitivity is 0 (invalid but test the logic)
  UserSettings current = {.sensitivity = 0, .reverse = 1};
  UserSettings lastSaved = {.sensitivity = 0, .reverse = 1};
  assert(settingsHaveChanged(current, lastSaved) == false);
  std::cout << "Testing settingsHaveChanged (zero sensitivity, no change)... PASS\n";
}

int main() {
  testNoChanges();
  testSensitivityChanged();
  testReverseChanged();
  testBothChanged();
  testEdgeCaseMinSensitivity();
  testEdgeCaseMaxSensitivity();
  testEdgeCaseReverseToggle();
  testEdgeCaseSmallSensitivityDifference();
  testEdgeCaseNegativeToPositiveReverse();
  testEdgeCaseZeroSensitivity();
  std::cout << "All settings_persistence tests passed!\n";
  return 0;
}