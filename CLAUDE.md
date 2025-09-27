# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

---

**👤 For Human Users:** See **[README.md](./README.md)** - Complete user manual with operational guide, hardware setup, troubleshooting, and safety information.

**🤖 For AI Developers:** Continue reading below for development guidance, code conventions, and technical architecture.

---

## Project Overview

AutoTiller5000 is an Arduino-based marine autopilot system for a Miura sailboat. The system maintains a compass heading using a BNO055 IMU sensor and servo motor control. **Safety and reliability are paramount** - this is a safety-critical marine system that must operate reliably in harsh saltwater environments with minimal power consumption.

## Hardware Configuration

**Critical Pin Assignments:**
- Servo control: Pin 9 (⚠️ Buzzer interferes with pins 3 and 11 - avoid these!)
- Main button: Pin 2 (INPUT_PULLUP)
- Port button: Pin 4 (INPUT_PULLUP)
- Starboard button: Pin 5 (INPUT_PULLUP)
- Buzzer: Pin 8
- Status LEDs: Pins 10, 11, 12
- I2C (BNO055): SDA→A4, SCL→A5

**Sensor:**
- BNO055 IMU at I2C address 0x28
- Sample rate: 100ms (BNO055_SAMPLERATE_DELAY_MS)

**Libraries Required:**
- Adafruit BNO055
- Adafruit Unified Sensor
- Servo (Arduino/Michael Margolis)
- Wire, EEPROM (built-in)

## System Architecture

### Control Flow

The autotiller uses a **proportional sine-based control algorithm**:

```
servo_position = servo_position + (sensitivity/10) × sin(heading_adjustment × π/180) × reverse
```

- `heading_adjustment` is normalized to [-180, +180] degrees
- `sensitivity` ranges from 30-1000 (stored in EEPROM at address 251)
- `reverse` is ±1 (stored in EEPROM at address 250) to reverse servo direction

### Operational States

1. **Standby Mode**: Display heading, show calibration status (LED indicators)
2. **Course Holding Mode**: Engaged when main button pressed, continuously adjusts servo to maintain course
3. **Calibration Mode**: Auto-saves BNO055 calibration to EEPROM when fully calibrated

### Button Functions

**Main Button (Pin 2):**
- Short press: Set/lock current heading as course
- While in course mode, hold:
  - <2s: Exit course mode
  - 2-4s: Enter sensitivity adjustment mode
  - >4s: Toggle servo reverse direction

**Port/Starboard Buttons (Pins 4/5):**
- Short press (<3s): Adjust course by ±2° (amountcourseadj)
- Long press (>3s): Tack by ±90° (tackangle) with 10-beep warning
  - Press main button during warning to cancel tack

**In Sensitivity Mode:**
- Port button: Decrease sensitivity by 10 (min 30)
- Starboard button: Increase sensitivity by 10 (max 1000)
- Buzzer frequency indicates current sensitivity level

### EEPROM Usage

- **Addresses 0-253**: BNO055 calibration data (CalibBlob struct with magic number 0xB055)
- **Address 250**: Servo reverse flag (int8_t, ±1)
- **Address 251**: Sensitivity value (int, 30-1000)
- **Address 252**: Settings magic number (0xA5) for validation

Auto-saves calibration once per boot when sensor is fully calibrated.

### LED Status Indicators

- **LED1 (Pin 10)**: Lit when system active
- **LED2 (Pin 11)**: Lit during course setting
- **LED3 (Pin 12)**: Lit during sensor diagnostics
- **Calibration Display**: LEDs 1-3 indicate BNO055 system calibration level (0-3)

## Development Guidelines

### Context Management

**⚠️ CRITICAL: Always keep `docs/ai/context/active/CURRENT_CONTEXT.md` updated:**
- Update session progress after completing each significant task
- Record all code changes, extractions, and architectural updates
- Update codebase metrics when files are added/modified/deleted
- Keep "Next Steps" and "Known Issues" sections current
- Archive completed work to `docs/ai/context/archive/` when sessions conclude

This context file is the source of truth for ongoing development - maintaining it ensures continuity across sessions.

### Building and Uploading

This is an Arduino .ino file - use Arduino CLI:

```bash
# Compile
arduino-cli compile --fqbn arduino:avr:uno autotiller5000.ino

# Upload
arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:uno autotiller5000.ino

# Lint
cpplint --extensions=ino *.ino
```

**Build & Lint Commands:**
- Compile: `arduino-cli compile --fqbn arduino:avr:uno autotiller5000.ino`
- Lint: `cpplint --extensions=ino *.ino`

