#include <iostream>
#include <cmath>
#include "../src/servo_control.h"

int main() {
  ServoUpdateResult result = updateServoPosition(90.0, 180.0, 100, 1);
  std::cout << "Position: " << result.newPosition << std::endl;
  std::cout << "Adjustment: " << result.adjustmentApplied << std::endl;
  std::cout << "sin(180deg): " << sin(3.14159265359) << std::endl;
  return 0;
}
