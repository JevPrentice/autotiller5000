# Practical Arduino TDD Guide

A hands-on guide to implementing Test-Driven Development for Arduino projects. Focus on what actually works, not academic theory.

## Table of Contents
- [TDD Essentials for Embedded Systems](#tdd-essentials-for-embedded-systems)
- [Testing Frameworks: What to Use](#testing-frameworks-what-to-use)
- [Quick Start Setup](#quick-start-setup)
- [Writing Testable Arduino Code](#writing-testable-arduino-code)
- [Hardware Mocking Strategies](#hardware-mocking-strategies)
- [Practical Examples](#practical-examples)
- [When to Test (and When Not To)](#when-to-test-and-when-not-to)
- [CI/CD Basics](#cicd-basics)

---

## TDD Essentials for Embedded Systems

### The Core Loop
1. **Write a failing test** - Define expected behavior
2. **Write minimal code** - Make it pass
3. **Refactor** - Clean up while tests stay green

### Why TDD Matters for Arduino
- **Faster iteration** - Test on your computer, not by uploading to hardware constantly
- **Catch bugs early** - Before they're burned into flash memory
- **Confident refactoring** - Change code without breaking functionality
- **Hardware abstraction** - Forces you to separate logic from I/O

### Key Principle
> **Separate testable logic from hardware-specific code.** Test the logic, trust the hardware abstraction.

---

## Testing Frameworks: What to Use

### 1. PlatformIO + Unity (Recommended)
**Best for**: All projects, especially beginners

**Pros**:
- Built into PlatformIO
- Runs tests natively on your computer (fast!)
- Runs tests on actual hardware when needed
- Simple C-style API
- Good CI/CD integration

**Cons**:
- Requires PlatformIO setup (not Arduino IDE)

### 2. AUnit
**Best for**: Pure Arduino IDE users, AVR platforms

**Pros**:
- Works with Arduino IDE
- Lightweight (50% less flash than alternatives on AVR)
- Similar to Google Test syntax
- Supports ESP8266, ESP32, SAMD21, STM32, Teensy

**Cons**:
- Tests must run on actual hardware
- Slower feedback loop

### 3. Unity + EpoxyDuino
**Best for**: Advanced users wanting native testing

**Pros**:
- Run Arduino code on Linux/MacOS/FreeBSD
- Fast iteration
- No hardware needed for most tests

**Cons**:
- Steeper learning curve
- Some Arduino features won't work natively

### Recommendation
**Start with PlatformIO + Unity.** It's the most practical path with the best tooling support.

---

## Quick Start Setup

### Install PlatformIO
```bash
# Using pip
pip install platformio

# Or via VS Code extension
# Search "PlatformIO IDE" in VS Code extensions
```

### Create Project
```bash
pio project init --board uno
```

### Project Structure
```
my-arduino-project/
├── platformio.ini          # Configuration
├── src/
│   └── main.cpp           # Your Arduino code
├── lib/
│   └── YourLibrary/       # Custom libraries
└── test/
    ├── test_native/       # Tests that run on computer
    └── test_embedded/     # Tests that run on hardware
```

### Basic platformio.ini
```ini
[env:native]
platform = native
test_framework = unity

[env:uno]
platform = atmelavr
framework = arduino
board = uno
test_framework = unity
```

### Run Tests
```bash
# Test on computer (fast)
pio test -e native

# Test on hardware
pio test -e uno

# Test everywhere
pio test
```

---

## Writing Testable Arduino Code

### The Problem: Untestable Code
```cpp
// BAD: Tightly coupled to hardware
void controlLED() {
    int reading = analogRead(A0);
    if (reading > 512) {
        digitalWrite(LED_BUILTIN, HIGH);
    } else {
        digitalWrite(LED_BUILTIN, LOW);
    }
}
```

### Solution 1: Extract Logic
```cpp
// GOOD: Logic separated from I/O
bool shouldLEDBeOn(int sensorValue) {
    return sensorValue > 512;  // Pure function, easily testable
}

void controlLED() {
    int reading = analogRead(A0);
    bool ledState = shouldLEDBeOn(reading);
    digitalWrite(LED_BUILTIN, ledState ? HIGH : LOW);
}
```

### Solution 2: Dependency Injection
```cpp
// Interface for hardware abstraction
class PinInterface {
public:
    virtual void digitalWrite(uint8_t pin, uint8_t value) = 0;
    virtual int analogRead(uint8_t pin) = 0;
    virtual ~PinInterface() {}
};

// Real hardware implementation
class ArduinoPin : public PinInterface {
public:
    void digitalWrite(uint8_t pin, uint8_t value) override {
        ::digitalWrite(pin, value);
    }

    int analogRead(uint8_t pin) override {
        return ::analogRead(pin);
    }
};

// Controller that accepts any pin implementation
class LEDController {
private:
    PinInterface* pins;
    uint8_t ledPin;
    uint8_t sensorPin;

public:
    LEDController(PinInterface* p, uint8_t led, uint8_t sensor)
        : pins(p), ledPin(led), sensorPin(sensor) {}

    void update() {
        int reading = pins->analogRead(sensorPin);
        uint8_t state = (reading > 512) ? HIGH : LOW;
        pins->digitalWrite(ledPin, state);
    }
};

// Use in production
ArduinoPin realPins;
LEDController controller(&realPins, LED_BUILTIN, A0);
```

### Solution 3: Function Pointers (Lightweight)
```cpp
// Function pointer approach (uses less memory)
typedef void (*DigitalWriteFn)(uint8_t pin, uint8_t value);
typedef int (*AnalogReadFn)(uint8_t pin);

class LEDController {
private:
    DigitalWriteFn writePin;
    AnalogReadFn readPin;

public:
    LEDController(DigitalWriteFn w, AnalogReadFn r)
        : writePin(w), readPin(r) {}

    void update() {
        int reading = readPin(A0);
        writePin(LED_BUILTIN, reading > 512 ? HIGH : LOW);
    }
};

// Production use
LEDController controller(digitalWrite, analogRead);

// Test use
void mockDigitalWrite(uint8_t pin, uint8_t value) { /* record calls */ }
int mockAnalogRead(uint8_t pin) { return 600; }
LEDController testController(mockDigitalWrite, mockAnalogRead);
```

---

## Hardware Mocking Strategies

### Mock Pin I/O
```cpp
// test/test_native/mock_pin.h
class MockPin : public PinInterface {
public:
    uint8_t lastPin = 0;
    uint8_t lastValue = 0;
    int analogValue = 0;

    void digitalWrite(uint8_t pin, uint8_t value) override {
        lastPin = pin;
        lastValue = value;
    }

    int analogRead(uint8_t pin) override {
        lastPin = pin;
        return analogValue;
    }

    // Helper methods for tests
    void setAnalogValue(int val) { analogValue = val; }
    uint8_t getLastValue() { return lastValue; }
};
```

### Mock Serial Communication
```cpp
// test/test_native/mock_serial.h
class MockSerial {
private:
    String buffer;

public:
    void print(const char* str) {
        buffer += str;
    }

    void println(const char* str) {
        buffer += str;
        buffer += "\n";
    }

    String getBuffer() { return buffer; }
    void clear() { buffer = ""; }
};
```

### Mock I2C Sensor
```cpp
// Mock temperature sensor
class MockTempSensor {
private:
    float temperature;

public:
    MockTempSensor(float temp = 20.0) : temperature(temp) {}

    float readTemperature() {
        return temperature;
    }

    void setTemperature(float temp) {
        temperature = temp;
    }
};

// Interface both real and mock implement
class TempSensorInterface {
public:
    virtual float readTemperature() = 0;
    virtual ~TempSensorInterface() {}
};
```

---

## Practical Examples

### Example 1: Testing Pure Logic
```cpp
// lib/TempControl/TempControl.h
class TempControl {
public:
    static bool shouldHeatOn(float currentTemp, float targetTemp) {
        return currentTemp < (targetTemp - 2.0);
    }

    static bool shouldCoolOn(float currentTemp, float targetTemp) {
        return currentTemp > (targetTemp + 2.0);
    }
};

// test/test_native/test_temp_control.cpp
#include <unity.h>
#include <TempControl.h>

void test_heating_turns_on_when_too_cold() {
    TEST_ASSERT_TRUE(TempControl::shouldHeatOn(18.0, 22.0));
}

void test_heating_stays_off_when_warm() {
    TEST_ASSERT_FALSE(TempControl::shouldHeatOn(21.0, 22.0));
}

void test_cooling_turns_on_when_too_hot() {
    TEST_ASSERT_TRUE(TempControl::shouldCoolOn(26.0, 22.0));
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_heating_turns_on_when_too_cold);
    RUN_TEST(test_heating_stays_off_when_warm);
    RUN_TEST(test_cooling_turns_on_when_too_hot);
    UNITY_END();
}

void loop() {}
```

### Example 2: Testing with Mocks
```cpp
// lib/LEDController/LEDController.h
class LEDController {
private:
    PinInterface* pins;
    uint8_t ledPin;

public:
    LEDController(PinInterface* p, uint8_t pin)
        : pins(p), ledPin(pin) {}

    void turnOn() {
        pins->digitalWrite(ledPin, HIGH);
    }

    void turnOff() {
        pins->digitalWrite(ledPin, LOW);
    }
};

// test/test_native/test_led_controller.cpp
#include <unity.h>
#include "mock_pin.h"
#include <LEDController.h>

MockPin mockPin;
LEDController* controller;

void setUp() {
    controller = new LEDController(&mockPin, 13);
}

void tearDown() {
    delete controller;
}

void test_turn_on_sets_pin_high() {
    controller->turnOn();
    TEST_ASSERT_EQUAL(13, mockPin.lastPin);
    TEST_ASSERT_EQUAL(HIGH, mockPin.lastValue);
}

void test_turn_off_sets_pin_low() {
    controller->turnOff();
    TEST_ASSERT_EQUAL(13, mockPin.lastPin);
    TEST_ASSERT_EQUAL(LOW, mockPin.lastValue);
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_turn_on_sets_pin_high);
    RUN_TEST(test_turn_off_sets_pin_low);
    UNITY_END();
}

void loop() {}
```

### Example 3: Testing State Machine
```cpp
// lib/Button/Button.h
class Button {
private:
    PinInterface* pins;
    uint8_t pin;
    bool lastState;

public:
    Button(PinInterface* p, uint8_t buttonPin)
        : pins(p), pin(buttonPin), lastState(false) {}

    bool wasPressed() {
        bool currentState = pins->digitalRead(pin) == HIGH;
        bool pressed = currentState && !lastState;
        lastState = currentState;
        return pressed;
    }
};

// test/test_native/test_button.cpp
#include <unity.h>
#include "mock_pin.h"
#include <Button.h>

MockPin mockPin;
Button* button;

void setUp() {
    button = new Button(&mockPin, 2);
}

void tearDown() {
    delete button;
}

void test_press_detected_on_rising_edge() {
    mockPin.setDigitalValue(LOW);
    button->wasPressed();  // Initialize state

    mockPin.setDigitalValue(HIGH);
    TEST_ASSERT_TRUE(button->wasPressed());
}

void test_no_press_when_held() {
    mockPin.setDigitalValue(HIGH);
    button->wasPressed();
    TEST_ASSERT_FALSE(button->wasPressed());  // Still held
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_press_detected_on_rising_edge);
    RUN_TEST(test_no_press_when_held);
    UNITY_END();
}

void loop() {}
```

---

## When to Test (and When Not To)

### ✅ DO Test
- **Business logic** - Temperature calculations, state machines, algorithms
- **Data parsing** - JSON, CSV, protocol parsing
- **State management** - Mode switching, configuration logic
- **Complex conditions** - Multi-sensor fusion, decision trees
- **Math operations** - PID controllers, filters, conversions
- **String manipulation** - Command parsing, formatting

### ❌ DON'T Test
- **Arduino library calls** - Trust `digitalWrite()`, `analogRead()` work
- **Hardware timing** - `delay()`, `millis()` behavior
- **Physical interactions** - "Does LED actually turn on?"
- **Simple getters/setters** - No logic, no value
- **One-line wrappers** - Unless they contain logic

### When Hardware Testing IS Needed
- **Initial hardware validation** - Verify your hardware abstraction works
- **Timing-critical code** - Interrupt handlers, precise delays
- **Communication protocols** - I2C, SPI, UART edge cases
- **Integration tests** - Full system behavior
- **Power management** - Sleep modes, brownout behavior

### Practical Balance
```
Native Tests (fast):  70% - Business logic, algorithms
Hardware Tests (slow): 20% - Integration, protocol validation
Manual Testing (slow): 10% - Physical verification, edge cases
```

---

## CI/CD Basics

### GitHub Actions Example
```yaml
# .github/workflows/test.yml
name: Arduino Tests

on: [push, pull_request]

jobs:
  test:
    runs-on: ubuntu-latest

    steps:
    - uses: actions/checkout@v3

    - name: Set up Python
      uses: actions/setup-python@v4
      with:
        python-version: '3.x'

    - name: Install PlatformIO
      run: |
        pip install platformio

    - name: Run Tests
      run: |
        pio test -e native
```

### GitLab CI Example
```yaml
# .gitlab-ci.yml
test:
  image: python:3.9

  before_script:
    - pip install platformio

  script:
    - pio test -e native

  only:
    - main
    - merge_requests
```

### Local Pre-commit Hook
```bash
# .git/hooks/pre-commit
#!/bin/sh
pio test -e native
if [ $? -ne 0 ]; then
    echo "Tests failed. Commit aborted."
    exit 1
fi
```

### Simple Makefile
```makefile
# Makefile
test:
	pio test -e native

test-hardware:
	pio test -e uno

upload:
	pio run -t upload

all: test upload
```

---

## Real-World Workflow

### Daily Development
```bash
# 1. Write failing test
vim test/test_native/test_feature.cpp

# 2. Run tests (they should fail)
pio test -e native

# 3. Implement feature
vim lib/Feature/Feature.cpp

# 4. Run tests again (they should pass)
pio test -e native

# 5. Test on hardware occasionally
pio test -e uno

# 6. Deploy
pio run -t upload
```

### TDD Cycle Times
- **Native tests**: 2-5 seconds
- **Hardware tests**: 30-60 seconds
- **Manual testing**: 2-5 minutes

**Target**: Run native tests after every code change. Run hardware tests every 5-10 changes.

---

## Common Pitfalls and Solutions

### Pitfall 1: Testing Too Much
**Problem**: Testing every line, including Arduino library calls
**Solution**: Focus on YOUR logic, not Arduino's

### Pitfall 2: Not Using Dependency Injection
**Problem**: Impossible to mock hardware
**Solution**: Always pass dependencies, don't hard-code them

### Pitfall 3: Slow Test Feedback
**Problem**: Uploading to hardware for every test
**Solution**: Use native tests for logic, hardware only for integration

### Pitfall 4: Ignoring Hardware Tests Completely
**Problem**: Code works on computer but fails on Arduino
**Solution**: Run hardware tests before major releases

### Pitfall 5: Over-Engineering Abstractions
**Problem**: Complex class hierarchies for simple LED blinking
**Solution**: Start simple, add abstraction only when needed

---

## Resources

### Libraries
- [PlatformIO](https://platformio.org/) - Testing platform
- [Unity](https://github.com/ThrowTheSwitch/Unity) - C testing framework
- [AUnit](https://github.com/bxparks/AUnit) - Arduino unit testing
- [EpoxyDuino](https://github.com/bxparks/EpoxyDuino) - Run Arduino on desktop

### Documentation
- [PlatformIO Unit Testing Docs](https://docs.platformio.org/en/latest/advanced/unit-testing/index.html)
- [Unity Assertions Reference](https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityAssertionsReference.md)

### Examples
- [PlatformIO Test Examples](https://github.com/platformio/platformio-examples)
- [Arduino Abstractions](https://github.com/mum4k/arduino_abstractions)

---

## Quick Reference

### Unity Assertions
```cpp
// Equality
TEST_ASSERT_EQUAL(expected, actual)
TEST_ASSERT_EQUAL_INT(expected, actual)
TEST_ASSERT_EQUAL_FLOAT(expected, actual)

// Boolean
TEST_ASSERT_TRUE(condition)
TEST_ASSERT_FALSE(condition)

// Null checks
TEST_ASSERT_NULL(pointer)
TEST_ASSERT_NOT_NULL(pointer)

// Ranges
TEST_ASSERT_GREATER_THAN(threshold, actual)
TEST_ASSERT_LESS_THAN(threshold, actual)

// Strings
TEST_ASSERT_EQUAL_STRING(expected, actual)

// Arrays
TEST_ASSERT_EQUAL_INT_ARRAY(expected, actual, count)
```

### Test Structure Template
```cpp
#include <unity.h>
#include <YourCode.h>

void setUp() {
    // Runs before each test
}

void tearDown() {
    // Runs after each test
}

void test_function_name() {
    // Arrange
    int input = 5;

    // Act
    int result = yourFunction(input);

    // Assert
    TEST_ASSERT_EQUAL(10, result);
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_function_name);
    UNITY_END();
}

void loop() {}
```

---

## Conclusion

TDD for Arduino is practical and worthwhile when you:
1. **Separate logic from hardware** using dependency injection
2. **Test business logic natively** for fast feedback
3. **Use hardware tests sparingly** for integration validation
4. **Automate with CI/CD** to catch regressions early

Start small: Pick one module, write tests for its logic, and grow from there. The investment pays off when you refactor confidently or debug issues without constantly uploading to hardware.

**Remember**: The goal isn't 100% coverage. It's building reliable systems faster.