# Arduino Programming Reference Guide

## Table of Contents
1. [Introduction](#introduction)
2. [Language Foundation](#language-foundation)
3. [Program Structure](#program-structure)
4. [Data Types](#data-types)
5. [Variables and Constants](#variables-and-constants)
6. [Operators](#operators)
7. [Control Structures](#control-structures)
8. [Functions](#functions)
9. [Pin Modes and I/O](#pin-modes-and-io)
10. [Timing Functions](#timing-functions)
11. [Serial Communication](#serial-communication)
12. [Interrupts](#interrupts)
13. [Memory Management](#memory-management)
14. [Libraries](#libraries)
15. [Common Gotchas and Best Practices](#common-gotchas-and-best-practices)
16. [Differences from Standard C/C++](#differences-from-standard-cc)

---

## Introduction

Arduino programming is based on the Wiring framework, which is itself based on C/C++. The Arduino language provides a simplified interface to microcontroller programming while maintaining the power and flexibility of C/C++.

### What is Arduino?

Arduino sketches are written in a subset of C/C++ with additional functions and libraries specific to Arduino hardware. The Arduino IDE handles compilation using the AVR-GCC compiler (for AVR-based boards) and provides an accessible environment for embedded programming.

### Key Characteristics

- **Simplified Setup**: Automatic handling of many low-level details
- **Cross-Platform**: Works on Windows, macOS, and Linux
- **Open Source**: Both hardware and software are open source
- **Large Community**: Extensive libraries and community support

---

## Language Foundation

Arduino programming language can be divided into three main parts:

1. **Functions**: Procedures that perform specific tasks
2. **Values**: Variables and constants that store data
3. **Structure**: Control flow and program organization

### Based on C/C++

Arduino uses:
- C/C++ syntax and structure
- AVR-GCC compiler for AVR boards
- AVR-libc (not the full C++ Standard Library)
- Custom Arduino libraries and functions

---

## Program Structure

Every Arduino sketch must contain two essential functions:

### setup()

Runs once when the program starts. Use it to initialize variables, pin modes, libraries, etc.

```cpp
void setup() {
  // Initialization code here
  pinMode(13, OUTPUT);      // Set pin 13 as output
  Serial.begin(9600);       // Start serial communication
}
```

### loop()

Runs continuously after `setup()` completes. Contains the main program logic.

```cpp
void loop() {
  // Main code here - runs repeatedly
  digitalWrite(13, HIGH);   // Turn LED on
  delay(1000);              // Wait 1 second
  digitalWrite(13, LOW);    // Turn LED off
  delay(1000);              // Wait 1 second
}
```

### Basic Sketch Structure

```cpp
// Global variables and constants
const int ledPin = 13;
int sensorValue = 0;

void setup() {
  // Runs once at startup
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Runs continuously
  sensorValue = analogRead(A0);
  Serial.println(sensorValue);
  delay(100);
}
```

---

## Data Types

Arduino supports various data types with specific memory sizes and ranges.

### Integer Types

| Type | Size | Range | Use Case |
|------|------|-------|----------|
| `byte` | 8 bits | 0 to 255 | Unsigned values, efficient storage |
| `int` | 16 bits | -32,768 to 32,767 | General purpose integers |
| `unsigned int` | 16 bits | 0 to 65,535 | Positive values only |
| `long` | 32 bits | -2,147,483,648 to 2,147,483,647 | Large integers |
| `unsigned long` | 32 bits | 0 to 4,294,967,295 | millis() return value |

### Floating Point Types

| Type | Size | Range | Precision |
|------|------|-------|-----------|
| `float` | 32 bits | -3.4028235E38 to 3.4028235E38 | ~6-7 decimal digits |
| `double` | 32 bits | Same as float | Same as float (on most Arduino boards) |

**Note**: Floating point operations are not native to most Arduino boards and should be avoided when possible for performance reasons.

### Character Types

| Type | Size | Range |
|------|------|-------|
| `char` | 8 bits | -128 to 127 |
| `unsigned char` | 8 bits | 0 to 255 (same as byte) |

### Boolean Type

```cpp
bool ledState = true;   // or false
boolean buttonPressed = HIGH;  // Arduino-specific, same as bool
```

### String Types

#### C-Style String (Character Array)
```cpp
char message[] = "Hello";        // Null-terminated array
char buffer[20];                 // Fixed-size buffer
```

#### Arduino String Class
```cpp
String text = "Hello World";    // Dynamic String object
text.concat(" Arduino");         // String manipulation
int length = text.length();      // Get length
```

**Warning**: The String class uses dynamic memory allocation, which can cause heap fragmentation on memory-constrained boards.

### Arrays

```cpp
int readings[10];                        // Array of 10 integers
int values[] = {1, 2, 3, 4, 5};         // Initialized array
char message[] = "Hello";                // Character array
```

---

## Variables and Constants

### Variable Declaration

```cpp
int sensorPin = A0;              // Variable declaration and initialization
int sensorValue;                 // Declaration only
sensorValue = analogRead(sensorPin);  // Assignment
```

### Constants

#### Using const
```cpp
const int LED_PIN = 13;          // Type-safe constant (preferred)
const float PI = 3.14159;
```

#### Using #define
```cpp
#define LED_PIN 13               // Preprocessor constant
#define MAX_SENSORS 5
```

### Variable Scope

```cpp
int globalVar = 0;               // Global - accessible everywhere

void setup() {
  int localVar = 10;             // Local to setup()
}

void loop() {
  static int counter = 0;        // Retains value between loop() calls
  counter++;
}
```

### Volatile Variables

Use `volatile` for variables modified in interrupt service routines (ISRs):

```cpp
volatile int flag = 0;           // Can change unexpectedly

void setup() {
  attachInterrupt(digitalPinToInterrupt(2), buttonISR, RISING);
}

void buttonISR() {
  flag = 1;                      // Modified in ISR
}

void loop() {
  if (flag == 1) {              // Checked in main loop
    // Handle interrupt
    flag = 0;
  }
}
```

---

## Operators

### Arithmetic Operators

| Operator | Description | Example |
|----------|-------------|---------|
| `+` | Addition | `a + b` |
| `-` | Subtraction | `a - b` |
| `*` | Multiplication | `a * b` |
| `/` | Division | `a / b` |
| `%` | Modulus (remainder) | `a % b` |

```cpp
int x = 10 + 5;                  // 15
int y = 10 - 5;                  // 5
int z = 10 * 5;                  // 50
int w = 10 / 3;                  // 3 (integer division)
int r = 10 % 3;                  // 1 (remainder)
```

### Comparison Operators

| Operator | Description | Returns |
|----------|-------------|---------|
| `==` | Equal to | true/false |
| `!=` | Not equal to | true/false |
| `<` | Less than | true/false |
| `>` | Greater than | true/false |
| `<=` | Less than or equal | true/false |
| `>=` | Greater than or equal | true/false |

```cpp
if (sensorValue == 512) {        // Equal comparison
  // Do something
}

if (temperature > 25) {          // Greater than
  // Turn on fan
}
```

### Logical (Boolean) Operators

| Operator | Description | Example |
|----------|-------------|---------|
| `&&` | Logical AND | `a && b` (true if both true) |
| `||` | Logical OR | `a || b` (true if either true) |
| `!` | Logical NOT | `!a` (inverts boolean) |

```cpp
if (temp > 25 && humidity > 60) {    // Both conditions must be true
  // Turn on AC
}

if (buttonA || buttonB) {            // Either button pressed
  // Activate
}

if (!doorClosed) {                   // If door is NOT closed
  // Sound alarm
}
```

### Bitwise Operators

| Operator | Description | Example |
|----------|-------------|---------|
| `&` | Bitwise AND | `a & b` |
| `|` | Bitwise OR | `a | b` |
| `^` | Bitwise XOR | `a ^ b` |
| `~` | Bitwise NOT | `~a` |
| `<<` | Left shift | `a << n` |
| `>>` | Right shift | `a >> n` |

```cpp
byte flags = 0b00001111;
flags = flags & 0b11110000;      // Clear lower 4 bits: 0b00000000
flags = flags | 0b00001100;      // Set bits: 0b00001100
flags = flags ^ 0b00000011;      // Toggle bits: 0b00001111

int value = 4;
int shifted = value << 2;        // Left shift: 16 (multiply by 4)
int reduced = value >> 1;        // Right shift: 2 (divide by 2)
```

**Important**: Don't confuse logical operators (`&&`, `||`) with bitwise operators (`&`, `|`).

### Compound Operators

| Operator | Equivalent | Description |
|----------|------------|-------------|
| `++` | `x = x + 1` | Increment |
| `--` | `x = x - 1` | Decrement |
| `+=` | `x = x + y` | Add and assign |
| `-=` | `x = x - y` | Subtract and assign |
| `*=` | `x = x * y` | Multiply and assign |
| `/=` | `x = x / y` | Divide and assign |

```cpp
int count = 0;
count++;                         // Increment: count = 1
count += 5;                      // Add 5: count = 6
count *= 2;                      // Multiply by 2: count = 12
```

---

## Control Structures

### if Statement

```cpp
if (condition) {
  // Execute if condition is true
}
```

### if-else Statement

```cpp
if (temperature > 25) {
  digitalWrite(fanPin, HIGH);
} else {
  digitalWrite(fanPin, LOW);
}
```

### if-else if-else Chain

```cpp
if (sensorValue < 300) {
  Serial.println("Dark");
} else if (sensorValue < 700) {
  Serial.println("Medium");
} else {
  Serial.println("Bright");
}
```

### for Loop

```cpp
// Standard for loop
for (int i = 0; i < 10; i++) {
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(100);
}

// Iterate through array
int values[] = {1, 2, 3, 4, 5};
for (int i = 0; i < 5; i++) {
  Serial.println(values[i]);
}
```

### while Loop

```cpp
while (condition) {
  // Execute while condition is true
}

// Example
int count = 0;
while (count < 10) {
  Serial.println(count);
  count++;
}
```

### do-while Loop

```cpp
do {
  // Execute at least once, then while condition is true
} while (condition);

// Example
int i = 0;
do {
  Serial.println(i);
  i++;
} while (i < 5);
```

### switch-case Statement

```cpp
int command = 2;

switch (command) {
  case 1:
    Serial.println("Command 1");
    break;
  case 2:
    Serial.println("Command 2");
    break;
  case 3:
    Serial.println("Command 3");
    break;
  default:
    Serial.println("Unknown command");
    break;
}
```

**Note**: Always use `break` unless you intentionally want fall-through behavior.

### break and continue

```cpp
// break - exit loop immediately
for (int i = 0; i < 10; i++) {
  if (i == 5) break;             // Exit loop when i equals 5
  Serial.println(i);
}

// continue - skip to next iteration
for (int i = 0; i < 10; i++) {
  if (i % 2 == 0) continue;      // Skip even numbers
  Serial.println(i);             // Print odd numbers only
}
```

---

## Functions

### Function Declaration

```cpp
returnType functionName(parameters) {
  // Function body
  return value;                  // If returnType is not void
}
```

### Function Examples

```cpp
// No parameters, no return value
void blinkLED() {
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
  delay(500);
}

// With parameters
void blinkLED(int pin, int duration) {
  digitalWrite(pin, HIGH);
  delay(duration);
  digitalWrite(pin, LOW);
  delay(duration);
}

// With return value
int addNumbers(int a, int b) {
  return a + b;
}

// Using the functions
void loop() {
  blinkLED();                    // Call without parameters
  blinkLED(13, 1000);           // Call with parameters
  int sum = addNumbers(5, 3);   // Get return value
}
```

### Function Prototypes

The Arduino IDE automatically generates function prototypes, but for complex code, you may need to declare them manually:

```cpp
// Function prototypes
void blinkLED();
int calculateAverage(int* values, int count);

void setup() {
  // Code
}

void loop() {
  blinkLED();
  // Code
}

// Function definitions
void blinkLED() {
  // Implementation
}

int calculateAverage(int* values, int count) {
  // Implementation
}
```

---

## Pin Modes and I/O

### pinMode()

Configures a pin as input or output.

```cpp
pinMode(pin, mode);
```

**Modes:**
- `OUTPUT`: Low impedance, can source/sink current
- `INPUT`: High impedance, reads external signals
- `INPUT_PULLUP`: High impedance with internal pull-up resistor enabled

```cpp
void setup() {
  pinMode(13, OUTPUT);           // LED output
  pinMode(2, INPUT);             // Button input
  pinMode(3, INPUT_PULLUP);      // Button with internal pull-up
}
```

### Digital I/O

#### digitalWrite()

Sets a digital pin HIGH (5V/3.3V) or LOW (0V).

```cpp
digitalWrite(pin, value);

// Examples
digitalWrite(13, HIGH);          // Turn on LED
digitalWrite(13, LOW);           // Turn off LED
```

#### digitalRead()

Reads the state of a digital pin.

```cpp
int state = digitalRead(pin);

// Example
int buttonState = digitalRead(2);
if (buttonState == HIGH) {
  // Button pressed
}
```

### Analog I/O

#### analogRead()

Reads voltage on analog pin (0-5V or 0-3.3V) as value 0-1023 (10-bit ADC).

```cpp
int value = analogRead(analogPin);

// Example
int sensorValue = analogRead(A0);      // Read from A0
float voltage = sensorValue * (5.0 / 1023.0);  // Convert to voltage
```

**Note**: Analog pins can be referenced by number (0, 1, 2...) or by name (A0, A1, A2...).

#### analogWrite() - PWM

Writes an analog value (PWM) to a pin. Value 0 (always off) to 255 (always on).

```cpp
analogWrite(pin, value);

// Examples
analogWrite(9, 128);             // 50% duty cycle
analogWrite(11, 255);            // 100% duty cycle (fully on)

// Fade LED
for (int brightness = 0; brightness < 255; brightness++) {
  analogWrite(ledPin, brightness);
  delay(10);
}
```

**Note**: Only works on PWM-capable pins (usually marked with ~ on the board).

### Pin Mode Gotchas

- **Digital pins 0 & 1**: Used for Serial communication (RX/TX). Avoid using them for other purposes if using Serial.
- **Floating pins**: Input pins without pull-up/pull-down resistors can read random values. Use `INPUT_PULLUP` or external resistors.
- **Internal pull-up**: Approximately 20-50kΩ, suitable for buttons but not for all applications.

---

## Timing Functions

### delay()

Pauses program execution for specified milliseconds.

```cpp
delay(milliseconds);

// Example
delay(1000);                     // Wait 1 second (1000 ms)
```

**Warning**: Blocking function - no other code executes during delay.

### delayMicroseconds()

Pauses program execution for specified microseconds.

```cpp
delayMicroseconds(microseconds);

// Example
delayMicroseconds(100);          // Wait 100 microseconds
```

**Note**: Maximum value is 16,383 microseconds on 16MHz boards.

### millis()

Returns milliseconds since program started (unsigned long).

```cpp
unsigned long currentMillis = millis();

// Non-blocking blink example
unsigned long previousMillis = 0;
const long interval = 1000;

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    // Toggle LED
    digitalWrite(ledPin, !digitalRead(ledPin));
  }
}
```

**Overflow**: `millis()` overflows after approximately 49.7 days.

### micros()

Returns microseconds since program started (unsigned long).

```cpp
unsigned long currentMicros = micros();

// Precise timing
unsigned long startTime = micros();
// Do something
unsigned long duration = micros() - startTime;
```

**Overflow**: `micros()` overflows after approximately 70 minutes.

**Resolution**: 4 microseconds on 16MHz Arduino boards.

### Timing Best Practices

#### Don't Use delay() for Multitasking

❌ **Bad - Blocking:**
```cpp
void loop() {
  digitalWrite(led1, HIGH);
  delay(1000);                   // Blocks everything
  digitalWrite(led1, LOW);
  delay(1000);
}
```

✅ **Good - Non-blocking:**
```cpp
unsigned long previousMillis = 0;
void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;
    digitalWrite(led1, !digitalRead(led1));
  }
  // Other code can run here
}
```

---

## Serial Communication

Serial communication allows Arduino to communicate with computers and other devices.

### Serial.begin()

Initializes serial communication at specified baud rate.

```cpp
Serial.begin(baudRate);

// Common baud rates: 9600, 19200, 38400, 57600, 115200
Serial.begin(9600);              // Start at 9600 bits/second
```

**Note**: Both devices must use the same baud rate. Place in `setup()`.

### Serial.print() and Serial.println()

Sends data to serial port.

```cpp
Serial.print(data);              // Print without newline
Serial.println(data);            // Print with newline (\r\n)

// Examples
Serial.print("Temperature: ");
Serial.print(temp);
Serial.println(" C");

int value = 255;
Serial.println(value);           // Print as decimal: 255
Serial.println(value, HEX);      // Print as hex: FF
Serial.println(value, BIN);      // Print as binary: 11111111
```

### Serial.read()

Reads incoming serial data (one byte at a time).

```cpp
int incomingByte = Serial.read();

// Example
void loop() {
  if (Serial.available() > 0) {
    int incomingByte = Serial.read();
    Serial.print("Received: ");
    Serial.println(incomingByte);
  }
}
```

### Serial.available()

Returns number of bytes available to read.

```cpp
int bytesAvailable = Serial.available();

// Example
if (Serial.available() > 0) {
  // Data is ready to read
  char c = Serial.read();
}
```

### Reading Strings from Serial

```cpp
String inputString = "";

void loop() {
  while (Serial.available() > 0) {
    char inChar = (char)Serial.read();
    inputString += inChar;

    if (inChar == '\n') {        // End of line
      Serial.println("Received: " + inputString);
      inputString = "";          // Clear string
    }
  }
}
```

### Serial Communication Example

```cpp
void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();

    if (command == '1') {
      digitalWrite(13, HIGH);
      Serial.println("LED ON");
    }
    else if (command == '0') {
      digitalWrite(13, LOW);
      Serial.println("LED OFF");
    }
  }
}
```

### Hardware Serial vs Software Serial

Arduino boards have hardware serial (pins 0 & 1). For additional serial ports, use SoftwareSerial library:

```cpp
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11);  // RX, TX

void setup() {
  Serial.begin(9600);            // Hardware serial
  mySerial.begin(9600);          // Software serial
}
```

---

## Interrupts

Interrupts allow the Arduino to respond immediately to external events.

### attachInterrupt()

Attaches a function (ISR) to an interrupt.

```cpp
attachInterrupt(digitalPinToInterrupt(pin), ISR, mode);
```

**Interrupt Modes:**
- `LOW`: Trigger when pin is LOW
- `CHANGE`: Trigger on any state change
- `RISING`: Trigger on LOW to HIGH transition
- `FALLING`: Trigger on HIGH to LOW transition

```cpp
const byte interruptPin = 2;
volatile byte state = LOW;

void setup() {
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
}

void loop() {
  digitalWrite(13, state);
}

void blink() {
  state = !state;
}
```

### Interrupt Service Routines (ISR)

**ISR Requirements:**
- No parameters
- No return value (void)
- Should be as short and fast as possible
- Cannot use delay()
- Variables shared with main code must be `volatile`

```cpp
volatile int flag = 0;

void setup() {
  attachInterrupt(digitalPinToInterrupt(2), handleInterrupt, RISING);
}

void handleInterrupt() {
  flag = 1;                      // Set flag only
}

void loop() {
  if (flag == 1) {
    // Handle the interrupt in main loop
    Serial.println("Interrupt occurred");
    flag = 0;
  }
}
```

### Volatile Keyword

Use `volatile` for variables modified in ISRs:

```cpp
volatile int counter = 0;        // Tells compiler to always read from memory

void ISR_handler() {
  counter++;                     // Modified in ISR
}

void loop() {
  int value = counter;           // Read in main code
}
```

**Why volatile?** Prevents compiler optimization that might cache variable values, ensuring the actual memory value is always read.

### Available Interrupt Pins

| Board | Interrupt Pins |
|-------|---------------|
| Uno, Nano, Mini | 2, 3 |
| Mega | 2, 3, 18, 19, 20, 21 |
| Leonardo, Micro | 0, 1, 2, 3, 7 |

### detachInterrupt()

Disables an interrupt.

```cpp
detachInterrupt(digitalPinToInterrupt(pin));
```

### Interrupt Best Practices

1. **Keep ISRs short**: Only set flags, don't process
2. **Use volatile**: For all variables shared between ISR and main code
3. **Avoid Serial**: Don't use Serial.print() in ISRs
4. **No delay()**: Cannot use delay() in ISRs
5. **Minimal processing**: Do heavy work in main loop

---

## Memory Management

Arduino microcontrollers have three types of memory:

### Memory Types

| Type | Volatile | Size (Uno) | Usage | Write Cycles |
|------|----------|------------|-------|--------------|
| **Flash** | No | 32 KB | Program storage | 10,000 |
| **SRAM** | Yes | 2 KB | Variables, runtime | Unlimited |
| **EEPROM** | No | 1 KB | Persistent data | 100,000 |

### Flash Memory

Stores the compiled program (sketch).

**Characteristics:**
- Non-volatile (persists after power off)
- Read-only during execution
- 0.5 KB used by bootloader on Uno
- Fast access, but slower than SRAM

#### PROGMEM - Storing Data in Flash

Store constant data in Flash instead of SRAM using `PROGMEM`:

```cpp
#include <avr/pgmspace.h>

// Store string in Flash
const char string_0[] PROGMEM = "This is stored in Flash";

// Store array in Flash
const int values[] PROGMEM = {0, 1, 2, 3, 4};

void setup() {
  Serial.begin(9600);

  // Read from Flash
  char buffer[50];
  strcpy_P(buffer, string_0);
  Serial.println(buffer);

  // Read array value
  int val = pgm_read_word(&values[2]);
  Serial.println(val);
}
```

**Important**: Data in PROGMEM cannot be modified at runtime.

### SRAM (Static RAM)

Stores variables during program execution.

**Characteristics:**
- Volatile (lost on power off)
- Very fast access (2 clock cycles)
- Limited size (2 KB on Uno)
- Shared by: global variables, local variables, function call stack

#### Checking Available SRAM

```cpp
int freeRam() {
  extern int __heap_start, *__brkval;
  int v;
  return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

void setup() {
  Serial.begin(9600);
  Serial.print("Free RAM: ");
  Serial.println(freeRam());
}
```

#### SRAM Optimization Tips

1. **Use appropriate data types**: Use `byte` instead of `int` when possible
2. **Avoid String class**: Use char arrays instead
3. **Use PROGMEM**: Store constants in Flash
4. **Minimize global variables**: Use local variables when possible
5. **Reuse buffers**: Don't create multiple large buffers

### EEPROM

Non-volatile memory for storing data that persists across power cycles.

**Characteristics:**
- Non-volatile (persists after power off)
- Slow access (milliseconds)
- 100,000 write cycles per location
- Good for configuration, calibration data

#### Using EEPROM

```cpp
#include <EEPROM.h>

void setup() {
  // Write single byte
  EEPROM.write(0, 123);

  // Read single byte
  int value = EEPROM.read(0);

  // Write int (2 bytes)
  int address = 0;
  int number = 12345;
  EEPROM.put(address, number);

  // Read int
  int readNumber;
  EEPROM.get(address, readNumber);
}
```

#### EEPROM Best Practices

1. **Minimize writes**: EEPROM has limited write cycles
2. **Update instead of write**: Use `EEPROM.update()` to write only if value changed
3. **Wear leveling**: Distribute writes across addresses
4. **Validate data**: Check if data is valid after reading

```cpp
// Only write if value changed (saves write cycles)
int addr = 0;
byte value = 100;
EEPROM.update(addr, value);      // Only writes if different
```

### Memory Gotchas

1. **Stack/Heap collision**: Excessive local variables + dynamic allocation can cause crashes
2. **String fragmentation**: String class can fragment heap memory
3. **Array bounds**: No bounds checking - overflows corrupt memory
4. **Recursion**: Deep recursion quickly exhausts stack

---

## Libraries

Libraries extend Arduino functionality with reusable code.

### Including Libraries

```cpp
#include <LibraryName.h>

// Examples
#include <Servo.h>               // Servo motor control
#include <Wire.h>                // I2C communication
#include <SPI.h>                 // SPI communication
#include <EEPROM.h>              // EEPROM access
```

**Syntax:**
- `<LibraryName.h>`: Standard library or installed library
- `"LibraryName.h"`: Library in sketch folder

### Using Libraries

```cpp
#include <Servo.h>

Servo myServo;                   // Create servo object

void setup() {
  myServo.attach(9);             // Attach to pin 9
}

void loop() {
  myServo.write(90);             // Set position
  delay(1000);
}
```

### Installing Libraries

**Via Library Manager:**
1. Arduino IDE → Sketch → Include Library → Manage Libraries
2. Search for library
3. Click Install

**Manual Installation:**
1. Download library ZIP
2. Sketch → Include Library → Add .ZIP Library
3. Select downloaded file

### Common Arduino Libraries

| Library | Purpose |
|---------|---------|
| `Servo` | Servo motor control |
| `Wire` | I2C/TWI communication |
| `SPI` | SPI communication |
| `EEPROM` | EEPROM read/write |
| `SoftwareSerial` | Additional serial ports |
| `LiquidCrystal` | LCD display control |
| `SD` | SD card read/write |

### Creating Custom Libraries

#### Basic Library Structure

```
MyLibrary/
├── MyLibrary.h         // Header file
├── MyLibrary.cpp       // Implementation file
├── keywords.txt        // Syntax highlighting (optional)
└── examples/           // Example sketches (optional)
```

#### Header File (MyLibrary.h)

```cpp
#ifndef MyLibrary_h
#define MyLibrary_h

#include <Arduino.h>

class MyLibrary {
  public:
    MyLibrary(int pin);
    void begin();
    void doSomething();

  private:
    int _pin;
};

#endif
```

#### Implementation File (MyLibrary.cpp)

```cpp
#include "MyLibrary.h"

MyLibrary::MyLibrary(int pin) {
  _pin = pin;
}

void MyLibrary::begin() {
  pinMode(_pin, OUTPUT);
}

void MyLibrary::doSomething() {
  digitalWrite(_pin, HIGH);
  delay(100);
  digitalWrite(_pin, LOW);
}
```

#### Using Your Library

```cpp
#include <MyLibrary.h>

MyLibrary myLib(13);

void setup() {
  myLib.begin();
}

void loop() {
  myLib.doSomething();
  delay(1000);
}
```

### Library Dependencies

If your library needs other libraries, include them in both header and main sketch:

```cpp
// In MyLibrary.h
#include <Arduino.h>
#include <Wire.h>

// In main sketch
#include <Wire.h>              // Must include dependencies
#include <MyLibrary.h>
```

---

## Common Gotchas and Best Practices

### Hardware Gotchas

#### 1. Power Supply Issues
❌ **Problem**: Insufficient or incorrect power
- Using 5V when 7-12V required (or vice versa)
- Underpowered USB for high-current devices
- Reversed polarity (destroys board)

✅ **Solution**:
- Use 7-12V for VIN pin (9V recommended)
- Use external power for motors, many LEDs
- Double-check polarity: V+ to VIN, GND to GND

#### 2. Pin Conflicts
❌ **Problem**: Pins 0 & 1 used for both Serial and digital I/O
```cpp
pinMode(0, OUTPUT);              // Conflicts with Serial RX
```

✅ **Solution**: Avoid pins 0 & 1 if using Serial communication

#### 3. Floating Pins
❌ **Problem**: Unpredictable input readings
```cpp
pinMode(2, INPUT);               // No pull-up/pull-down
int val = digitalRead(2);        // Random values
```

✅ **Solution**: Use INPUT_PULLUP or external resistors
```cpp
pinMode(2, INPUT_PULLUP);        // Stable HIGH when not pressed
```

#### 4. PWM Pin Limitations
❌ **Problem**: Using analogWrite() on non-PWM pins
```cpp
analogWrite(2, 128);             // Pin 2 is not PWM-capable
```

✅ **Solution**: Use only PWM pins (marked with ~ on board)
```cpp
analogWrite(9, 128);             // Pin 9 supports PWM
```

### Programming Gotchas

#### 1. Integer Division
❌ **Problem**: Unexpected results from integer division
```cpp
int result = 5 / 2;              // Result is 2, not 2.5
```

✅ **Solution**: Use float for decimal results
```cpp
float result = 5.0 / 2.0;        // Result is 2.5
```

#### 2. Delay Blocking
❌ **Problem**: delay() blocks all code execution
```cpp
void loop() {
  digitalWrite(led1, HIGH);
  delay(1000);                   // Nothing else can happen
  digitalWrite(led1, LOW);
  delay(1000);
}
```

✅ **Solution**: Use millis() for non-blocking timing
```cpp
unsigned long previousMillis = 0;
void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;
    digitalWrite(led1, !digitalRead(led1));
  }
  // Other code runs here
}
```

#### 3. Comparison vs Assignment
❌ **Problem**: Using = instead of ==
```cpp
if (x = 5) {                     // Assignment, not comparison!
  // Always true (except if x becomes 0)
}
```

✅ **Solution**: Use == for comparison
```cpp
if (x == 5) {                    // Comparison
  // True only if x equals 5
}
```

#### 4. Serial Buffer Overflow
❌ **Problem**: Reading only one byte when multiple arrive
```cpp
if (Serial.available() > 0) {
  char c = Serial.read();        // Reads only one byte
}
```

✅ **Solution**: Read all available bytes
```cpp
while (Serial.available() > 0) {
  char c = Serial.read();        // Reads all bytes
}
```

#### 5. millis() Overflow
❌ **Problem**: Not handling millis() rollover (after 49.7 days)
```cpp
if (millis() - previousMillis >= interval) {
  // Fails when millis() overflows
}
```

✅ **Solution**: This actually works correctly due to unsigned arithmetic!
```cpp
if (millis() - previousMillis >= interval) {
  // Handles overflow correctly
  previousMillis = millis();
}
```

#### 6. Missing break in switch
❌ **Problem**: Fall-through in switch statements
```cpp
switch (cmd) {
  case 1:
    doSomething();
    // Falls through to case 2!
  case 2:
    doOther();
    break;
}
```

✅ **Solution**: Always use break (unless fall-through intended)
```cpp
switch (cmd) {
  case 1:
    doSomething();
    break;                       // Prevent fall-through
  case 2:
    doOther();
    break;
}
```

### Memory Gotchas

#### 1. String Class Memory Issues
❌ **Problem**: Heap fragmentation from String class
```cpp
String msg = "Temperature: ";
msg += String(temp);             // Multiple allocations
msg += " C";
```

✅ **Solution**: Use char arrays with sprintf
```cpp
char msg[50];
sprintf(msg, "Temperature: %d C", temp);
```

#### 2. Array Bounds
❌ **Problem**: No bounds checking on arrays
```cpp
int arr[5];
arr[10] = 100;                   // Corrupts memory!
```

✅ **Solution**: Always validate array access
```cpp
int arr[5];
int index = 10;
if (index >= 0 && index < 5) {
  arr[index] = 100;
}
```

#### 3. Stack Overflow
❌ **Problem**: Large local arrays or deep recursion
```cpp
void loop() {
  byte buffer[1000];             // May overflow 2KB SRAM!
}
```

✅ **Solution**: Use smaller buffers or global/static variables
```cpp
byte buffer[100];                // Smaller buffer
// or
static byte buffer[1000];        // Static/global (if needed)
```

### Best Practices

#### 1. Modular Code
✅ Break complex code into functions
```cpp
void updateSensors() {
  temperature = readTemperature();
  humidity = readHumidity();
}

void controlOutputs() {
  if (temperature > 25) digitalWrite(fanPin, HIGH);
  else digitalWrite(fanPin, LOW);
}

void loop() {
  updateSensors();
  controlOutputs();
}
```

#### 2. Descriptive Names
✅ Use clear, meaningful names
```cpp
const int TEMP_SENSOR_PIN = A0;
const int FAN_CONTROL_PIN = 9;
const int TEMP_THRESHOLD = 25;
```

#### 3. Const for Constants
✅ Use const instead of #define for type safety
```cpp
const int LED_PIN = 13;          // Type-safe
#define LED_PIN 13               // No type checking
```

#### 4. Test Incrementally
✅ Build and test small pieces
- Test each sensor individually
- Verify each output separately
- Combine only after individual testing

#### 5. Use Appropriate Data Types
✅ Match data type to use case
```cpp
byte age = 25;                   // 0-255 is enough
unsigned long timestamp = millis(); // Needs 32 bits
int temperature = -10;           // Needs negative values
```

#### 6. Avoid Magic Numbers
❌ **Bad**: Numbers without context
```cpp
if (sensorValue > 512) {         // What is 512?
  digitalWrite(13, HIGH);        // What is 13?
}
```

✅ **Good**: Named constants
```cpp
const int SENSOR_THRESHOLD = 512;
const int LED_PIN = 13;

if (sensorValue > SENSOR_THRESHOLD) {
  digitalWrite(LED_PIN, HIGH);
}
```

#### 7. Handle Interrupts Properly
✅ Keep ISRs short, use volatile
```cpp
volatile bool dataReady = false;

void dataISR() {
  dataReady = true;              // Set flag only
}

void loop() {
  if (dataReady) {
    processData();               // Heavy work here
    dataReady = false;
  }
}
```

---

## Differences from Standard C/C++

### 1. No Standard Library Support

Arduino uses AVR-libc, not the C++ Standard Library.

❌ **Not Available:**
- `std::string`
- `std::vector`
- `std::map`
- `iostream` (cin, cout)
- Most STL containers and algorithms

✅ **Available:**
- Basic C functions (strlen, strcpy, etc.)
- Arduino String class (instead of std::string)
- Arduino-specific libraries

### 2. Arduino-Specific Extensions

#### Main Entry Points
Standard C/C++ uses `main()`, Arduino uses `setup()` and `loop()`:

```cpp
// Standard C++
int main() {
  // Code
  return 0;
}

// Arduino
void setup() {
  // Initialization
}

void loop() {
  // Repeated execution
}
```

#### Pin Functions
Arduino provides simplified hardware access:
```cpp
pinMode(13, OUTPUT);             // Arduino-specific
digitalWrite(13, HIGH);          // Arduino-specific
int val = analogRead(A0);        // Arduino-specific
```

### 3. Automatic Function Prototypes

Arduino IDE automatically generates function prototypes:

```cpp
// You write:
void loop() {
  myFunction();
}

void myFunction() {
  // Code
}

// IDE automatically adds:
void myFunction();               // Prototype
```

### 4. Pre-processor Definitions

Arduino automatically includes:
```cpp
#include <Arduino.h>             // Automatically included
```

Provides:
- `HIGH`, `LOW`
- `INPUT`, `OUTPUT`, `INPUT_PULLUP`
- `true`, `false`
- `PI`, `HALF_PI`, `TWO_PI`
- Core functions (pinMode, digitalWrite, etc.)

### 5. Data Type Differences

#### double vs float
On most Arduino boards, `double` is the same as `float` (32-bit):
```cpp
float f = 3.14;
double d = 3.14;                 // Same precision as float on Arduino!
```

Standard C++ typically has 64-bit double.

#### int Size
Arduino `int` is 16-bit on most boards:
```cpp
int x;                           // 16 bits on Arduino (-32,768 to 32,767)
                                 // 32 bits on most PCs
```

### 6. String Handling

#### C++ Standard Library (Not Available)
```cpp
// Standard C++ - NOT available on Arduino
#include <string>
std::string str = "Hello";       // Won't compile
```

#### Arduino Alternatives
```cpp
// Option 1: Arduino String class
String str = "Hello";
str.concat(" World");

// Option 2: C-style strings (preferred for memory)
char str[20] = "Hello";
strcat(str, " World");
```

### 7. Memory Model

#### No Virtual Memory
Arduino has direct memory access, no virtual memory:
- Fixed memory sizes
- No memory protection
- Stack/heap can collide
- No garbage collection

#### Pointers
Pointers work the same, but memory is limited:
```cpp
int* ptr = &variable;            // Works same as C/C++
byte* buffer = (byte*)malloc(100); // Manual memory management
free(buffer);                    // Must manually free
```

### 8. No Exceptions

Arduino doesn't support C++ exceptions:

❌ **Not Available:**
```cpp
try {
  // Code
} catch (exception& e) {
  // Won't compile
}
```

✅ **Use Instead:**
```cpp
int result = doSomething();
if (result == -1) {
  // Handle error
}
```

### 9. Limited Template Support

Basic templates work, but complex template metaprogramming may not:

✅ **Works:**
```cpp
template<typename T>
T maximum(T a, T b) {
  return (a > b) ? a : b;
}
```

❌ **May Not Work:**
- Complex STL-style templates
- Template specialization (limited)
- Variadic templates (limited)

### 10. No Dynamic Casting

RTTI (Run-Time Type Information) is disabled:

❌ **Not Available:**
```cpp
dynamic_cast<DerivedClass*>(basePtr);  // Won't compile
typeid(obj);                           // Won't compile
```

✅ **Use Instead:**
- Static casting
- Manual type tracking
- Virtual functions for polymorphism

### 11. Compilation Differences

#### Sketch Processing
Arduino IDE preprocesses sketches:
1. Adds `#include <Arduino.h>`
2. Generates function prototypes
3. Wraps code in C++ structure

#### File Extensions
- `.ino` files: Arduino sketches (preprocessed)
- `.cpp` files: Standard C++ (no preprocessing)
- `.h` files: Headers

### Summary Table

| Feature | Standard C++ | Arduino |
|---------|--------------|---------|
| Entry point | `main()` | `setup()` + `loop()` |
| Standard Library | Full STL | No STL (AVR-libc only) |
| String class | `std::string` | `String` class |
| int size | Usually 32-bit | 16-bit |
| double precision | 64-bit | 32-bit (same as float) |
| Exceptions | Supported | Not supported |
| RTTI | Supported | Disabled |
| Templates | Full support | Limited |
| Memory model | Virtual memory | Direct access |
| Automatic includes | No | Yes (`Arduino.h`) |

---

## Additional Resources

### Official Documentation
- [Arduino Language Reference](https://www.arduino.cc/reference/en/)
- [Arduino Documentation](https://docs.arduino.cc/)
- [Arduino Programming Guide](https://docs.arduino.cc/programming)

### Learning Resources
- [Arduino Built-in Examples](https://docs.arduino.cc/built-in-examples/)
- [Arduino Tutorials](https://docs.arduino.cc/learn/)
- [Arduino Forum](https://forum.arduino.cc/)

### Advanced Topics
- AVR-libc Reference (for low-level AVR programming)
- ATmega Datasheets (for hardware details)
- Arduino Hardware Specification

---

## Quick Reference

### Essential Functions
```cpp
// Setup and Loop
void setup()                     // Runs once at start
void loop()                      // Runs repeatedly

// Digital I/O
pinMode(pin, mode)              // Configure pin: OUTPUT, INPUT, INPUT_PULLUP
digitalWrite(pin, value)        // Write: HIGH or LOW
digitalRead(pin)                // Read: HIGH or LOW

// Analog I/O
analogRead(pin)                 // Read 0-1023 (10-bit ADC)
analogWrite(pin, value)         // PWM 0-255 (8-bit)

// Timing
delay(ms)                       // Blocking delay in milliseconds
delayMicroseconds(us)          // Blocking delay in microseconds
millis()                        // Time since start in milliseconds
micros()                        // Time since start in microseconds

// Serial
Serial.begin(baud)              // Initialize serial at baud rate
Serial.print(data)              // Print data
Serial.println(data)            // Print data with newline
Serial.read()                   // Read one byte
Serial.available()              // Bytes available to read

// Interrupts
attachInterrupt(interrupt, ISR, mode)  // Attach interrupt
detachInterrupt(interrupt)             // Detach interrupt

// Math
min(x, y)                       // Minimum value
max(x, y)                       // Maximum value
abs(x)                          // Absolute value
constrain(x, a, b)             // Constrain x between a and b
map(value, fromLow, fromHigh, toLow, toHigh)  // Re-map value range
```

### Pin Modes
- `OUTPUT` - Low impedance output
- `INPUT` - High impedance input
- `INPUT_PULLUP` - Input with internal pull-up resistor

### Interrupt Modes
- `LOW` - Trigger when pin is LOW
- `CHANGE` - Trigger on any change
- `RISING` - Trigger on LOW to HIGH
- `FALLING` - Trigger on HIGH to LOW

---

*This guide covers the fundamentals of Arduino programming. For specific hardware or advanced topics, consult the official Arduino documentation and your board's specific reference materials.*