**⚠️ MANDATORY: All code changes MUST pass build and lint checks before completion:**
1. After ANY code modification to .ino files, ALWAYS run: `arduino-cli compile --fqbn arduino:avr:uno autotiller5000.ino`
2. After ANY code modification to .ino files, ALWAYS run: `cpplint --extensions=ino *.ino`
3. Fix ALL compilation errors and lint warnings before considering the task complete
4. If build or lint fails, the code change is INCOMPLETE and must be fixed

### Serial Monitor

Serial output at 115200 baud shows:
- Heading, Course, Heading Adjustment, Servo Position
- Calibration status (Sys, Gyro, Accel, Mag values 0-3)
- Sensor diagnostics

### Critical Safety Considerations

**When modifying code:**
1. **Never block the main loop** - delays must allow button checking
2. **Always normalize course to 0-360°** after adjustments
3. **Validate sensor readings** before using them for control
4. **Test servo limits** (0-180°) to prevent mechanical damage
5. **Implement fail-safes** for sensor failures (see docs/research/arduino-reliability.md)
6. **Consider power consumption** - this runs on boat battery

**Hardware Reliability:**
- Use watchdog timer for production deployment
- Implement brown-out detection for low voltage conditions
- Add timeout protection for I2C communication failures
- Consider redundancy for safety-critical functions

### Code Principles

**Simplicity First:**
- KEEP the code as simple as possible
- Avoid over-engineering - this is embedded code with limited resources
- Prefer straightforward solutions over clever abstractions
- If a simple approach works, use it

