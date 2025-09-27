# AutoTiller 5000 - User Manual

**Marine Autopilot System for Miura Sailboat**

Arduino-based heading hold autopilot using BNO055 IMU sensor and servo motor control.

---

## Table of Contents

1. [Overview](#overview)
2. [Hardware Components](#hardware-components)
3. [Getting Started](#getting-started)
4. [Operating Modes](#operating-modes)
5. [Button Controls](#button-controls)
6. [Status Indicators](#status-indicators)
7. [Calibration System](#calibration-system)
8. [Settings & Configuration](#settings--configuration)
9. [Audio Feedback](#audio-feedback)
10. [Safety Features](#safety-features)
11. [Troubleshooting](#troubleshooting)
12. [Technical Documentation](#technical-documentation) ⭐ **Developer Info**

**Quick Links for Developers:**
- 📁 [Project Structure](#project-structure) - File organization and architecture
- 🔧 [CLAUDE.md](./CLAUDE.md) - Development guide, build instructions, code conventions
- 🧪 [Testing](./test/README.md) - Unit test documentation

---

## Overview

The AutoTiller 5000 is a safety-critical marine autopilot system designed to maintain a compass heading on a Miura sailboat. The system uses a proportional sine-based control algorithm to smoothly adjust the tiller, maintaining your set course while compensating for wind, waves, and current.

**Key Capabilities:**
- **Automatic heading hold** - Locks and maintains any compass course (0-360°)
- **Fine course adjustments** - ±2° incremental changes while underway
- **Quick tacking** - 90° course changes with safety confirmation
- **Adjustable sensitivity** - Tune response from gentle (30) to aggressive (1000)
- **Reversible servo direction** - Adapt to your tiller configuration
- **Persistent settings** - EEPROM storage survives power cycles
- **Auto-calibration** - BNO055 sensor calibration saved automatically
- **Watchdog protection** - Auto-recovery from system freezes
- **Sensor fail-safe** - Safe mode on sensor communication failure

---

## Hardware Components

### System Architecture Diagram

```
                     ┌─────────────────────────────────────────────────┐
                     │              ARDUINO UNO / NANO                 │
                     │                                                 │
                     │   ┌─────────────────────────────────────────┐   │
                     │   │          ATmega328P CPU                 │   │
                     │   │                                         │   │
                     │   │  • Watchdog Timer (2s) ←── wdt_reset() │   │
                     │   │  • EEPROM (1KB) ←── Settings + Calib   │   │
                     │   │  • 16MHz Clock                          │   │
                     │   │  • I2C Controller (100ms timeout)      │   │
                     │   └─────────────────────────────────────────┘   │
                     │                                                 │
                     └─────────────────────────────────────────────────┘
                                           │
               ┌───────────────────────────┼───────────────────────────┐
               │                           │                           │
               ▼                           ▼                           ▼
       ┌───────────────┐          ┌───────────────┐          ┌───────────────┐
       │    INPUTS     │          │   SENSORS     │          │   OUTPUTS     │
       │  + DEBOUNCE   │          │  + SAFETY     │          │  + CONTROL    │
       └───────────────┘          └───────────────┘          └───────────────┘
               │                           │                           │
    ┌──────────┴──────────┐       ┌────────┴────────┐       ┌─────────┴──────────┐
    │                     │       │                 │       │                    │
    │ Main Btn (P2)       │       │   BNO055 IMU    │       │ Servo Motor (P9)   │
    │ Port Btn (P4)       │       │                 │       │  • 0-180° range    │
    │ Stbd Btn (P5)       │       │  • Gyroscope    │       │  • Tiller control  │
    │                     │       │  • Accel        │       │  • Center: 90°     │
    │ INPUT_PULLUP +      │       │  • Magnetometer │       │  • Proportional    │
    │ 50ms Debounce       │       │                 │       │    sine control    │
    │                     │       │  I2C @ 0x28     │       ├────────────────────┤
    │ ┌─────────────────┐ │       │  SDA → A4       │       │ Buzzer (P8)        │
    │ │ Debounce Logic: │ │       │  SCL → A5       │       │  • Audio feedback  │
    │ │ • State tracking│ │       │                 │       │  • Status tones    │
    │ │ • Time filtering│ │       │ ┌─────────────┐ │       │  • Warning beeps   │
    │ │ • Noise reject │ │       │ │ Timeout: 5  │ │       ├────────────────────┤
    │ └─────────────────┘ │       │ │ failures →  │ │       │ LED 1 (P10)        │
    └─────────────────────┘       │ │ SAFE MODE   │ │       │ LED 2 (P11)        │
                                  │ └─────────────┘ │       │ LED 3 (P12)        │
                                  └─────────────────┘       │  • System status   │
                                                            │  • Calibration     │
                                                            │  • Mode indicators │
                                                            └────────────────────┘

              ┌─────────────────────────────────────────────────────────────┐
              │                    SAFETY FEATURES                          │
              ├─────────────────────────────────────────────────────────────┤
              │  • Watchdog Reset Protection (2s timeout)                   │
              │  • Sensor Timeout Protection (100ms I2C, 5 fail → safe)    │
              │  • Button Debouncing (50ms, noise rejection)               │
              │  • Servo Position Constraints (0-180°)                     │
              │  • EEPROM Write Verification (read-back validation)        │
              │  • Exponential Backoff (sensor recovery)                   │
              │  • Tack Confirmation (10-beep warning + cancel)            │
              └─────────────────────────────────────────────────────────────┘

              ┌─────────────────────────────────────────────────────────────┐
              │                 EEPROM MEMORY MAP (1KB)                     │
              ├─────────────────────────────────────────────────────────────┤
              │  0-253: BNO055 Calibration Blob (CalibBlob struct)         │
              │         Magic Number: 0xB055 (validates calibration)       │
              │  250:   Servo Reverse Flag (int8_t: +1 or -1)              │
              │  251:   Sensitivity Value (int: 30-1000 range)             │
              │  252:   Settings Magic Number (byte: 0xA5)                 │
              │         Validates user settings integrity                   │
              └─────────────────────────────────────────────────────────────┘
```

### Control Buttons
- **Main Button (Pin 2)** - Primary control for course engagement and settings
- **Port Button (Pin 4)** - Course adjustment to port (left)
- **Starboard Button (Pin 5)** - Course adjustment to starboard (right)

### Indicators
- **LED 1 (Pin 10)** - System active indicator
- **LED 2 (Pin 11)** - Course mode / settings indicator
- **LED 3 (Pin 12)** - Diagnostics / high-level menu indicator
- **Buzzer (Pin 8)** - Audio feedback for all operations

### Sensors & Actuators
- **BNO055 IMU** - 9-axis absolute orientation sensor (I2C: SDA→A4, SCL→A5)
- **Servo Motor (Pin 9)** - Tiller control actuator (0-180° range)

---

## Getting Started

### First Power-On

1. **System Check**
   - All LEDs briefly illuminate during startup
   - Serial monitor displays "Orientation Sensor Test" at 115200 baud
   - Watchdog timer enables (2-second timeout)
   - If a watchdog reset occurred, buzzer sounds warning (1000 Hz, 500ms)

2. **Calibration Status**
   - System checks EEPROM for saved BNO055 calibration
   - If found: "Found Calibration for this sensor in EEPROM"
   - If not found: "No Calibration Data for this sensor exists in EEPROM"
   - Sensor begins outputting heading data

3. **Standby Mode**
   - LED 1 illuminates (system active)
   - Serial monitor displays: X (heading), Y (pitch), Z (roll) values
   - Calibration status displayed: Sys/Gyro/Accel/Mag (0-3 scale each)
   - Servo centered at 90° (neutral position)

### Initial Calibration

The BNO055 sensor auto-calibrates through motion. For best results:

1. **Gyroscope** - Place unit stationary for a few seconds
2. **Magnetometer** - Move unit in figure-8 pattern
3. **Accelerometer** - Position unit in 6 orientations (all faces up/down)
4. **System** - Overall calibration (combines all sensors)

**Calibration is automatically saved to EEPROM once fully calibrated (all 3s).**

LED indicators during calibration show system calibration level:
- No LEDs = Level 0 (uncalibrated)
- LED 1 = Level 1
- LEDs 1+2 = Level 2
- LEDs 1+2+3 = Level 3 (fully calibrated)

---

## Operating Modes

### System State Flow Diagram

```
                  ┌─────────────────────────────────────────────────────┐
                  │                POWER ON / RESET                     │
                  │                                                     │
                  │  • Watchdog enabled (2s timeout) + reset detection  │
                  │  • Load calibration from EEPROM (magic: 0xB055)     │
                  │  • Load user settings (magic: 0xA5)                 │
                  │  • Initialize BNO055 sensor (I2C timeout: 100ms)    │
                  │  • Initialize button debounce states (50ms)         │
                  └───────────────────┬─────────────────────────────────┘
                                      │
                                      ▼
                  ┌─────────────────────────────────────────────────────┐
                  │              STANDBY MODE (Default)                 │
                  │                                                     │
                  │  LED 1: ON (system active)                         │
                  │  Display: Heading, Calibration Status (0-3 levels) │
                  │  Servo: Centered (90°)                             │
                  │  Buttons: Debounced (state tracking + time filter) │
                  └─────────────────────────────────────────────────────┘
                            │           │           │
              Main Button   │           │           │ Main Button
              Short Press   │           │           │ Hold 10s
              (Debounced)   ▼           │           ▼
      ┌────────────────────────┐        │    ┌──────────────────────┐
      │   COURSE HOLD MODE     │        │    │ SETTINGS SAVE MODE   │
      │                        │        │    │                      │
      │  LED 1+2: ON           │        │    │  5s: Halfway beep    │
      │  Servo: Active Control │        │    │  10s: Save/Confirm   │
      │  Course: Locked        │        │    │  EEPROM verification │
      │  100ms control loop    │        │    └──────────────────────┘
      └────────────────────────┘        │
              │   │   │   │              │ ┌──────────────────────┐
              │   │   │   │              │ │ Sensor Timeout       │
              │   │   │   │              │ │ Protection:          │
              │   │   │   │              ▼ │ • 100ms I2C timeout  │
              │   │   │   │    ┌─────────────│ • Exponential       │
              │   │   │   │    │ SENSOR  ────│   backoff (10→1000) │
              │   │   │   │    │ SAFE    │   │ • 5 failures max    │
              │   │   │   │    │ MODE    │   │ • Auto-recovery     │
              │   │   │   │    │         │   └──────────────────────┘
              │   │   │   │    │  LEDs: ALL OFF       │
              │   │   │   │    │  Servo: Centered     │
              │   │   │   │    │  Alert: 3x 800Hz     │
              │   │   │   │    │  Course: Disabled    │
              │   │   │   │    └──────────────────────┘
              │   │   │   │
              │   │   │   └─── Main Hold >4s (Debounced) ──────────────┐
              │   │   │                                                │
              │   │   └─── Main Hold 2-4s (Debounced) ──┐              │
              │   │                                      │              │
              │   └─── Main Hold <2s (Debounced)         │              │
              │         (Exit to Standby)                │              │
              │                                          ▼              ▼
              │                            ┌──────────────────┐  ┌─────────────────┐
              │                            │ SENSITIVITY      │  │ SERVO REVERSE   │
              │                            │ ADJUST MODE      │  │     MODE        │
              │                            │                  │  │                 │
              │                            │ Port: Dec-10     │  │ Toggle: +1↔-1   │
              │                            │ Stbd: Inc+10     │  │ LED 1+2+3: ON   │
              │                            │ Range: 30-1000   │  │ Immediate save  │
              │                            │ Buzz: Frequency  │  └─────────────────┘
              │                            │ (pitch = value)  │
              │                            └──────────────────┘
              │
              └─── Port/Stbd Buttons (Debounced)
                   • Short: ±2° course adjust
                   • Long: ±90° tack (10-beep warning)
                   • Cancel: Main button during tack beeps

      ┌─────────────────────────────────────────────────────────────────┐
      │                        SAFETY MONITOR                           │
      │                     (Active in all modes)                      │
      ├─────────────────────────────────────────────────────────────────┤
      │  • Watchdog Timer: wdt_reset() every loop (2s timeout)         │
      │  • Button Debouncing: 50ms state tracking + noise rejection    │
      │  • Sensor Monitoring: 100ms I2C timeout + failure counting     │
      │  • Servo Constraints: Position clamped [0, 180°]               │
      │  • EEPROM Integrity: Write verification + magic number check   │
      └─────────────────────────────────────────────────────────────────┘
```

### 1. Standby Mode (Default)

**Display:** Current heading on serial monitor, calibration status on LEDs

**Functions:**
- Monitor compass heading in real-time
- View sensor calibration status
- Save settings (10-second main button hold from standby)
- Enter course hold mode (press main button)

**LED Status:** LED 1 ON (system active)

---

### 2. Course Hold Mode (Active Autopilot)

**Entry:** Press main button in standby mode

**Operation:**
- Current heading becomes locked course
- Servo continuously adjusts to maintain course
- LED 2 illuminates (course mode active)
- Serial monitor displays:
  ```
  Heading | Course | Heading Adj | Servo Pos | Servo Adjust
  045     | 045    | 0           | 90        | 0.0
  ```

**Active Functions:**
- Course adjustments (port/starboard buttons)
- Tacking maneuvers (long press port/starboard)
- Exit to standby (short main button press <2s)
- Enter settings menu (hold main button 2-4s)
- Toggle servo reverse (hold main button >4s)

**LED Status:** LEDs 1+2 ON (system active + course engaged)

---

### 3. Settings Adjustment Mode

**Entry:** From course hold mode, hold main button for 2-4 seconds

**LED Sequence During Button Hold:**
- 0-2s: LED 1 flashing (will exit to standby)
- 2-4s: LEDs 1+2 flashing (will enter sensitivity adjust)
- >4s: LEDs 1+2+3 flashing (will toggle servo reverse)

**Sensitivity Adjustment:**
- Port button: Decrease by 10 (minimum 30)
- Starboard button: Increase by 10 (maximum 1000)
- Buzzer pitch indicates current value (higher frequency = higher sensitivity)
- Serial monitor displays: "Sensitivity: [value]"
- Press main button to return to course hold mode

**Range:**
- **30-100** = Very gentle response (light wind, calm seas)
- **100-300** = Moderate response (typical conditions)
- **300-600** = Aggressive response (strong wind, rough seas)
- **600-1000** = Maximum response (emergency/testing)

---

### 4. Servo Reverse Mode

**Entry:** From course hold mode, hold main button for >4 seconds

**Operation:**
- Servo direction multiplier toggles between +1 and -1
- Use if servo moves opposite to desired direction
- Serial monitor displays: "Reverse toggled to: [1 or -1]"
- Immediately returns to course hold mode
- Setting persists until changed or saved to EEPROM

**LED Status:** LEDs 1+2+3 ON during button hold

---

### 5. Settings Save Mode

**Entry:** From standby mode, hold main button for 10 seconds

**Operation:**
- 5-second mark: Single beep (800 Hz, "halfway" confirmation)
- Serial: "Hold for 5 more seconds to save settings..."
- 10-second mark:
  - **If changes exist:** 3 quick beeps (1500 Hz) = saved successfully
  - **If no changes:** Single low beep (800 Hz) = nothing to save
  - **On error:** Long low beep (600 Hz, 2s) = EEPROM write failed

**Saved Settings:**
- Sensitivity value
- Servo reverse direction

**LED Status:** LED 1 ON (standby mode)

---

### 6. Sensor Safe Mode (Automatic)

**Trigger:** 5 consecutive sensor communication failures

**Operation:**
- Servo immediately centers to 90° (neutral)
- Course hold mode exits (pushed = HIGH)
- Triple warning beep (800 Hz)
- Serial: "CRITICAL: Entering sensor safe mode!"
- System attempts recovery on next successful sensor read

**Recovery:**
- Automatic when sensor communication restored
- Single beep (1200 Hz, 200ms) confirmation
- Serial: "Sensor recovered from safe mode"
- Returns to standby mode

**LED Status:** All LEDs OFF (emergency state)

---

## Button Controls

### Main Button (Pin 2)

| Context | Press Duration | Action | LED Indicator |
|---------|---------------|--------|---------------|
| Standby | Short press | Enter course hold mode | LED 2 ON |
| Standby | Hold 10s | Save settings to EEPROM | LED 1 flashing |
| Course Hold | <2s hold + release | Exit to standby mode | LED 1 flashing |
| Course Hold | 2-4s hold + release | Enter sensitivity adjust | LEDs 1+2 flashing |
| Course Hold | >4s hold + release | Toggle servo reverse | LEDs 1+2+3 flashing |
| Tack Warning | Press during beeps | Cancel tack, revert course | Beeping stops |

---

### Port Button (Pin 4)

| Context | Press Duration | Action | Audio Feedback |
|---------|---------------|--------|----------------|
| Course Hold | Short (<3s) | Adjust course -2° (to port) | 200 Hz tone |
| Course Hold | Long (>3s) | Tack -90° (to port) | 10 warning beeps at 600 Hz |
| Sensitivity Adjust | Any press | Decrease sensitivity by 10 | Current sensitivity frequency |

---

### Starboard Button (Pin 5)

| Context | Press Duration | Action | Audio Feedback |
|---------|---------------|--------|----------------|
| Course Hold | Short (<3s) | Adjust course +2° (to starboard) | 400 Hz tone |
| Course Hold | Long (>3s) | Tack +90° (to starboard) | 10 warning beeps at 600 Hz |
| Sensitivity Adjust | Any press | Increase sensitivity by 10 | Current sensitivity frequency |

---

## Status Indicators

### LED Patterns

| LEDs | Standby | Course Hold | Settings | Meaning |
|------|---------|-------------|----------|---------|
| LED 1 | ON | ON | Flashing | System active |
| LED 2 | OFF | ON | Flashing | Course locked |
| LED 3 | OFF | OFF | ON (>4s hold) | High-level menu |
| 1+2+3 (calibration) | Flashing | OFF | OFF | Sensor calibration level (0-3) |

### Calibration Display (Standby Mode Only)

The number of LEDs indicates BNO055 **system calibration level**:
- **0 LEDs** = Level 0 (uncalibrated - move sensor to calibrate)
- **1 LED** = Level 1 (partial calibration)
- **2 LEDs** = Level 2 (good calibration)
- **3 LEDs** = Level 3 (fully calibrated - will auto-save)

---

## Audio Feedback

### Operational Tones

| Frequency | Duration | Meaning |
|-----------|----------|---------|
| 200 Hz | Continuous | Port button pressed |
| 400 Hz | Continuous | Starboard button pressed |
| 600 Hz | 10 beeps | Tack warning (10 beeps with 50ms delays) |
| Variable | Continuous | Sensitivity adjustment (pitch = value) |

### System Status Tones

| Frequency | Duration | Meaning |
|-----------|----------|---------|
| 1000 Hz | 500ms | Watchdog reset on startup |
| 800 Hz | 1000ms (3x) | Sensor safe mode entered |
| 1200 Hz | 200ms | Sensor recovered from safe mode |
| 1500 Hz | 100ms (3x) | Settings saved successfully |
| 800 Hz | 200ms | No changes to save |
| 600 Hz | 2000ms | EEPROM write error |
| 800 Hz | 100ms | Halfway point (5s) during 10s save hold |

---

## Calibration System

### BNO055 Sensor Calibration

**Auto-Save Behavior:**
- Calibration saved **once per boot** when sensor reaches fully calibrated state (3/3/3/3)
- Stored in EEPROM addresses 0-253 with magic number 0xB055
- Automatically loaded on next power-up
- Sensor may refine calibration with gentle motion after loading

**Calibration Requirements:**
1. **Gyro** (0-3): Stationary placement for 2-3 seconds
2. **Accel** (0-3): Position in 6 orientations (each face up/down)
3. **Mag** (0-3): Figure-8 motion to map magnetic field
4. **Sys** (0-3): Overall fusion calibration (combines above)

**Serial Monitor Output:**
```
Sys: 3  Gyro: 3  Accel: 3  Mag: 3  [FULLY CALIBRATED]
```

**LED Calibration Display:** In standby mode, LEDs 1-3 show system calibration level (0-3)

---

### User Settings Calibration

**Persistent Settings (EEPROM addresses 250-252):**
- **Address 250:** Servo reverse flag (±1)
- **Address 251:** Sensitivity value (30-1000)
- **Address 252:** Magic number (0xA5) - indicates valid settings

**Save Procedure:**
1. From standby mode, hold main button for 10 seconds
2. At 5 seconds: Single beep (halfway confirmation)
3. At 10 seconds:
   - 3 beeps = saved successfully
   - 1 beep = no changes to save
   - Long beep = error

**Load Behavior:**
- Settings automatically loaded on startup
- If magic number invalid: defaults used (sensitivity=100, reverse=1)
- Serial confirmation: "User settings loaded from EEPROM"

---

## Control Algorithm

### Proportional Sine-Based Control

The AutoTiller uses a sophisticated control formula:

```
servo_adjustment = (sensitivity/10) × sin(heading_adjustment × π/180) × reverse
servo_position = constrain(servo_position + servo_adjustment, 0, 180)
```

### Control Loop Flow Diagram

```
  ┌───────────────────────────────────────────────────────────────────┐
  │                       COURSE HOLD LOOP                           │
  │                      (100ms cycle time)                          │
  │               Safety-Critical Marine Control                     │
  └───────────────────────────────────────────────────────────────────┘
                                    │
                                    ▼
                  ┌─────────────────────────────┐
                  │    Watchdog Reset (wdt)     │◄─── System Protection
                  │    Reset every loop cycle   │     (2s timeout)
                  └─────────────────────────────┘
                                    │
                                    ▼
              ┌─────────────────────────────────────┐
              │        Read BNO055 Heading          │
              │                                     │
              │  getBNO055EventWithTimeout():       │
              │  • I2C timeout: 100ms               │
              │  • Failure tracking                 │
              │  • Exponential backoff (10→1000ms)  │
              └─────────────────────────────────────┘
                                    │
                   ┌────────────────┴────────────────┐
                   │                                 │
                   ▼                                 ▼
        ┌──────────────────┐              ┌──────────────────────┐
        │  Sensor Success  │              │   Sensor Timeout     │
        │                  │              │                      │
        │ • Reset failures │              │ • Increment failures │
        │ • Reset backoff  │              │ • Exponential delay  │
        │ • Exit safe mode │              │ • 5 fails → SAFE    │
        └──────────────────┘              │ • Center servo       │
                   │                      │ • Exit course mode   │
                   ▼                      └──────────────────────┘
        ┌────────────────────────────────────┐
        │    Calculate Heading Adjustment    │
        │                                    │
        │  headadj = course - heading        │
        │  Normalize to [-180, +180]         │
        │                                    │
        │  Example:                          │
        │   Course: 10°, Heading: 350°       │
        │   → headadj = +20° (not -340°)     │
        │                                    │
        │  Safety: Always validate inputs    │
        └────────────────────────────────────┘
                   │
                   ▼
        ┌────────────────────────────────────┐
        │    Calculate Servo Adjustment      │
        │                                    │
        │  adjustment = (sensitivity/10)     │
        │             × sin(headadj × π/180) │
        │             × reverse              │
        │                                    │
        │  Example (sensitivity=100):        │
        │   headadj=+20° → +3.4° servo       │
        │   headadj=+5°  → +0.9° servo       │
        │   (smooth, proportional response)  │
        └────────────────────────────────────┘
                   │
                   ▼
        ┌────────────────────────────────────┐
        │       Update Servo Position        │
        │                                    │
        │  servoposition += adjustment       │
        │  Constrain to [0, 180]             │
        │  Write to servo (Pin 9)            │
        │  Safety: Hard limits enforced      │
        └────────────────────────────────────┘
                   │
                   ▼
        ┌────────────────────────────────────┐
        │      Check Button Inputs           │
        │      (Debounced Processing)        │
        │                                    │
        │  debouncedDigitalRead():           │
        │  • 50ms debounce window            │
        │  • State tracking                  │
        │  • Noise rejection                 │
        │                                    │
        │  Actions:                          │
        │  • Port/Stbd: Course adjust ±2°   │
        │  • Long Press: Tack ±90° + beeps   │
        │  • Main: Exit/Settings             │
        └────────────────────────────────────┘
                   │
                   ▼
        ┌────────────────────────────────────┐
        │         Serial Diagnostics         │
        │                                    │
        │  Display: Heading | Course |       │
        │           HeadAdj | ServoPos |     │
        │           ServoAdj                 │
        │                                    │
        │  Safety: Monitor all values        │
        └────────────────────────────────────┘
                   │
                   │
                   └─────────► Loop (100ms delay)
                               │
                               ▼
                    ┌─────────────────────┐
                    │   Safety Monitor    │
                    │   (Background)      │
                    │                     │
                    │ • Watchdog health   │
                    │ • Sensor backoff    │
                    │ • Button debounce   │
                    │ • EEPROM integrity  │
                    └─────────────────────┘
```

### Tacking Sequence Flow

```
  Port/Starboard Button Long Press (>3s) During Course Hold
                     │
                     ▼
          ┌──────────────────────────┐
          │  Save Current Course     │
          │  (oldcourse = course)    │
          └──────────────────────────┘
                     │
                     ▼
          ┌──────────────────────────┐
          │  Calculate New Course    │
          │                          │
          │  Port:  course -= 90°    │
          │  Stbd:  course += 90°    │
          │                          │
          │  Normalize [0, 360]      │
          └──────────────────────────┘
                     │
                     ▼
          ┌─────────────────────────────────────┐
          │  10-Beep Warning Sequence           │
          │                                     │
          │  ♪ 600Hz beep (50ms)                │
          │     delay (300ms)                   │
          │  ♪ Check main button (cancel?)      │
          │     → Repeat 10 times               │
          └─────────────────────────────────────┘
                     │
         ┌───────────┴───────────┐
         │                       │
         ▼                       ▼
    ┌────────────┐        ┌─────────────────┐
    │ Main Btn   │        │  Complete       │
    │ Pressed    │        │  10 Beeps       │
    └────────────┘        └─────────────────┘
         │                       │
         ▼                       ▼
    ┌──────────────┐      ┌───────────────────┐
    │ CANCEL TACK  │      │ EXECUTE TACK      │
    │              │      │                   │
    │ Restore:     │      │ New course holds  │
    │ course =     │      │ Servo adjusts     │
    │  oldcourse   │      │ to new heading    │
    └──────────────┘      └───────────────────┘
```

**Heading Adjustment Calculation:**
- Normalized to range [-180°, +180°]
- Negative = turn to port
- Positive = turn to starboard
- Uses shortest angular distance (e.g., 350° to 10° = +20°, not -340°)

**Sensitivity Effect:**
- Dividing by 10 gives proportional gain (e.g., 100 → gain of 10)
- Higher sensitivity = stronger servo response to heading error
- Sine function provides smooth, non-linear response (small errors → gentle correction)

**Servo Constraints:**
- Position clamped to 0-180° to prevent mechanical damage
- Centered at 90° when disengaged

---

## Safety Features

### Watchdog Timer
- **Timeout:** 2 seconds
- **Reset Detection:** Buzzer warning on startup (1000 Hz, 500ms)
- **Purpose:** Auto-recovery from system freezes or infinite loops
- **Manual Reset:** `wdt_reset()` called throughout main loop

### Sensor Timeout Protection
- **I2C Timeout:** 100ms per read attempt
- **Failure Threshold:** 5 consecutive failures trigger safe mode
- **Safe Mode Actions:**
  - Servo centers to 90° (neutral)
  - Course hold exits
  - Triple warning beep
  - System attempts auto-recovery

### Tack Confirmation
- **10-beep warning** (600 Hz) before 90° course change
- **Cancel capability:** Press main button during warning
- **Purpose:** Prevent accidental major course changes

### EEPROM Verification
- **Write-verify cycle:** Read back after write to confirm
- **Error handling:** Long beep (600 Hz, 2s) on verification failure
- **Prevents:** Corrupt settings from causing erratic behavior

### Button Debouncing
- **Debounce delays:** 300ms (standard), 30ms (settings adjust)
- **Purpose:** Prevent switch bounce from triggering multiple actions

---

## Troubleshooting

### Sensor Issues

**Problem:** "Ooops, no BNO055 detected..."
**Solution:**
- Check I2C connections: SDA→A4, SCL→A5
- Verify sensor power (3-5V DC, common ground)
- Confirm I2C address 0x28 (check sensor solder jumpers)

**Problem:** "WARNING: Sensor timeout during calibration"
**Solution:**
- Move sensor gently to help calibration converge
- Wait for 3-second timeout, system will continue
- Check for I2C bus interference (shorten wires, add pull-ups)

**Problem:** "CRITICAL: Entering sensor safe mode!"
**Solution:**
- System detected 5 consecutive read failures
- Check all I2C connections and power
- System will auto-recover when sensor communication restores

---

### Calibration Issues

**Problem:** Calibration won't reach level 3
**Solution:**
- **Gyro:** Ensure unit is completely stationary for 2-3 seconds
- **Accel:** Slowly position in all 6 orientations (each face up/down)
- **Mag:** Move away from magnetic interference (motors, batteries, metal)
- **Figure-8 motion:** Helps magnetometer calibration

**Problem:** "No Calibration Data for this sensor exists in EEPROM"
**Solution:**
- Normal for first use or new sensor
- Calibrate by moving through positions
- Will auto-save when reaching 3/3/3/3

**Problem:** Calibration lost after power cycle
**Solution:**
- Check EEPROM save occurred: "Calibration saved to EEPROM" in serial
- Magic number 0xB055 at address 0 confirms valid calibration blob
- Re-calibrate if necessary

---

### Control Issues

**Problem:** Servo moves opposite to desired direction
**Solution:**
- Toggle servo reverse: Hold main button >4s from course hold mode
- Verify: Turn to port should move servo to correct port
- Save setting: 10-second hold from standby mode

**Problem:** Course hunting (oscillation around target heading)
**Solution:**
- **Reduce sensitivity:** Hold main button 2-4s, decrease with port button
- Start at 100, adjust down to 30-50 for smoother response
- Higher seas may require higher sensitivity (300-600)

**Problem:** Servo response too slow/weak
**Solution:**
- **Increase sensitivity:** Hold main button 2-4s, increase with starboard button
- Range 300-1000 for stronger response
- Note: Very high values (>600) may cause hunting

---

### Settings Issues

**Problem:** "No saved settings found - using defaults"
**Solution:**
- Normal for first use
- Adjust sensitivity/reverse as needed
- Save with 10-second main button hold from standby

**Problem:** Settings not persisting after power cycle
**Solution:**
- Verify save sequence: 5s beep → 10s 3 beeps (success)
- Check serial: "Settings saved successfully"
- If "EEPROM write verification failed!" appears, retry save

**Problem:** "No changes to save" when trying to save
**Solution:**
- Settings already match last saved values
- Make adjustment first (sensitivity or reverse)
- Then save with 10-second hold

---

### Startup Issues

**Problem:** "WARNING: Watchdog reset detected!"
**Solution:**
- System recovered from freeze/hang
- Review recent changes if persistent
- Check for infinite loops or blocking delays in code

**Problem:** Erratic behavior on startup
**Solution:**
- Wait for full sensor calibration (3/3/3/3)
- Check serial monitor for error messages
- Verify all button connections (pins 2, 4, 5 with INPUT_PULLUP)

---

## Technical Documentation

### Project Structure

```
autotiller5000/
├── autotiller5000.ino          # Main Arduino sketch (152 lines)
│   └── Runtime state + setup() + loop() orchestration only
│
├── src/                         # All header files (22 files, 1,731 lines)
│   ├── Configuration (2 files):
│   │   ├── constants.h          # System constants + const struct initialization
│   │   └── config.h             # Configuration struct definitions
│   │
│   ├── Pure Logic Headers (11 files - testable, no side effects):
│   │   ├── navigation.h         # Heading/course calculations
│   │   ├── safety_logic.h       # Safety evaluation logic
│   │   ├── button_logic.h       # Button timing logic
│   │   ├── button_debounce.h    # Hardware button debouncing
│   │   ├── watchdog_safety.h    # Watchdog timer safety functions
│   │   ├── tack_logic.h         # Tack warning logic
│   │   ├── led_patterns.h       # LED pattern calculations
│   │   ├── course_adjustment.h  # Course adjustment logic
│   │   ├── state_machine.h      # State machine logic
│   │   ├── servo_control.h      # Servo position control
│   │   └── sensitivity_adjustment.h # Sensitivity logic
│   │
│   ├── Workflow Headers (8 files - hardware integration):
│   │   ├── calibration.h        # EEPROM calibration save/load
│   │   ├── settings_persistence.h # User settings EEPROM
│   │   ├── sensor_safety.h      # Sensor timeout handling
│   │   ├── button_workflows.h   # Button press workflows
│   │   ├── state_workflows.h    # State machine workflows
│   │   ├── save_workflow.h      # Settings save workflow
│   │   ├── setup_workflows.h    # Setup orchestration
│   │   └── loop_workflows.h     # Loop orchestration
│   │
│   └── Diagnostics (1 file):
│       └── diagnostics.h        # Serial debug output
│
├── test/                        # Unit tests (14 files, 98 tests, 1,400 lines)
│   ├── test_navigation.cpp
│   ├── test_safety_logic.cpp
│   ├── test_button_logic.cpp
│   ├── test_button_debounce.cpp
│   ├── test_tack_logic.cpp
│   ├── test_led_patterns.cpp
│   ├── test_course_adjustment.cpp
│   ├── test_state_machine.cpp
│   ├── test_servo_control.cpp
│   ├── test_servo_debug.cpp
│   ├── test_sensitivity_adjustment.cpp
│   ├── test_settings_persistence.cpp
│   ├── test_watchdog_safety.cpp
│   ├── test_sensor_backoff.cpp
│   └── Makefile                 # Build system for tests
│
└── docs/                        # Documentation
    ├── README.md                # Documentation index
    ├── code_reviews/            # Code analysis
    ├── research/                # Technical references
    └── ai/context/              # AI development context
```

### Key Architecture Principles

**Main .ino File (152 lines):**
- **ZERO logic** - Pure orchestration only
- Contains only: includes, runtime state variables, setup(), loop()
- All configuration moved to `src/constants.h`
- All logic extracted to header files in `src/`

**Configuration System:**
- `src/constants.h` - Single source of truth (constants + configuration structs)
- `src/config.h` - 8 struct type definitions
- Struct-based parameter passing reduces function signatures

**Testing:**
- 98 unit tests covering all pure logic functions
- Simple assert-based testing (no frameworks needed)
- Test with: `cd test && make`
- 100% coverage of testable pure logic

### For Developers

- **[CLAUDE.md](./CLAUDE.md)** - Complete development guide
  - Hardware pin assignments
  - Build and upload instructions
  - Code architecture and conventions
  - Safety considerations
  - Testing approach

- **[docs/README.md](./docs/README.md)** - Documentation index
  - Navigation guide for all docs
  - Research paper summaries
  - Code review findings

### Research Documentation

- **[docs/research/arduino-programming.md](./docs/research/arduino-programming.md)** - Arduino language reference
- **[docs/research/arduino-tdd.md](./docs/research/arduino-tdd.md)** - Testing frameworks and mocking
- **[docs/research/arduino-reliability.md](./docs/research/arduino-reliability.md)** - Embedded reliability patterns
- **[docs/research/autotillers.md](./docs/research/autotillers.md)** - Marine autopilot theory
- **[docs/research/seek-and-hunt.md](./docs/research/seek-and-hunt.md)** - Hunting behavior solutions
- **[docs/research/sailing-mathematics.md](./docs/research/sailing-mathematics.md)** - Navigation calculations

### Code Reviews

- **[docs/code_reviews/](./docs/code_reviews/)** - Comprehensive code analysis
  - 19 issues identified (7 critical)
  - Improvement recommendations
  - Safety enhancements

---

## Quick Reference Card

### Button Quick Reference

| Action | Buttons | Duration | Mode |
|--------|---------|----------|------|
| Engage course | Main | Short | Standby |
| Exit course | Main | <2s hold | Course Hold |
| Adjust sensitivity | Main | 2-4s hold | Course Hold |
| Toggle reverse | Main | >4s hold | Course Hold |
| Save settings | Main | 10s hold | Standby |
| Course -2° | Port | Short | Course Hold |
| Course +2° | Starboard | Short | Course Hold |
| Tack -90° | Port | >3s hold | Course Hold |
| Tack +90° | Starboard | >3s hold | Course Hold |
| Cancel tack | Main | During beeps | Tack Warning |
| Decrease sensitivity | Port | Any | Sensitivity Mode |
| Increase sensitivity | Starboard | Any | Sensitivity Mode |

### LED Quick Reference

| Pattern | Meaning |
|---------|---------|
| LED 1 ON | System active (standby) |
| LEDs 1+2 ON | Course engaged |
| LEDs flashing (1→1+2→1+2+3) | Menu levels during hold |
| LEDs showing 0-3 | Sensor calibration level |

### Audio Quick Reference

| Tone | Meaning |
|------|---------|
| 200 Hz | Port button |
| 400 Hz | Starboard button |
| 600 Hz (10 beeps) | Tack warning |
| 800 Hz (3 beeps) | Safe mode / no changes |
| 1000 Hz | Watchdog reset |
| 1200 Hz | Sensor recovery |
| 1500 Hz (3 beeps) | Settings saved |
| Variable pitch | Sensitivity value |

---

## Safety Notice

⚠️ **CRITICAL SAFETY WARNING**

This is a **safety-critical marine system** that controls your vessel's steering. Before deploying:

1. **Extensive testing required** - Test in calm conditions with experienced crew
2. **Always maintain proper watch** - Never rely solely on autopilot
3. **Manual override capability** - Be ready to instantly disengage and steer manually
4. **Monitor system status** - Watch for safe mode alerts and sensor warnings
5. **Understand limitations** - Autopilot cannot detect traffic, obstacles, or hazards
6. **Follow maritime regulations** - Comply with COLREGs and local navigation rules
7. **Battery backup** - Ensure adequate power supply for extended operation
8. **Weather awareness** - Disengage in severe weather or challenging conditions

**The operator is fully responsible for safe navigation at all times.**

---

## Version Information

- **Firmware:** AutoTiller 5000
- **Hardware:** Arduino Uno / Arduino-compatible
- **Sensor:** Adafruit BNO055 (I2C 0x28)
- **Libraries:**
  - Adafruit BNO055
  - Adafruit Unified Sensor
  - Servo (Arduino/Michael Margolis)
  - Wire, EEPROM (built-in)

---

*For development questions, see [CLAUDE.md](./CLAUDE.md)*
*For additional research, see [docs/README.md](./docs/README.md)*
