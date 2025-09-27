# Hunting and Oscillation in Marine Autopilots

## Important Clarification

**"Seek and hunt" is not a control technique or operating mode** in marine autopilot systems. The term "hunting" refers to an **undesirable oscillation behavior** where the vessel continuously weaves back and forth around the desired course. This document explains the hunting phenomenon, its causes, solutions, and the control strategies that prevent or minimize it.

## Table of Contents
- [What is Hunting?](#what-is-hunting)
- [Root Causes of Hunting](#root-causes-of-hunting)
- [Control Strategies and Their Hunting Characteristics](#control-strategies-and-their-hunting-characteristics)
- [Diagnosing Hunting Behavior](#diagnosing-hunting-behavior)
- [Eliminating Hunting: Tuning Parameters](#eliminating-hunting-tuning-parameters)
- [Implementation for Arduino/DIY Autopilots](#implementation-for-arduinodiy-autopilots)
- [Sea State and Operating Conditions](#sea-state-and-operating-conditions)
- [Power and Fuel Efficiency Implications](#power-and-fuel-efficiency-implications)
- [Comparison to Other Control Issues](#comparison-to-other-control-issues)

## What is Hunting?

### Definition

**Hunting** is a periodic oscillation of the vessel's heading around the target course. The autopilot continuously overcorrects, causing the boat to zigzag as it repeatedly overshoots the desired heading in both directions.

```
Desired Course (180°)
         │
         │    ╱╲      ╱╲      ╱╲
         │   ╱  ╲    ╱  ╲    ╱  ╲
         │  ╱    ╲  ╱    ╲  ╱    ╲
─────────┼─╱──────╲╱──────╲╱──────╲────→ Time
         │
    Hunting pattern: continuous oscillation
```

### Characteristics

1. **Continuous Oscillation**: Vessel heading swings rhythmically port and starboard of the set course
2. **Rudder Activity**: Excessive back-and-forth rudder movement
3. **No Convergence**: Unlike proper damping, the oscillation does not diminish over time
4. **Periodic Pattern**: Regular, repeating cycle of overcorrection

### Physical Manifestation

- Visible wake shows serpentine or sinusoidal pattern
- Increased distance traveled (longer track over ground)
- Helmsman can feel/see continuous rudder reversals
- Uncomfortable motion for crew
- Reduced speed made good (SMG) toward destination

## Root Causes of Hunting

### 1. Control Algorithm Deficiencies

**Proportional-Only Control**

The most fundamental cause of hunting is using **proportional control alone** without derivative damping:

```
Rudder Command = Kp × heading_error

Problem: No damping term to slow approach to target
Result: System overshoots, then overcorrects, repeatedly
```

**Why P-only control hunts:**
- Rudder angle is proportional to error magnitude
- As vessel approaches desired heading, rudder decreases proportionally
- Vessel momentum carries it past the setpoint
- Error now exists in opposite direction
- Rudder applies correction in opposite direction
- Cycle repeats indefinitely

**Mathematical Explanation:**

In control theory, a proportional-only controller creates a **second-order underdamped response** when controlling an integrating process (like vessel heading). The system exhibits sustained oscillation around the setpoint with no natural damping.

### 2. Excessive Proportional Gain (Kp)

Even with PID control, **Kp set too high** causes hunting:

- Rudder response is overly aggressive for heading errors
- Small deviations trigger large rudder movements
- Vessel overshoots correction
- Oscillation amplitude increases (potential instability)

**Critical Gain (Ku):**
- There exists a proportional gain value where sustained oscillation begins
- Operation above Ku causes growing oscillation (instability)
- Operation near Ku causes hunting behavior
- Proper tuning keeps Kp well below Ku

### 3. Insufficient Derivative Gain (Kd)

The **derivative term provides damping** by responding to rate of heading change:

```
D_term = Kd × (rate_of_heading_change)

Purpose: Apply counter-rudder as vessel approaches desired heading
Effect: Slows the approach, prevents overshoot
```

**Too little Kd:**
- Insufficient damping to prevent overshoot
- Vessel swings past target heading
- Results in oscillation

**No Kd (P or PI control):**
- No predictive damping whatsoever
- Guaranteed hunting in most marine applications

### 4. Mechanical and System Lag

**Sources of Lag:**
- Sensor measurement delay (compass update rate)
- Computational delay (control loop execution time)
- Actuator response time (motor/hydraulic system)
- Mechanical slack/play in steering linkages
- Rudder hysteresis in hydraulic systems

**Impact:**
- Correction arrives after it's needed
- System constantly "chasing" the error
- Lag effectively reduces system damping
- Can make well-tuned system exhibit hunting

### 5. Vessel-Specific Dynamics

**High Responsiveness:**
- Lightweight vessels with large rudders respond very quickly
- Fast response combined with control lag causes overshoot
- Multihulls particularly susceptible (low drag, quick turning)

**Poor Helm Balance:**
- Excessive weather helm or lee helm
- Requires constant rudder offset
- Integral wind-up can cause slow hunting oscillation

**Speed-Related Response:**
- Vessel response changes dramatically with speed
- Control tuned for one speed hunts at another
- Fixed PID gains inadequate across speed range

## Control Strategies and Their Hunting Characteristics

### Proportional-Only Control

**Algorithm:**
```cpp
rudder_command = Kp × (desired_heading - actual_heading)
```

**Hunting Behavior:**
- **Always exhibits hunting** on course corrections
- Oscillation amplitude depends on Kp value
- Higher Kp = faster response but larger oscillation
- Lower Kp = slower response but smaller oscillation
- **Cannot eliminate hunting**, only minimize it

**When Used:**
- Budget autopilots in protected waters
- Low-power applications (less rudder activity than PID)
- Vessels with excellent natural course stability
- Not recommended for serious offshore use

**Advantages:**
- Simplest implementation
- Lowest computational requirements
- Minimal memory use
- Easiest to understand and tune

**Disadvantages:**
- Persistent hunting behavior
- Cannot compensate for environmental forces
- Poor course accuracy
- Inefficient (excess rudder activity and distance traveled)

### Proportional-Derivative (PD) Control

**Algorithm:**
```cpp
P = Kp × error
D = Kd × (error - previous_error) / dt
rudder_command = P + D
```

**Hunting Behavior:**
- **Derivative term provides damping**, reduces/eliminates hunting
- Properly tuned PD can achieve critically damped response (no overshoot)
- Still cannot eliminate steady-state error from wind/current
- Hunting only occurs if Kd is too low or Kp is too high

**When Used:**
- Situations where steady-state error is acceptable
- When integral wind-up is problematic
- Simple autopilots that need good transient response
- Applications with manually adjustable trim/offset

**Advantages:**
- Good transient response without overshoot
- Simpler than full PID (no wind-up issues)
- Effective damping prevents hunting
- Fast response to course changes

**Disadvantages:**
- Cannot eliminate persistent heading offset
- Requires manual trim adjustment for wind/current
- Derivative term sensitive to noise

### Proportional-Integral-Derivative (PID) Control

**Algorithm:**
```cpp
P = Kp × error
I = Ki × Σ(error × dt)
D = Kd × (error - previous_error) / dt
rudder_command = P + I + D
```

**Hunting Behavior:**
- **Properly tuned PID eliminates hunting** with good course keeping
- Excessive Ki can cause slow-period hunting (integral wind-up)
- Excessive Kp with low Kd causes fast hunting
- Best overall performance when correctly tuned

**When Used:**
- Professional and commercial autopilots
- Offshore cruising requiring precise course keeping
- Variable conditions (wind, current, sea state)
- Modern autopilot systems (standard implementation)

**Advantages:**
- Eliminates both transient oscillation and steady-state error
- Automatically compensates for wind, current, helm imbalance
- Superior performance in varying conditions
- Can be adaptive (gains adjust to conditions)

**Disadvantages:**
- More complex to implement and tune
- Integral wind-up can cause problems if not managed
- Requires careful anti-wind-up measures
- More computational overhead

### Bang-Bang (On-Off) Control

**Algorithm:**
```cpp
if (error > deadband) {
    rudder = max_rudder_right
} else if (error < -deadband) {
    rudder = max_rudder_left
} else {
    rudder = 0
}
```

**Hunting Behavior:**
- **Inherently exhibits hunting** with characteristic zigzag
- Deadband determines oscillation amplitude
- Always oscillates within deadband range
- Aggressive, binary rudder movements

**When Used:**
- Extremely simple DIY autopilots
- Low-accuracy applications
- Systems where simplicity trumps performance
- Educational/demonstration purposes

**Advantages:**
- Trivial to implement (no math required)
- No tuning parameters (only deadband)
- Deterministic behavior
- Works without sensors beyond compass

**Disadvantages:**
- Poor course keeping (always hunting)
- Excessive rudder wear
- Inefficient (high drag from rudder activity)
- Uncomfortable motion
- Not suitable for serious marine use

### Deadband Control (Hysteresis)

**Algorithm:**
```cpp
if (abs(error) < deadband) {
    rudder = 0  // No correction
} else {
    rudder = PID(error)  // Apply control
}
```

**Hunting Behavior:**
- Deadband **reduces hunting** by ignoring small errors
- Creates intentional oscillation within deadband range
- Trade-off: course accuracy vs. rudder activity
- Larger deadband = less hunting but worse course keeping

**When Used:**
- Rough sea conditions (avoid constant corrections)
- Power conservation (reduce rudder activity)
- Downwind sailing (tight course less critical)
- Combination with any control algorithm

**Advantages:**
- Reduces unnecessary rudder activity
- Conserves power
- Prevents response to wave-induced heading changes
- Adjustable precision vs. efficiency trade-off

**Disadvantages:**
- Intentional course error (within deadband)
- May not correct small but persistent errors
- Too large deadband defeats autopilot purpose

## Diagnosing Hunting Behavior

### Observing the Symptoms

**Visual Indicators:**
1. **Wake Pattern**: Serpentine wake instead of straight
2. **Compass**: Heading swings rhythmically ±5-15° around set course
3. **Rudder**: Continuous back-and-forth movement
4. **Track Plot**: GPS track shows sinusoidal pattern

**Quantitative Metrics:**
```
Oscillation Period: Time between successive peaks (seconds)
Amplitude: Maximum deviation from set course (degrees)
Rudder Activity: Rudder reversals per minute
Track Error: Actual distance vs. straight-line distance (%)
```

### Differentiation from Other Issues

**Hunting vs. Normal Correction:**

| Characteristic | Hunting | Normal Operation |
|---------------|---------|------------------|
| Pattern | Continuous oscillation | Correction → settle → stable |
| Rudder Activity | Constant reversals | Intermittent corrections |
| Damping | No decrease over time | Oscillation diminishes |
| Steady-State | Never achieved | Reaches stable heading |

**Hunting vs. Wave-Induced Motion:**

| Characteristic | Hunting | Wave Motion |
|---------------|---------|-------------|
| Periodicity | Regular, autopilot-driven | Irregular, wave period |
| Correlation | Synchronized with rudder | Random, environmental |
| Calm Water | Still exhibits hunting | Minimal motion |
| Frequency | Lower (5-20 sec period) | Higher (2-8 sec period) |

### Diagnostic Test Procedure

**Step 1: Calm Water Test**
- Engage autopilot in flat calm conditions
- Set course and observe for 5-10 minutes
- Hunting present in calm = control problem

**Step 2: Rudder Response Analysis**
- Manually induce 10° heading error
- Observe how autopilot responds:
  - **Good**: Smooth return, slight overshoot, settle
  - **Hunting**: Multiple oscillations, no settling

**Step 3: Gain Sensitivity Test**
- Reduce proportional gain (Kp) by 30%
- If hunting decreases: Kp was too high
- Reduce to minimum that maintains adequate response

**Step 4: Damping Test**
- Increase derivative gain (Kd) by 20%
- If hunting decreases: Kd was too low (insufficient damping)
- Increase until hunting is eliminated

**Step 5: Mechanical Inspection**
- Check for play in steering linkages
- Verify rudder feedback (if equipped)
- Inspect for hydraulic lag or slop
- Mechanical issues compound control problems

## Eliminating Hunting: Tuning Parameters

### Systematic Tuning Approach

**Recommended Tuning Sequence:**

**Phase 1: Establish Proportional Response**
1. Set Ki = 0, Kd = 0 (P-only control)
2. Start with low Kp (e.g., 1.0)
3. Increase Kp until vessel responds adequately
4. Note when sustained oscillation begins (critical gain Ku)
5. Set Kp = 0.5 × Ku (well below critical)

**Phase 2: Add Derivative Damping**
1. Keep Kp at safe value from Phase 1
2. Start with Kd = 0
3. Gradually increase Kd (try 0.1, 0.5, 1.0, 2.0...)
4. Observe damping effect on response
5. Optimal Kd: Approach to heading without overshoot
6. **This step eliminates hunting**

**Phase 3: Add Integral for Steady-State**
1. With Kp and Kd established
2. Start with very small Ki (0.01 - 0.05)
3. Increase slowly until persistent offset is eliminated
4. Watch for slow hunting (sign of excessive Ki)
5. Implement anti-windup if needed

**Phase 4: Fine-Tuning**
1. Test at various speeds
2. Test in different sea states
3. Make small adjustments (10-20% changes)
4. Document final parameters

### Parameter Effects on Hunting

**Proportional Gain (Kp) - "Rudder Response"**

| Setting | Effect on Hunting | Course Keeping |
|---------|-------------------|----------------|
| Too Low | No hunting, but slow, sluggish response | Poor (large errors persist) |
| Optimal | No hunting with adequate response | Good |
| Too High | **Causes hunting** - oscillation begins | Varies (may be tight but oscillating) |

**Adjustment Rule:**
- If hunting: Reduce Kp by 20-30%
- If sluggish: Increase Kp by 10-20%
- Iterate until balanced

**Derivative Gain (Kd) - "Counter Rudder/Damping"**

| Setting | Effect on Hunting | Overshoot Behavior |
|---------|-------------------|-------------------|
| Too Low/Zero | **Causes hunting** - no damping | Large overshoot, oscillation |
| Optimal | **Eliminates hunting** - critically damped | Minimal or no overshoot |
| Too High | No hunting, but sluggish, unresponsive | No overshoot, slow approach |

**Adjustment Rule:**
- If hunting: Increase Kd by 20-50%
- If too slow: Decrease Kd by 10-20%
- Derivative is key to preventing hunting

**Integral Gain (Ki) - "Auto Trim/Permanent Helm"**

| Setting | Effect on Hunting | Steady-State Error |
|---------|-------------------|-------------------|
| Too Low/Zero | No hunting from Ki, but offset persists | Persistent heading offset |
| Optimal | No hunting, offset eliminated | Zero steady-state error |
| Too High | **Slow hunting** - wind-up oscillation | Eventually eliminated but oscillates |

**Adjustment Rule:**
- If slow hunting (long period): Reduce Ki by 50%
- If persistent offset: Increase Ki by small increments
- Always implement anti-windup

### Manufacturer Parameter Equivalents

Different manufacturers use various names for PID terms:

| Manufacturer | Proportional (Kp) | Derivative (Kd) | Integral (Ki) |
|-------------|-------------------|-----------------|---------------|
| **Generic PID** | P Gain | D Gain | I Gain |
| **Raymarine** | Rudder Gain | Counter Rudder | Auto Trim |
| **Garmin** | Rudder Response | Rudder Damping | Auto Trim |
| **Simrad** | Rudder | Counter Rudder | Automatic Trim |
| **Furuno** | Rudder (Gain) | Counter Rudder | Weather (Trim) |

**Universal Tuning Guidance:**
- **"Rudder Gain"** - Reduces hunting if decreased
- **"Counter Rudder"** - Increases damping if increased (eliminates hunting)
- **"Auto Trim"** - Slow hunting if too high

### Deadband Configuration

**Deadband Parameter:**

```cpp
if (abs(heading_error) < deadband) {
    // No rudder correction
    return;
}
```

**Typical Settings:**
- **Calm water / precision navigation**: 1-2° deadband
- **Moderate seas / cruising**: 3-5° deadband
- **Rough weather / power conservation**: 5-10° deadband
- **Downwind sailing**: 5-15° deadband (less critical)

**Effect on Hunting:**
- Larger deadband **reduces apparent hunting** (ignores small oscillations)
- Does NOT fix underlying control problem
- Trade course accuracy for reduced rudder activity
- Useful for mitigating wave-induced motion

### Anti-Windup for Integral Term

Integral wind-up can cause slow-period hunting. Prevent with:

**Method 1: Conditional Integration**
```cpp
if (rudder_at_limit && same_sign(error, integral)) {
    // Don't accumulate integral
} else {
    integral += error * dt;
}
```

**Method 2: Integral Clamping**
```cpp
integral += error * dt;
integral = constrain(integral, -max_integral, max_integral);
```

**Method 3: Back-Calculation**
```cpp
if (rudder > max_rudder) {
    integral -= (rudder - max_rudder) / Ki;  // Reduce integral
    rudder = max_rudder;
}
```

## Implementation for Arduino/DIY Autopilots

### PID Implementation to Prevent Hunting

**Complete Arduino PID Controller with Anti-Hunting Measures:**

```cpp
// PID Controller Class for Marine Autopilot
class AutopilotPID {
private:
    // PID Gains
    float Kp = 2.0;   // Proportional gain
    float Ki = 0.1;   // Integral gain
    float Kd = 5.0;   // Derivative gain (critical for damping)

    // State variables
    float integral = 0.0;
    float previous_error = 0.0;
    float filtered_derivative = 0.0;

    // Configuration
    float deadband = 2.0;           // Degrees
    float integral_limit = 20.0;    // Anti-windup
    float rudder_limit = 35.0;      // Degrees
    float dt = 0.1;                 // 100ms update rate
    float derivative_filter = 0.2;  // Low-pass filter coefficient

    // Normalize angle to -180 to +180
    float normalizeAngle(float angle) {
        while (angle > 180.0) angle -= 360.0;
        while (angle < -180.0) angle += 360.0;
        return angle;
    }

public:
    // Main control function
    float compute(float desired_heading, float actual_heading) {
        // Calculate heading error with wraparound
        float error = normalizeAngle(desired_heading - actual_heading);

        // Deadband: ignore small errors
        if (abs(error) < deadband) {
            // Reset integral in deadband to prevent wind-up
            integral *= 0.9;  // Gradual decay
            return 0.0;
        }

        // Proportional term
        float P = Kp * error;

        // Integral term with anti-windup
        integral += error * dt;
        integral = constrain(integral, -integral_limit, integral_limit);
        float I = Ki * integral;

        // Derivative term with filtering (key to preventing hunting)
        float raw_derivative = (error - previous_error) / dt;
        filtered_derivative = derivative_filter * raw_derivative +
                             (1.0 - derivative_filter) * filtered_derivative;
        float D = Kd * filtered_derivative;

        // PID output
        float rudder_command = P + I + D;

        // Apply rudder limits
        rudder_command = constrain(rudder_command, -rudder_limit, rudder_limit);

        // Anti-windup: prevent integral accumulation at saturation
        if (abs(rudder_command) >= rudder_limit) {
            if ((error > 0 && integral > 0) || (error < 0 && integral < 0)) {
                integral -= error * dt;  // Undo last accumulation
            }
        }

        // Update state
        previous_error = error;

        return rudder_command;
    }

    // Tuning functions
    void setGains(float p, float i, float d) {
        Kp = p;
        Ki = i;
        Kd = d;
    }

    void setDeadband(float db) {
        deadband = db;
    }

    void reset() {
        integral = 0.0;
        previous_error = 0.0;
        filtered_derivative = 0.0;
    }
};
```

### Auto-Tuning Implementation

**Ziegler-Nichols Method (Arduino Implementation):**

```cpp
class AutoTuner {
private:
    float Ku = 0.0;      // Critical gain (where oscillation starts)
    float Pu = 0.0;      // Oscillation period
    bool tuning = false;

public:
    // Step 1: Find critical gain (Ku)
    void findCriticalGain() {
        // Disable I and D, vary P until sustained oscillation
        // Start with low Kp, increment by 0.5
        // Monitor for oscillation using FFT or zero-crossing detection
        // Record Ku when oscillation amplitude stabilizes
    }

    // Step 2: Measure oscillation period
    void measurePeriod() {
        // Count time between successive heading peaks
        // Average over several cycles
        // Record as Pu
    }

    // Step 3: Calculate PID gains
    void calculateGains(float &Kp, float &Ki, float &Kd) {
        // Ziegler-Nichols PID formula
        Kp = 0.6 * Ku;
        Ki = 2.0 * Kp / Pu;
        Kd = Kp * Pu / 8.0;
    }
};
```

### Adaptive Gain Scheduling

**Speed-Based Gain Adjustment:**

```cpp
void updateGains(float vessel_speed) {
    const float reference_speed = 5.0;  // knots
    float speed_factor = reference_speed / vessel_speed;

    // At higher speeds, reduce gains (vessel more responsive)
    Kp_effective = Kp_base * speed_factor;
    Kd_effective = Kd_base * speed_factor;

    // Limit factor range
    Kp_effective = constrain(Kp_effective, Kp_base * 0.5, Kp_base * 2.0);
    Kd_effective = constrain(Kd_effective, Kd_base * 0.5, Kd_base * 2.0);
}
```

**Sea State Adaptive:**

```cpp
void adjustForSeaState(float heading_variance) {
    // Higher variance = rougher seas
    if (heading_variance > 10.0) {
        // Rough: increase damping, reduce gain
        Kd_effective = Kd_base * 1.5;
        Kp_effective = Kp_base * 0.7;
        deadband = 5.0;
    } else if (heading_variance < 3.0) {
        // Calm: standard gains, tight tolerance
        Kd_effective = Kd_base;
        Kp_effective = Kp_base;
        deadband = 2.0;
    }
}
```

### Wave Filtering to Prevent False Corrections

**Low-Pass Filter for Heading:**

```cpp
class HeadingFilter {
private:
    float alpha = 0.15;  // Filter coefficient (0-1)
    float filtered_heading = 0.0;
    bool initialized = false;

public:
    float filter(float raw_heading) {
        if (!initialized) {
            filtered_heading = raw_heading;
            initialized = true;
            return raw_heading;
        }

        // Handle angle wraparound
        float delta = raw_heading - filtered_heading;
        if (delta > 180.0) delta -= 360.0;
        if (delta < -180.0) delta += 360.0;

        // Apply low-pass filter
        filtered_heading += alpha * delta;

        // Normalize
        while (filtered_heading >= 360.0) filtered_heading -= 360.0;
        while (filtered_heading < 0.0) filtered_heading += 360.0;

        return filtered_heading;
    }
};
```

### Complete Autopilot Loop

**Main Control Loop with Anti-Hunting Measures:**

```cpp
// Global objects
AutopilotPID pid;
HeadingFilter heading_filter;
BNO055 compass;
Servo rudder_servo;

// Tuned parameters (example for 30ft sailboat)
const float KP = 1.8;
const float KI = 0.08;
const float KD = 4.5;
const float DEADBAND = 2.5;  // degrees

void setup() {
    Serial.begin(115200);

    // Initialize compass
    compass.begin();
    compass.setMode(OPERATION_MODE_NDOF);

    // Initialize rudder servo
    rudder_servo.attach(RUDDER_PIN);

    // Configure PID
    pid.setGains(KP, KI, KD);
    pid.setDeadband(DEADBAND);

    Serial.println("Autopilot initialized - Anti-hunting PID active");
}

void loop() {
    static unsigned long last_update = 0;
    unsigned long now = millis();

    // 100ms update rate (10 Hz)
    if (now - last_update >= 100) {
        last_update = now;

        // Read compass heading
        sensors_event_t event;
        compass.getEvent(&event);
        float raw_heading = event.orientation.x;

        // Apply wave filtering
        float actual_heading = heading_filter.filter(raw_heading);

        // Get desired heading (from user input, GPS, etc.)
        float desired_heading = getDesiredHeading();

        // Compute PID control (with anti-hunting)
        float rudder_angle = pid.compute(desired_heading, actual_heading);

        // Apply to rudder servo
        int servo_position = map(rudder_angle, -35, 35, 0, 180);
        servo_position = constrain(servo_position, 0, 180);
        rudder_servo.write(servo_position);

        // Diagnostics
        logTelemetry(desired_heading, actual_heading, rudder_angle);
    }
}

void logTelemetry(float desired, float actual, float rudder) {
    float error = desired - actual;
    if (error > 180.0) error -= 360.0;
    if (error < -180.0) error += 360.0;

    Serial.print("Desired: "); Serial.print(desired, 1);
    Serial.print(" | Actual: "); Serial.print(actual, 1);
    Serial.print(" | Error: "); Serial.print(error, 1);
    Serial.print(" | Rudder: "); Serial.println(rudder, 1);
}
```

## Sea State and Operating Conditions

### Conditions Where Hunting is Most Problematic

**1. Calm Seas, Light Winds**
- Hunting is most visible and problematic
- No environmental excuses for poor course keeping
- Excessive rudder activity clearly wasteful
- **Solution**: Tight tuning, small deadband (1-2°)

**2. Beam Seas (Waves from Side)**
- Waves cause rolling, which affects compass heading
- Risk of autopilot responding to wave-induced heel
- Can cause rhythmic hunting synchronized with wave period
- **Solution**: Wave filtering, increased deadband (3-5°)

**3. Following Seas (Waves from Behind)**
- Waves lift stern, causing yaw acceleration
- Vessel may surf, speed varies dramatically
- Autopilot struggles with rapid heading changes
- **Solution**: Adaptive gains based on speed, higher damping

**4. Downwind Sailing**
- Vessel rolling significantly (death roll risk)
- Less critical course precision needed
- Power conservation important
- **Solution**: Large deadband (10-15°), relaxed gains

### Operating Condition Tuning Profiles

**Profile 1: Upwind/Close-Hauled (Tight Course Required)**
```cpp
Kp = 2.0;     // Responsive
Ki = 0.1;     // Eliminate offset
Kd = 6.0;     // High damping
deadband = 1.5;  // Tight tolerance
```

**Profile 2: Reaching (Moderate Precision)**
```cpp
Kp = 1.5;     // Moderate response
Ki = 0.08;    // Some trim
Kd = 4.0;     // Moderate damping
deadband = 3.0;  // Relaxed tolerance
```

**Profile 3: Downwind/Running (Power Conservation)**
```cpp
Kp = 1.0;     // Gentle
Ki = 0.05;    // Minimal integral
Kd = 3.0;     // Light damping
deadband = 8.0;  // Wide tolerance
```

**Profile 4: Rough Weather (Stability Priority)**
```cpp
Kp = 1.2;     // Reduced to avoid chasing waves
Ki = 0.0;     // Disabled (manual trim)
Kd = 8.0;     // Maximum damping
deadband = 5.0;  // Ignore wave motion
```

### Environmental Adaptation

**Automatic Sea State Detection:**

```cpp
float detectSeaState() {
    // Calculate heading variance over 30 seconds
    float variance = calculateHeadingVariance(30);

    if (variance < 2.0) return SEA_CALM;
    else if (variance < 5.0) return SEA_MODERATE;
    else if (variance < 10.0) return SEA_ROUGH;
    else return SEA_VERY_ROUGH;
}

void adaptToSeaState(int sea_state) {
    switch(sea_state) {
        case SEA_CALM:
            pid.setGains(2.0, 0.1, 5.0);
            pid.setDeadband(2.0);
            break;
        case SEA_MODERATE:
            pid.setGains(1.5, 0.08, 6.0);
            pid.setDeadband(3.0);
            break;
        case SEA_ROUGH:
            pid.setGains(1.2, 0.05, 8.0);
            pid.setDeadband(5.0);
            break;
        case SEA_VERY_ROUGH:
            pid.setGains(1.0, 0.0, 10.0);
            pid.setDeadband(8.0);
            break;
    }
}
```

## Power and Fuel Efficiency Implications

### Impact of Hunting on Energy Consumption

**Hunting Increases Power Consumption:**

1. **Excessive Rudder Activity**
   - Continuous motor/pump operation
   - No rest periods between corrections
   - Peak current draw more frequent
   - Battery drain 2-3× normal

2. **Increased Drag**
   - Rudder constantly deflected from centerline
   - Extra resistance slows vessel
   - Requires more propulsion power (motorsailing/motoring)

3. **Longer Distance Traveled**
   - Zigzag path is longer than straight line
   - Extra distance = more time under autopilot load
   - Compound effect on total energy

**Quantitative Impact:**

| Autopilot Behavior | Rudder Activity | Power Consumption | Distance Penalty |
|-------------------|-----------------|-------------------|------------------|
| **Well-Tuned (No Hunting)** | Intermittent corrections | 1.5-2.5A average | 0-2% extra distance |
| **Moderate Hunting** | Continuous oscillation | 3-5A average | 5-10% extra distance |
| **Severe Hunting** | Constant reversals | 5-8A average | 15-25% extra distance |

**Example Calculation (24-hour passage):**

```
Well-tuned autopilot:
- 24 hours × 2A average = 48 Ah
- Distance: 120 nautical miles

Hunting autopilot:
- 24 hours × 5A average = 120 Ah (2.5× more power)
- Distance: 135 nautical miles (12.5% longer)
- Fuel/motor hours also increased 12.5%
```

### Fuel Efficiency in Motor Vessels

**Commercial Vessel Studies:**
- Effective autopilot use: 0.5-4% fuel savings vs. manual steering
- Poor autopilot tuning (hunting): 1-3% fuel penalty vs. manual steering
- Difference between optimized and hunting: **Up to 7% fuel consumption**

**Why Hunting Wastes Fuel:**
1. Rudder deflection creates drag (resistance ∝ rudder_angle²)
2. Continuous rudder movement = continuous drag
3. Longer track distance = more engine hours
4. Vessel slowed by rudder drag = more throttle to maintain speed

**Optimal Tuning for Efficiency:**
- Minimize rudder activity while maintaining course
- Use economy/auto mode (not performance mode)
- Larger deadband in non-critical navigation
- Trim sails to balance helm (reduce autopilot workload)

### Power Conservation Strategies

**1. Eliminate Hunting (Primary Savings)**
- Proper PID tuning is #1 power saver
- Well-damped system uses 50-70% less power than hunting system
- One-time investment in tuning pays off every voyage

**2. Deadband Optimization**
```cpp
// Adaptive deadband for efficiency
if (navigation_critical) {
    deadband = 2.0;  // Precision required
} else {
    deadband = 5.0;  // Save power
}
```

**3. Differential Gain Adjustment**
```cpp
// Reduce Kp slightly, increase Kd proportionally
// Result: Same damping, less aggressive, lower power
Kp = Kp_base * 0.8;
Kd = Kd_base * 1.3;
```

**4. Update Rate Reduction** (for very low power systems)
```cpp
// Reduce control loop frequency in calm conditions
if (sea_state == CALM && abs(heading_error) < 5.0) {
    loop_rate = 5 Hz;   // Instead of 10 Hz
} else {
    loop_rate = 10 Hz;  // Normal rate
}
```

**5. Sleep Mode When Stable**
```cpp
if (abs(heading_error) < 1.0 && stable_for > 60) {
    // Heading stable for 60 seconds
    enter_low_power_mode();  // Reduce sensor polling
    rudder_servo.detach();   // Release servo (no holding current)
}
```

## Comparison to Other Control Issues

### Hunting vs. Overshoot

**Hunting:**
- Continuous oscillation around setpoint
- Does not converge
- Symmetric (equal magnitude both directions)
- Caused by lack of damping

**Overshoot:**
- Single excursion past setpoint
- Converges (system settles)
- Asymmetric (one-time event)
- Caused by excessive gain or insufficient damping
- Acceptable if small (< 5°) and settles quickly

### Hunting vs. Steady-State Error

**Hunting:**
- Dynamic problem (oscillation)
- Error average is near zero
- Vessel never settles on course

**Steady-State Error:**
- Static problem (constant offset)
- Vessel settles but at wrong heading
- No oscillation
- Caused by lack of integral term or external bias

**Example:**
```
Hunting: Heading oscillates 175° - 185° around 180° setpoint
Steady-State Error: Heading settles at 178° instead of 180° setpoint
```

### Hunting vs. Instability

**Hunting (Bounded Oscillation):**
- Oscillation amplitude constant
- System remains controllable
- Annoying but not dangerous

**Instability (Growing Oscillation):**
- Oscillation amplitude increases over time
- System diverges toward uncontrollable state
- Potentially dangerous (hard-over rudder)
- Caused by Kp exceeding critical gain (Ku)

**Warning Signs of Instability:**
```
Cycle 1: ±5° oscillation
Cycle 2: ±8° oscillation
Cycle 3: ±12° oscillation
→ INSTABILITY - reduce Kp immediately
```

### Hunting vs. Derivative Kick

**Hunting:**
- Continuous, repetitive
- Occurs during steady-state operation
- Related to proportional gain and damping

**Derivative Kick:**
- Single sharp spike in rudder command
- Occurs when setpoint changes (user adjusts course)
- Caused by differentiating the error (includes setpoint change)

**Prevention:**
```cpp
// Calculate derivative on process variable only
D = Kd × (actual_heading - previous_heading) / dt;
// NOT: D = Kd × (error - previous_error) / dt;
```

### Summary Table: Control Problems

| Problem | Pattern | Primary Cause | Solution |
|---------|---------|---------------|----------|
| **Hunting** | Continuous oscillation | Insufficient damping (low Kd) or high Kp | Increase Kd, reduce Kp |
| **Overshoot** | One-time excursion | Excessive gain, low damping | Reduce Kp, increase Kd |
| **Steady-State Error** | Constant offset | No integral term | Add Ki term |
| **Instability** | Growing oscillation | Kp > critical gain | Immediately reduce Kp |
| **Derivative Kick** | Spike on setpoint change | Derivative of error | Derive process variable only |
| **Integral Windup** | Slow, large overshoot | Integral saturation | Anti-windup algorithm |
| **Sluggish Response** | Slow, lethargic | Low Kp, excessive Kd | Increase Kp, reduce Kd |

## Conclusion

**Hunting is not a control technique—it is a control problem to be eliminated.** Proper autopilot design and tuning prevents hunting through:

1. **Use of PID control** (not proportional-only)
2. **Adequate derivative gain (Kd)** for damping
3. **Appropriate proportional gain (Kp)** below critical threshold
4. **Careful integral tuning (Ki)** with anti-windup
5. **Deadband for wave filtering** (not as a hunting "solution")
6. **Adaptive gains** for varying conditions
7. **Systematic tuning methodology**

For Arduino/DIY autopilot implementations:
- Start with proven PID algorithm structure
- Implement anti-windup and derivative filtering
- Use systematic tuning (Ziegler-Nichols or manual method)
- Test thoroughly in calm conditions before offshore use
- Consider adaptive gain scheduling for varying sea states
- Prioritize power efficiency through proper damping

**The result of eliminating hunting:**
- Precise course keeping
- Minimal rudder activity
- Optimal power/fuel efficiency
- Comfortable vessel motion
- Extended equipment life
- Professional-grade autopilot performance

---

*This document is part of the AutoTiller5000 documentation. For PID implementation details, see [autotillers.md](./autotillers.md). For Arduino coding patterns, see [arduino-programming.md](./arduino-programming.md) and [arduino-reliability.md](./arduino-reliability.md).*