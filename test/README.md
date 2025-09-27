# AutoTiller5000 Test Suite

Unit tests for AutoTiller5000 navigation and control logic. All tests run on development machine (no Arduino hardware required).

## Quick Start

```bash
cd test
make           # Compile and run all tests
make clean     # Remove test executables
```

## Test Coverage

### Navigation Tests (`test_navigation.cpp`)
- **Heading Adjustment**: Validates ±180° normalization (course - heading)
- **Course Normalization**: Ensures course stays within 0-360°
- **Servo Adjustment**: Tests proportional sine control formula
- **Servo Constraints**: Validates 0-180° servo limits
- **Sensitivity Clamping**: Tests min/max sensitivity bounds (30-1000)

### Safety Logic Tests (`test_safety_logic.cpp`)
- **Save Command Detection**: 10-second button hold validation
- **I2C Timeout Detection**: Sensor failure threshold logic
- **Settings Change Detection**: Tracks sensitivity/reverse modifications
- **EEPROM Write Verification**: Validates write-then-read verification

### Button Logic Tests (`test_button_logic.cpp`)
- **Short Press Detection**: <3s press → ±2° course adjustment
- **Long Press (Tack)**: ≥3s press → ±90° tack
- **Port/Starboard Logic**: Validates correct direction for each button

### LED Patterns Tests (`test_led_patterns.cpp`)
- **Button Hold Feedback**: LED patterns at 2s/4s/6s thresholds
- **State Indication**: LED count matches hold duration category

### Course Adjustment Tests (`test_course_adjustment.cpp`)
- **Small Adjustments**: ±2° course changes with normalization
- **Tack Maneuvers**: ±90° course changes with normalization
- **Boundary Conditions**: Tests wraparound at 0°/360°
- **⚠️ Known Bug Tests**: Documents and isolates course normalization bug at boundaries

### State Machine Tests (`test_state_machine.cpp`)
- **Exit Action**: 0-2s hold → exit course mode
- **Sensitivity Adjust**: 2-4s hold → enter sensitivity adjustment
- **Reverse Toggle**: 4s+ hold → toggle servo reverse

### Tack Logic Tests (`test_tack_logic.cpp`)
- **Tack Warning**: Validates warning loop behavior
- **Tack Cancellation**: Tests cancel-via-button logic
- **Iteration Counter**: Ensures proper loop termination

### Sensitivity Adjustment Tests (`test_sensitivity_adjustment.cpp`)
- **Button Input**: Port/starboard button logic for sensitivity changes
- **Boundary Clamping**: Tests min (30) and max (1000) sensitivity limits
- **Step Size**: Validates sensitivity increment/decrement by step value
- **Buzzer Frequency**: Verifies buzzer frequency matches sensitivity level

### Servo Control Tests (`test_servo_control.cpp`)
- **Servo Updates**: Integration of heading adjustment, sensitivity, and reverse
- **Constraint Logic**: Tests 0-180° servo position limits
- **Edge Cases**: Boundary conditions, high/low sensitivity, reverse direction

### Settings Persistence Tests (`test_settings_persistence.cpp`)
- **Change Detection**: Validates settingsHaveChanged() logic
- **Sensitivity Comparison**: Tests sensitivity value differences
- **Reverse Comparison**: Tests reverse flag differences
- **No Changes**: Verifies identical settings return false
- **Edge Cases**: Min/max sensitivity, small differences, toggle scenarios

## Test Statistics

- **Total Test Files**: 11
- **Total Tests**: 81
- **Total Lines**: 856 lines
- **Coverage**: 100% of testable pure logic functions

## Test Architecture

### Design Principles
- **Pure Functions**: All tested functions have no side effects
- **No Hardware Mocking**: Tests run directly on extracted logic
- **Assert-Based**: Simple `assert()` statements for pass/fail
- **Minimal Dependencies**: Standard C++ only (`<cassert>`, `<cmath>`, `<iostream>`)

### Header File Structure
```
autotiller5000/src/
├── navigation.h              # Tested by test_navigation.cpp
├── safety_logic.h            # Tested by test_safety_logic.cpp
├── button_logic.h            # Tested by test_button_logic.cpp
├── led_patterns.h            # Tested by test_led_patterns.cpp
├── course_adjustment.h       # Tested by test_course_adjustment.cpp
├── state_machine.h           # Tested by test_state_machine.cpp
├── tack_logic.h              # Tested by test_tack_logic.cpp
├── sensitivity_adjustment.h  # Tested by test_sensitivity_adjustment.cpp
├── servo_control.h           # Tested by test_servo_control.cpp
└── settings_persistence.h    # Tested by test_settings_persistence.cpp
```

### Test Output Format
```
Testing calculateHeadingAdjustment... PASS
Testing normalizeCourse... PASS
Testing calculateServoAdjustment... PASS
All tests passed!
```

## Adding New Tests

1. **Extract pure function** to appropriate `.h` file:
   ```cpp
   // Example: new_feature.h
   float calculateNewFeature(float input) {
     return input * 2.0;
   }
   ```

2. **Create test file** (`test_new_feature.cpp`):
   ```cpp
   #include <cassert>
   #include <iostream>
   #include "../new_feature.h"

   void testNewFeature() {
     assert(calculateNewFeature(5.0) == 10.0);
     std::cout << "Testing calculateNewFeature... PASS\n";
   }

   int main() {
     testNewFeature();
     std::cout << "All tests passed!\n";
     return 0;
   }
   ```

3. **Add to Makefile**:
   ```makefile
   TESTS = ... test_new_feature

   test_new_feature: test_new_feature.cpp
       $(CXX) $(CXXFLAGS) -o test_new_feature test_new_feature.cpp $(LDFLAGS)
   ```

4. **Run tests**: `cd test && make`

## CI/CD Integration

Tests run with:
- **Compiler**: `g++`
- **Flags**: `-Wall -std=c++11`
- **Math Library**: `-lm`

All tests must pass before:
- Merging pull requests
- Deploying to hardware
- Releasing new versions

## Known Issues

### Course Normalization Bug (Documented in `test_course_adjustment.cpp`)
- **Issue**: Course boundary wraparound may fail in edge cases
- **Status**: Bug isolated and documented in test suite
- **Impact**: Rare occurrence at exact 0°/360° boundaries
- **Tracking**: See `test_course_adjustment.cpp` comments for details

## References

- **Testing Strategy**: See `docs/research/arduino-tdd.md`
- **Development Guide**: See `CLAUDE.md` → Testing Approach
- **Code Review**: See `docs/code_reviews/code-review-2025-09-23.md`