**Clean Code & DRY:**
- Follow clean code principles - readable, maintainable, testable
- DRY (Don't Repeat Yourself) - extract duplicated logic into functions
- Functions should do one thing well
- Clear, descriptive naming over comments

**Strong Typing & Struct Initialization:**
- **ALWAYS** use named field initialization for structs (C++20 designated initializers)
- This prevents parameter order mistakes and makes code self-documenting
- Example: Use `{.course = 0.0f, .heading = 0.0f}` instead of `{0, 0}`
- Apply to all struct initializations in both production and test code

**Arduino Constraints:**
- Minimize memory usage (limited SRAM on Arduino Uno)
- Avoid dynamic allocation (new/delete)
- Keep stack usage minimal
- Consider flash memory limits for code

### Code Conventions

**Architecture:**
- Main .ino file contains PURE orchestration - **152 lines** (ZERO logic!)
  - Global variable declarations and includes
  - setup() function - hardware initialization calls
  - loop() function - sensor reading and control orchestration
- Logic extracted to four types of headers (22 total files):
  1. **Configuration Headers** (struct definitions, 2 files):
     - config.h - 8 configuration structs (HardwarePins, TimingConfig, AudioConfig, CourseAdjustmentConfig, NavigationState, SensorSafetyConfig, SensorConfig, SensitivityConfig)
     - constants.h - pin assignments and configuration constants
  2. **Pure Logic Headers** (testable, no side effects, 11 files):
     - navigation.h, safety_logic.h, button_logic.h, tack_logic.h
     - led_patterns.h, course_adjustment.h, state_machine.h
     - servo_control.h, sensitivity_adjustment.h, button_debounce.h, watchdog_safety.h
  3. **Workflow Headers** (hardware integration, struct-based params, 8 files):
     - calibration.h, settings_persistence.h, sensor_safety.h
     - button_workflows.h, state_workflows.h, save_workflow.h
     - setup_workflows.h, loop_workflows.h
  4. **Diagnostics Header** (debug output, 1 file):
     - diagnostics.h

**Key Functions by Header:**
- **config.h:** Configuration structs (HardwarePins, TimingConfig, AudioConfig, CourseAdjustmentConfig, NavigationState, SensorSafetyConfig, SensorConfig, SensitivityConfig)
- **constants.h:** Pin assignments and configuration constants
- **navigation.h:** `calculateHeadingAdjustment()`, `normalizeCourse()`, `calculateServoAdjustment()`, `constrainServoPosition()`, `clampSensitivity()`
- **safety_logic.h:** `evaluateSaveCommand()`, `evaluateI2CTimeout()`, `detectSettingsChanges()`, `verifyEEPROMWrite()`
- **button_logic.h:** `evaluateButtonPress()` - Button timing logic
- **button_debounce.h:** `debouncedDigitalRead()` - Hardware button debouncing
- **watchdog_safety.h:** Watchdog timer safety functions and risk assessment
- **tack_logic.h:** `evaluateTackWarning()` - Tack cancellation logic
- **led_patterns.h:** `calculateButtonHoldLeds()` - LED patterns
- **course_adjustment.h:** `applyCourseAdjustment()` - Course normalization
- **state_machine.h:** `determineButtonHoldAction()` - Button hold states
- **servo_control.h:** `updateServoPosition()` - Servo constraint logic
- **sensitivity_adjustment.h:** `adjustSensitivity()` - Sensitivity adjustment
- **calibration.h:** `saveCalibToEEPROM()`, `loadCalibFromEEPROM()`, `applyCalibToBNO()`
- **settings_persistence.h:** `loadUserSettings()`, `saveUserSettings()`
- **sensor_safety.h:** `getBNO055EventWithTimeout()`, `enterSensorSafeMode()`
- **button_workflows.h:** `portadj()`, `staradj()`, `checkbuttonpressed()`, `buzz()`
- **state_workflows.h:** `altercourse()`, `checktime()`, `setsensitive()`, `LEDOFF()`
- **save_workflow.h:** `checkSaveCommand()`

**⚠️ Code Quality Standards:**
- ALL code must be written to pass `cpplint --extensions=ino *.ino` on first attempt
- Follow Google C++ Style Guide (cpplint enforces this)
- Write clean, lint-compliant code from the start - don't write code expecting to fix lint errors later
- If cpplint reports issues, the code does not meet project standards
- **Parameter passing:** Functions should have ≤4-6 parameters; use structs from config.h for complex signatures
- **Build metrics:** Flash: 19,950 bytes (61%), SRAM: 552 bytes (26%) - excellent SRAM optimization

### Testing Approach

**Unit Tests** (no hardware required):
- Pure functions tested in separate files per module
- Uses simple assert-based testing (no external frameworks)
- Tests verify current behavior (including known bugs)

**Running Tests:**
```bash
cd test
make           # Compile and run all tests
make clean     # Remove test executables
```

**Test Files (14 files, 98 tests, 1400 lines):**
- `test/test_navigation.cpp` - Navigation functions (5 tests)
- `test/test_safety_logic.cpp` - Safety functions (4 tests)
- `test/test_button_logic.cpp` - Button timing logic (3 tests)
- `test/test_button_debounce.cpp` - Button debouncing logic (4 tests)
- `test/test_led_patterns.cpp` - LED pattern logic (6 tests)
- `test/test_course_adjustment.cpp` - Course adjustment logic (12 tests, includes bug isolation)
- `test/test_state_machine.cpp` - State machine logic (11 tests)
- `test/test_tack_logic.cpp` - Tack warning and cancellation (3 tests)
- `test/test_servo_control.cpp` - Servo control integration (14 tests)
- `test/test_servo_debug.cpp` - Servo debugging utilities
- `test/test_sensitivity_adjustment.cpp` - Sensitivity adjustment logic (13 tests)
- `test/test_settings_persistence.cpp` - Settings change detection (10 tests)
- `test/test_watchdog_safety.cpp` - Watchdog timer safety logic
- `test/test_sensor_backoff.cpp` - Sensor failure backoff logic (5 tests)

**Test Output:**
- Each test prints "PASS" or "FAIL" with test name
- All tests must pass before deployment
- Tests compile with: `g++ -Wall -std=c++11 -lm`

Refer to `docs/research/arduino-tdd.md` for mocking strategies and testing patterns.

## Reference Documentation

Comprehensive research documentation available in `/docs/research`. See `docs/README.md` for the complete documentation index with navigation tips and AI guidance.

### Arduino Development
- `research/arduino-programming.md`: Complete Arduino language reference (setup/loop, I/O, memory, interrupts)
- `research/arduino-tdd.md`: Practical TDD guide (testing frameworks, hardware mocking, CI/CD)
- `research/arduino-reliability.md`: Embedded systems reliability (watchdogs, fail-safe, power management)

### Marine Autopilot Systems
- `research/autotillers.md`: Marine autopilot overview (PID control, sensors, commercial systems, DIY projects)
- `research/seek-and-hunt.md`: Hunting behavior analysis and solutions (control tuning, anti-hunting measures)
- `research/miura-sailboat.md`: Target vessel specifications

### Sailing Knowledge
- `research/nautical-terms.md`: Comprehensive sailing terminology (directions, navigation, rigging, racing, commands)
- `research/sailing-mathematics.md`: Navigation calculations (heading/course, wind angles, VMG, compass math)
- `research/sailing-safety.md`: Safety procedures and best practices (emergencies, COLREGs, equipment)

### Code Reviews
- `code_reviews/code-review-2025-09-23.md`: Initial comprehensive review - 19 issues identified (7 critical)

**Navigation Tip**: When working on control algorithms, cross-reference `research/autotillers.md` for PID theory, `research/seek-and-hunt.md` for anti-hunting techniques, and `research/sailing-mathematics.md` for heading calculations.
