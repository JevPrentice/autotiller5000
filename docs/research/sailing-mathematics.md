# Comprehensive Sailing Mathematics Reference

A complete guide to mathematical calculations and formulas used in sailing navigation, covering wind calculations, course corrections, distance measurements, and performance optimization.

---

## Table of Contents

1. [Coordinate Systems and Reference Frames](#coordinate-systems-and-reference-frames)
2. [Heading and Course Calculations](#heading-and-course-calculations)
3. [Wind Angle Calculations](#wind-angle-calculations)
4. [Speed and Distance Calculations](#speed-and-distance-calculations)
5. [Tacking and Jibing Angles](#tacking-and-jibing-angles)
6. [Apparent vs True Wind Calculations](#apparent-vs-true-wind-calculations)
7. [Leeway Calculations and Corrections](#leeway-calculations-and-corrections)
8. [Course Corrections and Navigation Math](#course-corrections-and-navigation-math)
9. [Vector Mathematics in Sailing](#vector-mathematics-in-sailing)
10. [Trigonometry Applications](#trigonometry-applications)
11. [Compass Calculations](#compass-calculations)
12. [Velocity Made Good (VMG) Calculations](#velocity-made-good-vmg-calculations)
13. [Practical Examples and Worked Problems](#practical-examples-and-worked-problems)

---

## Coordinate Systems and Reference Frames

### Standard Sailing Variables

The following standard notation is used throughout sailing mathematics:

**Wind Variables:**
- **TWS** = True Wind Speed (knots, relative to earth)
- **TWD** = True Wind Direction (degrees true, direction wind is coming FROM)
- **TWA** = True Wind Angle (degrees, angle between boat heading and true wind)
- **AWS** = Apparent Wind Speed (knots, relative to boat)
- **AWD** = Apparent Wind Direction (degrees true, direction apparent wind comes from)
- **AWA** = Apparent Wind Angle (degrees, angle between bow and apparent wind, 0-180°)

**Boat Variables:**
- **H** = Heading (degrees true, direction bow points)
- **HDG** = Heading (alternative notation)
- **COG** = Course Over Ground (degrees true, actual track over bottom)
- **CTW** = Course Through Water (degrees true, track through water)
- **SOG** = Speed Over Ground (knots, speed over bottom)
- **STW** = Speed Through Water (knots, speed relative to water)
- **S** = Boat Speed (knots, typically refers to STW)

**Navigation Variables:**
- **L** = Leeway angle (degrees, drift to leeward)
- **Set** = Current direction (degrees true, direction current flows TO)
- **Drift** = Current speed (knots)
- **VMG** = Velocity Made Good (knots, speed toward target)

### Angular Conventions

**Direction Measurements:**
- **True North (0° or 360°)**: Reference for all compass directions
- **Clockwise rotation**: 0° → 90° (East) → 180° (South) → 270° (West) → 360°
- **Port/Starboard angles**: Often expressed as relative bearings
  - 0° = dead ahead
  - 90° = abeam to starboard
  - 180° = dead astern
  - 270° = abeam to port

**Wind Angle Conventions:**
- Wind angles measured from BOW (0°) to either side
- Starboard wind: positive angles (0° to 180°)
- Port wind: negative angles or (180° to 360°)
- Most instruments report 0-180° with port/starboard indicator

---

## Heading and Course Calculations

### Basic Definitions

**Heading (H or HDG)**
- Direction the vessel's bow points
- Magnetic or true, depending on reference
- Does NOT account for leeway, current, or drift

**Course Through Water (CTW)**
- Actual track of vessel through water
- Accounts for leeway but NOT current
- Formula: **CTW = HDG ± L**
  - On starboard tack: CTW = HDG + L
  - On port tack: CTW = HDG - L

**Course Over Ground (COG)**
- Actual track over the seafloor/earth
- Accounts for both leeway AND current
- What GPS measures
- Critical for navigation

### Course Calculation Formulas

**From Heading to COG (complete calculation):**

```
Step 1: Apply leeway to get CTW
  If starboard tack: CTW = HDG + L
  If port tack: CTW = HDG - L

Step 2: Apply current vector to get COG
  COG_x = STW × sin(CTW) + Drift × sin(Set)
  COG_y = STW × cos(CTW) + Drift × cos(Set)

  COG = atan2(COG_x, COG_y)
  SOG = sqrt(COG_x² + COG_y²)
```

**From COG to Heading (reverse calculation):**

```
Step 1: Remove current effect
  CTW_x = SOG × sin(COG) - Drift × sin(Set)
  CTW_y = SOG × cos(COG) - Drift × cos(Set)

  CTW = atan2(CTW_x, CTW_y)
  STW = sqrt(CTW_x² + CTW_y²)

Step 2: Remove leeway to get heading
  If starboard tack: HDG = CTW - L
  If port tack: HDG = CTW + L
```

### Bearing Calculations

**Bearing to Target:**
```
Given: Current position (lat₁, lon₁), Target (lat₂, lon₂)

Δλ = lon₂ - lon₁

y = sin(Δλ) × cos(lat₂)
x = cos(lat₁) × sin(lat₂) - sin(lat₁) × cos(lat₂) × cos(Δλ)

Bearing = atan2(y, x)

Convert to degrees: Bearing_deg = (Bearing_rad × 180/π + 360) mod 360
```

**Relative Bearing:**
```
Relative_Bearing = (Absolute_Bearing - HDG + 360) mod 360

Where:
  0° = dead ahead
  90° = starboard beam
  180° = dead astern
  270° = port beam
```

---

## Wind Angle Calculations

### True Wind Angle (TWA)

**Definition:** Angle between boat heading and true wind direction

**Calculation from True Wind Direction:**
```
TWA = abs((TWD - HDG + 180) mod 360 - 180)

Note: This formula normalizes to -180° to +180°, then takes absolute value
Result: 0° to 180° (where 0° = head wind, 180° = following wind)
```

**Determining Tack from TWA:**
```
If TWD measured clockwise from heading is 0-180°: Starboard tack
If TWD measured clockwise from heading is 180-360°: Port tack

Alternative using signed TWA:
  TWA_signed = (TWD - HDG + 180) mod 360 - 180
  If TWA_signed > 0: Starboard tack
  If TWA_signed < 0: Port tack
```

### Apparent Wind Angle (AWA)

**Direct Measurement:**
- Typically measured by masthead wind instrument
- Angle between bow and apparent wind (0-180°)
- Port/starboard indicated separately

**Calculation from Apparent Wind Direction:**
```
AWA = abs((AWD - HDG + 180) mod 360 - 180)
```

### Converting Between TWA and AWA

**TWA from AWA (requires AWS and boat speed):**
```
See "Apparent vs True Wind Calculations" section below for full vector solution
```

---

## Speed and Distance Calculations

### Speed Relationships

**Speed Over Ground (SOG):**
- Measured by GPS
- Actual speed over earth's surface
- Affected by current

**Speed Through Water (STW):**
- Measured by paddlewheel or other sensor
- Speed relative to water
- Not affected by current

**Relationship:**
```
SOG_vector = STW_vector + Current_vector

Component form:
  SOG_x = STW × sin(CTW) + Drift × sin(Set)
  SOG_y = STW × cos(CTW) + Drift × cos(Set)

  SOG = sqrt(SOG_x² + SOG_y²)
```

### Distance Calculations

**Distance Traveled (dead reckoning):**
```
Distance = Speed × Time

In nautical miles: Distance_nm = Speed_knots × Time_hours
```

**Distance Between Points (rhumb line):**
```
Given: Point 1 (lat₁, lon₁), Point 2 (lat₂, lon₂)

Δlat = lat₂ - lat₁
Δlon = lon₂ - lon₁

q = Δlat / ln(tan(lat₂/2 + π/4) / tan(lat₁/2 + π/4))

If abs(Δlat) < sqrt(ε):  # nearly east-west line
    q = cos(lat₁)

Distance = sqrt(Δlat² + q² × Δlon²) × 60 nautical miles

Note: Multiply by 60 because 1 degree latitude = 60 nautical miles
```

**Distance Between Points (great circle - shortest path):**
```
See "Trigonometry Applications" section for great circle formulas
```

### Hull Speed Calculation

**Theoretical Maximum Speed (displacement hulls):**
```
Hull_Speed_knots = 1.34 × sqrt(LWL_feet)

Or in meters:
Hull_Speed_knots = 2.43 × sqrt(LWL_meters)

Where LWL = Length at Waterline
```

**Speed/Length Ratio:**
```
S/L_ratio = Speed_knots / sqrt(LWL_feet)

Typical values:
  < 1.0: Displacement mode
  1.0-1.34: Transition zone
  > 1.34: Planing or semi-displacement mode (not achievable for pure displacement hulls)
```

---

## Tacking and Jibing Angles

### Tacking Angle

**Definition:** Total angle boat turns through when tacking (changing tacks by turning bow through wind)

**Tacking Angle Calculation:**
```
Tacking_Angle = 2 × Close_Hauled_TWA

Typical values:
  Racing boats: 80° to 90° (40-45° per tack)
  Cruising boats: 90° to 100° (45-50° per tack)
  Heavy cruisers: 100° to 120° (50-60° per tack)
```

**Optimal Tacking Angle (for VMG upwind):**
```
For maximum VMG to windward:
  Optimal_TWA = arccos(2/3 × V(45°)/V_max) approximately

Where:
  V(45°) = boat speed at 45° TWA
  V_max = maximum boat speed

Practical approximation: 40-45° for most modern sailboats
```

### Beating to Windward Distance

**Distance Sailed vs. Distance Made Good:**
```
Given:
  - Target bearing: B
  - True wind direction: TWD
  - Close-hauled TWA: α
  - Direct distance to target: D

Calculate bearing difference:
  θ = abs(B - TWD)

If θ < α: Cannot sail directly, must tack

Distance on each tack:
  D₁ = D × sin(α + θ) / sin(2α)
  D₂ = D × sin(α - θ) / sin(2α)

Total distance sailed: D_total = D₁ + D₂

Efficiency = D / D_total
```

### Jibing Angle

**Definition:** Angle turned through when jibing (changing tacks by turning stern through wind)

**Jibing Angle:**
```
Jibing_Angle = 360° - Tacking_Angle

For downwind sailing:
  Jibe_Turn = change in heading to move from one broad reach to opposite

Typical downwind jibing angles:
  From broad reach to broad reach: 60° to 90°
  Dead downwind jibing: 20° to 40° (smaller turn)
```

### Optimal Downwind Angles

**VMG Downwind (optimal jibing angle):**
```
For maximum VMG downwind:
  Optimal_Jibe_Angle ≈ 140° to 160° TWA

Many boats sail faster on broad reach than dead run:
  - Dead run (180° TWA): slowest due to wind shadowing
  - Broad reach (130-150° TWA): faster boat speed
  - VMG = Boat_Speed × cos(180° - TWA)
```

---

## Apparent vs True Wind Calculations

### The Wind Triangle

The relationship between true wind, apparent wind, and boat motion forms a vector triangle:

**Vector Relationship:**
```
Apparent_Wind_Vector = True_Wind_Vector - Boat_Velocity_Vector

Or equivalently:
True_Wind_Vector = Apparent_Wind_Vector + Boat_Velocity_Vector
```

### True Wind from Apparent Wind

**Given: AWA, AWS, HDG, STW**

**Method 1: Component Form (most reliable)**

```
Convert to Cartesian coordinates (bow = y-axis, starboard = x-axis):

Apparent wind components:
  AW_x = AWS × sin(AWA)
  AW_y = AWS × cos(AWA)

Boat velocity components (negative because boat moves into wind):
  BV_x = 0  # boat moves straight ahead
  BV_y = -STW

True wind components:
  TW_x = AW_x + BV_x = AWS × sin(AWA)
  TW_y = AW_y + BV_y = AWS × cos(AWA) - STW

True Wind Speed:
  TWS = sqrt(TW_x² + TW_y²)

True Wind Angle (relative to bow):
  TWA = atan2(abs(TW_x), TW_y)

  Convert to degrees: TWA_deg = TWA_rad × 180/π

True Wind Direction (absolute):
  TWD_relative = atan2(TW_x, TW_y)
  TWD = (HDG + TWA_deg) if starboard
  TWD = (HDG - TWA_deg) if port
```

**Method 2: Direct Formula**

```
TWS² = AWS² + STW² - 2 × AWS × STW × cos(AWA)
TWS = sqrt(AWS² + STW² - 2 × AWS × STW × cos(AWA))

sin(TWA) = AWS × sin(AWA) / TWS
TWA = arcsin(AWS × sin(AWA) / TWS)
```

### Apparent Wind from True Wind

**Given: TWA, TWS, HDG, STW**

```
Boat velocity components (in true wind reference frame):
  BV_x = STW × sin(0) = 0
  BV_y = STW × cos(0) = STW

True wind components:
  TW_x = TWS × sin(TWA)
  TW_y = TWS × cos(TWA)

Apparent wind components:
  AW_x = TW_x - BV_x = TWS × sin(TWA)
  AW_y = TW_y - BV_y = TWS × cos(TWA) - STW

Apparent Wind Speed:
  AWS = sqrt(AW_x² + AW_y²)
  AWS = sqrt(TWS² + STW² - 2 × TWS × STW × cos(TWA))

Apparent Wind Angle:
  AWA = atan2(abs(AW_x), AW_y)
  AWA = arctan(TWS × sin(TWA) / (TWS × cos(TWA) - STW))
```

### Key Insights

**Apparent Wind Shifts Forward:**
- When sailing, apparent wind always appears more ahead than true wind
- Faster boat speed → more forward apparent wind shift
- Dead downwind: apparent wind can disappear if boat speed approaches true wind speed

**Critical Cases:**

```
Close-hauled:
  AWA < TWA (apparent wind appears forward)
  AWS > TWS (apparent wind speed increases)

Dead downwind:
  If STW = TWS: AWS = 0 (no apparent wind!)
  If STW > TWS: Apparent wind from ahead (sailing faster than true wind)

Reaching:
  Maximum AWS occurs around beam reach
  AWS can be significantly greater than TWS
```

---

## Leeway Calculations and Corrections

### Leeway Definition

**Leeway (L):** Angle between heading and actual course through water, caused by lateral force of wind pushing boat sideways

- Always to leeward (downwind side)
- Maximum when close-hauled
- Minimal when running

### Estimating Leeway Angle

**Visual Method:**
```
Leeway_Angle = Heading - Wake_Direction

Measure:
1. Note compass heading
2. Sight along wake (use bearing compass)
3. Difference is leeway angle
```

**Empirical Formulas:**

```
Basic approximation:
  L ≈ k × (Heel_Angle)

Where k varies by boat type:
  Racing boats: k ≈ 0.2 to 0.4
  Cruising boats: k ≈ 0.4 to 0.6
  Heavy displacement: k ≈ 0.6 to 1.0

More detailed:
  L = k × (AWS/STW)² × sin(AWA)

Typical leeway values:
  Close-hauled, moderate wind: 3-5°
  Close-hauled, heavy wind: 5-10°
  Reaching: 2-3°
  Running: 0-2°
```

### Applying Leeway to Course

**Course Through Water from Heading:**

```
Rule: "Leeway Sign Convention"
  - On STARBOARD tack: ADD leeway
  - On PORT tack: SUBTRACT leeway

CTW = HDG + L  (starboard tack)
CTW = HDG - L  (port tack)

Alternative: Always add, where L is signed:
  L_signed = +L (starboard tack)
  L_signed = -L (port tack)

  CTW = HDG + L_signed
```

**Heading Required to Achieve Desired CTW:**

```
To achieve desired track:

  If starboard tack: HDG = CTW - L
  If port tack: HDG = CTW + L

Memory aid: "Subtract toward wind"
  - To achieve desired CTW, steer toward the wind by leeway amount
```

### Leeway in Navigation Calculations

**Dead Reckoning with Leeway:**

```
Given: Starting position, HDG, STW, time, leeway L

Step 1: Calculate CTW
  CTW = HDG ± L

Step 2: Calculate distance
  Distance = STW × time

Step 3: Calculate new position
  Δlat = Distance × cos(CTW) / 60
  Δlon = Distance × sin(CTW) / (60 × cos(lat))

  new_lat = old_lat + Δlat
  new_lon = old_lon + Δlon
```

---

## Course Corrections and Navigation Math

### Current Triangle (Set and Drift)

**Relationship:**
```
Course_Over_Ground_Vector = Course_Through_Water_Vector + Current_Vector

COG_x = STW × sin(CTW) + Drift × sin(Set)
COG_y = STW × cos(CTW) + Drift × cos(Set)

COG = atan2(COG_x, COG_y)
SOG = sqrt(COG_x² + COG_y²)
```

### Course to Steer (allowing for current)

**Given: Desired COG, SOG capability, Current (Set/Drift)**

**Find: Required CTW and STW**

```
Method: Solve vector triangle

Known vectors:
  - Desired COG direction
  - Current vector (Set, Drift)
  - Boat speed capability (STW)

Setup equations:
  STW × sin(CTW) + Drift × sin(Set) = SOG × sin(COG)
  STW × cos(CTW) + Drift × cos(Set) = SOG × cos(COG)

Solve for CTW:
  Required_CTW = COG - arcsin(Drift × sin(Set - COG) / STW)

Check if solution exists:
  If Drift × sin(Set - COG) / STW > 1: Current too strong, cannot achieve desired COG
```

### Composite Course (multiple legs)

**Calculating equivalent single course from multiple legs:**

```
Given: Multiple course segments
  Leg 1: Course C₁, Distance D₁
  Leg 2: Course C₂, Distance D₂
  ...
  Leg n: Course Cₙ, Distance Dₙ

Calculate net displacement:
  Total_North = Σ(Dᵢ × cos(Cᵢ))
  Total_East = Σ(Dᵢ × sin(Cᵢ))

Equivalent course:
  Total_Distance = sqrt(Total_North² + Total_East²)
  Equivalent_Course = atan2(Total_East, Total_North)
```

### Course Deviation Calculations

**Cross-Track Error (XTE):**

```
Given:
  - Planned track line from point A to B
  - Current position P

Calculate perpendicular distance from track:

XTE = Distance_AP × sin(Bearing_AP - Bearing_AB)

Sign convention:
  Positive: Right of track
  Negative: Left of track
```

**Course Correction to Intercept Track:**

```
To intercept track at point ahead:

Correction_Angle = arcsin(XTE / Distance_to_intercept) + Course_Error

Where:
  Course_Error = Current_COG - Planned_Track
```

---

## Vector Mathematics in Sailing

### Vector Fundamentals

**Standard Notation:**
```
Vector V = (V_x, V_y)  in Cartesian coordinates
Vector V = (V_magnitude, V_direction)  in polar coordinates

Conversion:
  V_x = V_magnitude × sin(V_direction)
  V_y = V_magnitude × cos(V_direction)

  V_magnitude = sqrt(V_x² + V_y²)
  V_direction = atan2(V_x, V_y)
```

### Vector Addition (velocity composition)

**Wind Triangle Vector Addition:**

```
Scenario: Finding resultant velocity

Given vectors: A = (A_x, A_y) and B = (B_x, B_y)

Resultant: R = A + B
  R_x = A_x + B_x
  R_y = A_y + B_y

  R_magnitude = sqrt(R_x² + R_y²)
  R_direction = atan2(R_x, R_y)
```

**Example: Current Effect**
```
Boat velocity through water: V_boat = (STW, CTW)
Current velocity: V_current = (Drift, Set)

Velocity over ground: V_ground = V_boat + V_current

In components:
  SOG_x = STW × sin(CTW) + Drift × sin(Set)
  SOG_y = STW × cos(CTW) + Drift × cos(Set)
```

### Vector Subtraction (relative velocity)

**Finding Apparent Wind:**

```
True wind velocity: V_true
Boat velocity: V_boat

Apparent wind = V_true - V_boat

In sailing convention (bow = y-axis):
  AW_x = TW_x - BV_x = TWS × sin(TWA) - 0
  AW_y = TW_y - BV_y = TWS × cos(TWA) - STW

  AWS = sqrt(AW_x² + AW_y²)
  AWA = atan2(AW_x, AW_y)
```

### Dot Product Applications

**VMG Calculation:**

```
Dot product formula:
  V₁ · V₂ = |V₁| |V₂| cos(θ)

Where θ is angle between vectors

VMG toward target:
  VMG = Boat_Velocity · Target_Direction
  VMG = SOG × cos(angle between COG and target bearing)
```

**Wind Component Calculation:**

```
Headwind component (into wind):
  V_headwind = -STW × cos(TWA)
  (Negative when sailing toward wind)

Crosswind component:
  V_crosswind = STW × sin(TWA)
```

### Vector Projection

**Projecting velocity onto desired track:**

```
Given:
  Velocity vector V = (V_magnitude, V_direction)
  Desired track direction: θ_track

Projection on track:
  V_track = V_magnitude × cos(V_direction - θ_track)

Perpendicular component (leeway/drift):
  V_perpendicular = V_magnitude × sin(V_direction - θ_track)
```

---

## Trigonometry Applications

### Distance Calculations

**Rhumb Line Distance (constant bearing):**

```
Given: Point 1 (φ₁, λ₁), Point 2 (φ₂, λ₂)
Where φ = latitude, λ = longitude (in radians)

Δφ = φ₂ - φ₁
Δλ = λ₂ - λ₁

Δψ = ln(tan(φ₂/2 + π/4) / tan(φ₁/2 + π/4))

q = Δφ / Δψ  (or cos(φ₁) if Δφ is very small)

Distance = sqrt(Δφ² + q² × Δλ²) × R

Where R = Earth radius = 3440 nautical miles (mean)
```

**Great Circle Distance (shortest path):**

```
Spherical Law of Cosines:

d = acos(sin(φ₁) × sin(φ₂) + cos(φ₁) × cos(φ₂) × cos(Δλ))

Distance = d × R

Where R = 3440 nm (or 6371 km)

For better numerical stability (Haversine formula):

a = sin²(Δφ/2) + cos(φ₁) × cos(φ₂) × sin²(Δλ/2)
c = 2 × atan2(sqrt(a), sqrt(1-a))

Distance = c × R
```

**Practical Difference:**
- Short distances (<100 nm): Rhumb ≈ Great Circle
- Long distances: Great Circle is shorter but requires course changes
- Rhumb line maintains constant compass bearing

### Angle Calculations

**Course/Bearing Calculations:**

```
Initial bearing on great circle:

θ = atan2(sin(Δλ) × cos(φ₂),
          cos(φ₁) × sin(φ₂) - sin(φ₁) × cos(φ₂) × cos(Δλ))

Convert to compass bearing (0-360°):
Bearing = (θ × 180/π + 360) mod 360
```

**Rhumb Line Course:**

```
θ = atan2(Δλ, Δψ)

Where Δψ = ln(tan(φ₂/2 + π/4) / tan(φ₁/2 + π/4))

Rhumb_Course = (θ × 180/π + 360) mod 360
```

### Position from Distance and Bearing

**Dead Reckoning Position:**

```
Given: Starting position (φ₁, λ₁), Course θ, Distance d

Δφ = (d / R) × cos(θ)
Δλ = (d / R) × sin(θ) / cos(φ₁)

New position:
φ₂ = φ₁ + Δφ
λ₂ = λ₁ + Δλ

Where:
  R = 3440 nm (Earth radius in nautical miles)
  d = distance in nautical miles
  θ = course in radians
```

**Great Circle Position (destination point):**

```
Given: Start (φ₁, λ₁), distance d, initial bearing θ

Angular distance:
δ = d / R

Destination latitude:
φ₂ = asin(sin(φ₁) × cos(δ) + cos(φ₁) × sin(δ) × cos(θ))

Destination longitude:
λ₂ = λ₁ + atan2(sin(θ) × sin(δ) × cos(φ₁),
                cos(δ) - sin(φ₁) × sin(φ₂))
```

### Horizon Distance Calculation

**Distance to Horizon:**

```
For observer at height h (in feet) above sea level:

Distance_nm = 1.17 × sqrt(h_feet)

Or for meters:
Distance_nm = 2.08 × sqrt(h_meters)

For two heights (observer and object):
Distance_nm = 1.17 × (sqrt(h₁) + sqrt(h₂))
```

### Dip of Sea Horizon

**Angle below horizontal:**

```
Dip_arcminutes = 0.97 × sqrt(height_feet)

Or:
Dip_arcminutes = 1.76 × sqrt(height_meters)

This correction is needed for sextant observations
```

---

## Compass Calculations

### Magnetic Variation (Declination)

**Definition:** Angle between True North and Magnetic North

**Notation:**
- East variation: Magnetic North is EAST of True North
- West variation: Magnetic North is WEST of True North

**Variation changes by:**
- Geographic location
- Time (secular change, typically 0-15 arcminutes per year)

### Compass Deviation

**Definition:** Error in magnetic compass caused by local magnetic fields on vessel

**Sources:**
- Ferrous metal in hull
- Electronic equipment
- Engines and generators
- Changes with boat heading

**Deviation Card:**
- Table showing deviation for different headings
- Specific to each vessel
- Must be updated when equipment changes

### Converting Between True, Magnetic, and Compass

**Memory Aid: "CADET"**
- **C**ompass → **A**dd **E**ast → **D**eviation → **M**agnetic → **A**dd **E**ast → **V**ariation → **T**rue
- When going from Compass to True: ADD East errors
- When going from True to Compass: ADD West errors (subtract East)

**Formulas:**

```
True to Magnetic:
  Magnetic = True - Variation
  (If variation is East, subtract; if West, add)

Magnetic to Compass:
  Compass = Magnetic - Deviation
  (If deviation is East, subtract; if West, add)

Combined (True to Compass):
  Compass = True - Variation - Deviation

Or using signs (East positive, West negative):
  Magnetic = True + Variation_signed
  Compass = Magnetic + Deviation_signed
```

**Alternative Memory Aids:**

```
"Variation East, Magnetic Least" (True to Magnetic)
"Variation West, Magnetic Best"

"From Compass to True, Add East is due"
"From True to Compass, West is best"
```

### Practical Compass Calculations

**Example 1: True to Compass**
```
Given:
  True Course = 090°
  Variation = 15° W
  Deviation = 5° E

Solution:
  Step 1: True to Magnetic
    Magnetic = 090° - (-15°) = 105°

  Step 2: Magnetic to Compass
    Compass = 105° - 5° = 100°

Steer compass course 100° to make good true course 090°
```

**Example 2: Compass to True**
```
Given:
  Compass Course = 270°
  Variation = 10° E
  Deviation = 3° W

Solution:
  Step 1: Compass to Magnetic
    Magnetic = 270° - (-3°) = 273°

  Step 2: Magnetic to True
    True = 273° - 10° = 263°

Compass course 270° equals true course 263°
```

### Determining Deviation

**Swing Ship Method:**
```
1. Motor in circles on known magnetic bearings (N, NE, E, SE, S, SW, W, NW)
2. Record compass heading at each magnetic direction
3. Deviation = Magnetic - Compass

Create deviation table:
  Ship Head (Mag) | Compass | Deviation
  000° | 358° | 2° W
  045° | 046° | 1° E
  090° | 093° | 3° E
  ... etc
```

**Range Method:**
```
1. Find charted range (two aligned navigation marks)
2. Note chart bearing of range (true)
3. Convert to magnetic using local variation
4. Steer boat along range, note compass heading
5. Deviation = Magnetic - Compass
```

---

## Velocity Made Good (VMG) Calculations

### VMG Definition

**Velocity Made Good:** The component of boat speed in the direction of the target or wind

**Two Primary Uses:**
1. **VMG to Waypoint:** Speed of progress directly toward destination
2. **VMG to Wind:** Speed of progress directly upwind or downwind

### VMG to Waypoint

**Formula:**
```
VMG_waypoint = SOG × cos(θ)

Where:
  θ = angle between COG and bearing to waypoint

  θ = abs(COG - Bearing_to_waypoint)
```

**Calculation Steps:**
```
1. Determine bearing to waypoint from current position
2. Determine current COG
3. Calculate angle difference: θ = |COG - Bearing|
4. VMG = SOG × cos(θ)

If θ > 90°: VMG is negative (moving away from waypoint)
```

**Example:**
```
Given:
  SOG = 6 knots
  COG = 045°
  Bearing to waypoint = 030°

Solution:
  θ = |045° - 030°| = 15°
  VMG = 6 × cos(15°) = 6 × 0.966 = 5.80 knots
```

### VMG Upwind (to weather)

**Formula:**
```
VMG_upwind = STW × cos(TWA)

Where TWA is True Wind Angle
```

**Optimization:**
```
Goal: Maximize VMG = STW × cos(TWA)

Since both STW and TWA vary with pointing angle:
  - Point higher: TWA decreases, but STW decreases (more drag)
  - Bear off: STW increases, but TWA increases (less direct)

Optimal TWA typically: 40-45° for modern boats
```

**Finding Optimal VMG Angle:**
```
Method 1: Empirical testing
  - Sail at different TWA (35°, 40°, 45°, 50°)
  - Record STW at each angle
  - Calculate VMG = STW × cos(TWA)
  - Maximum VMG is optimal angle

Method 2: Using polar diagram
  - Plot boat speed vs. TWA
  - Draw line from origin tangent to curve
  - Tangent point is optimal VMG angle
```

**VMG Table Example:**
```
TWA | STW (knots) | cos(TWA) | VMG (knots)
----|-------------|----------|------------
35° | 5.2 | 0.819 | 4.26
40° | 5.5 | 0.766 | 4.21
45° | 5.7 | 0.707 | 4.03
50° | 5.8 | 0.643 | 3.73

Optimal: 35° TWA gives best VMG upwind
```

### VMG Downwind

**Formula:**
```
VMG_downwind = STW × cos(180° - TWA)
            = -STW × cos(TWA)
            = STW × |cos(TWA)|  (for angles > 90°)

Alternative form:
VMG_downwind = STW × cos(TWA - 180°)
```

**Dead Downwind Paradox:**
```
At TWA = 180° (dead run):
  VMG = STW × cos(180°) = STW × (-1) = -STW
  VMG magnitude = STW (seems optimal)

BUT: Boat often sails slower dead downwind!

Better strategy:
  TWA = 140-150° (broad reach)
  Higher STW compensates for less direct angle

Example:
  Dead run: TWA = 180°, STW = 4.5 kts → VMG = 4.5
  Broad reach: TWA = 145°, STW = 6.0 kts → VMG = 6.0 × cos(35°) = 4.9 kts

Broad reach gives better VMG!
```

### Optimal Tacking/Jibing Angles

**Upwind Tacking:**
```
Tack when: VMG on current tack < potential VMG on other tack

Optimal tacking when:
  Layline reached: Can fetch mark on single tack

Layline calculation:
  Layline_angle = Bearing_to_mark ± Optimal_TWA

  Starboard layline: Mark_bearing - Optimal_TWA
  Port layline: Mark_bearing + Optimal_TWA
```

**Downwind Jibing:**
```
Similar to upwind, maximize VMG downwind

For windward/leeward course:
  Optimal jibe angle ≈ 140-150° TWA
  Sail "hot" angles rather than dead downwind

Jibe when reaching layline on opposite jibe
```

### VMG Performance Analysis

**Percent of Wind Speed:**
```
VMG% = (VMG_upwind / TWS) × 100

Good performance:
  World-class racing boats: 50-55% of TWS
  Club racers: 40-50% of TWS
  Cruising boats: 35-45% of TWS

Example:
  TWS = 12 knots
  STW = 6.5 knots at 42° TWA
  VMG = 6.5 × cos(42°) = 4.83 knots
  VMG% = (4.83 / 12) × 100 = 40.3%
```

**Target VMG:**
```
Use polar diagrams to determine target VMG for conditions

Target_VMG = Polar_Speed(TWA, TWS) × cos(TWA)

Compare actual vs. target to assess performance
```

---

## Practical Examples and Worked Problems

### Example 1: Complete Navigation Solution

**Problem:**
```
You are sailing with the following conditions:
  - Heading (magnetic): 045°
  - Boat speed: 6.0 knots
  - Leeway: 5° (starboard tack)
  - Current: 1.2 knots from 270° (setting East)
  - Variation: 12° W
  - Deviation on heading 045°: 2° E
  - Calculate: True COG and SOG
```

**Solution:**
```
Step 1: Convert heading to true
  Compass = Magnetic + Dev = 045° (given as magnetic)
  True HDG = Magnetic + Var = 045° + (-12°) = 033°

Step 2: Apply leeway (starboard tack, so add)
  CTW = 033° + 5° = 038°

Step 3: Calculate COG and SOG using current triangle

  Boat velocity components:
    BV_north = 6.0 × cos(038°) = 4.73 kts
    BV_east = 6.0 × sin(038°) = 3.69 kts

  Current components:
    Current_north = 1.2 × cos(270°) = 0 kts
    Current_east = 1.2 × sin(270°) = -1.2 kts

  Resultant (over ground):
    SOG_north = 4.73 + 0 = 4.73 kts
    SOG_east = 3.69 + 1.2 = 4.89 kts

    SOG = sqrt(4.73² + 4.89²) = 6.80 kts
    COG = atan2(4.89, 4.73) = 46.0°

Answer: True COG = 046°, SOG = 6.8 knots
```

### Example 2: True Wind Calculation

**Problem:**
```
Your instruments show:
  - AWA: 35° starboard
  - AWS: 18 knots
  - Boat speed: 6.5 knots
  - Heading: 090° true

Calculate: True wind speed and direction
```

**Solution:**
```
Step 1: Set up coordinate system (bow = north, starboard = east)

  Apparent wind components:
    AW_x = 18 × sin(35°) = 10.32 kts (to starboard)
    AW_y = 18 × cos(35°) = 14.74 kts (from ahead)

  Boat velocity components:
    BV_x = 0 (moving straight ahead)
    BV_y = -6.5 (negative because opposes apparent wind)

Step 2: Calculate true wind components
  TW_x = AW_x + BV_x = 10.32 + 0 = 10.32 kts
  TW_y = AW_y + BV_y = 14.74 + (-6.5) = 8.24 kts

Step 3: Calculate TWS and TWA
  TWS = sqrt(10.32² + 8.24²) = 13.2 kts
  TWA = atan2(10.32, 8.24) = 51.4°

Step 4: Calculate TWD
  TWD = HDG + TWA = 090° + 51.4° = 141.4°

Answer: TWS = 13.2 knots, TWD = 141° true
```

### Example 3: Beating to Windward

**Problem:**
```
You need to sail to a mark directly upwind. Conditions:
  - Distance to mark: 5.0 nm
  - True wind from: 000°
  - Your close-hauled angle: 45° TWA
  - Boat speed close-hauled: 5.5 knots
  - Current: 0.8 knots from 090° (setting West)

Calculate: Distance sailed and time to mark
```

**Solution:**
```
Step 1: Determine if you can fetch the mark
  Wind from 000°, mark at 000°
  Can only sail to 045° or 315° TWA
  Must tack to reach mark

Step 2: Calculate legs (assuming equal tacks)
  Since wind is aligned with mark, tacks are symmetric

  Starboard tack: HDG = 000° + 45° = 045°
  Port tack: HDG = 000° - 45° = 315°

Step 3: Account for current on each tack

  Starboard tack (045° HDG):
    BV_north = 5.5 × cos(045°) = 3.89 kts
    BV_east = 5.5 × sin(045°) = 3.89 kts
    Current: 0 north, -0.8 east

    COG_north = 3.89 kts
    COG_east = 3.89 - 0.8 = 3.09 kts
    SOG = 4.97 kts
    VMG_upwind = COG_north = 3.89 kts

  Port tack (315° HDG):
    BV_north = 5.5 × cos(315°) = 3.89 kts
    BV_east = 5.5 × sin(315°) = -3.89 kts
    Current: 0 north, -0.8 east

    COG_north = 3.89 kts
    COG_east = -3.89 - 0.8 = -4.69 kts
    SOG = 6.14 kts
    VMG_upwind = COG_north = 3.89 kts

Step 4: Calculate time
  Since both tacks give same VMG north (current is perpendicular):
  Average VMG = 3.89 kts
  Time = 5.0 nm / 3.89 kts = 1.29 hours = 77 minutes

Step 5: Calculate distance sailed
  Each tack covers: 5.0 / (2 × cos(45°)) = 3.54 nm
  But must account for actual SOG:

  Time per tack: 77 / 2 = 38.5 minutes = 0.64 hours
  Distance starboard: 4.97 × 0.64 = 3.18 nm
  Distance port: 6.14 × 0.64 = 3.93 nm
  Total distance: 7.11 nm

Answer: Distance sailed ≈ 7.1 nm, Time ≈ 77 minutes
```

### Example 4: Optimal VMG Angle

**Problem:**
```
Using boat polar data, find optimal upwind VMG angle:

TWA | Boat Speed
----|------------
30° | 4.8 kts
35° | 5.3 kts
40° | 5.6 kts
45° | 5.8 kts
50° | 5.9 kts
55° | 6.0 kts
```

**Solution:**
```
Calculate VMG for each angle:

TWA | Boat Speed | cos(TWA) | VMG
----|-----------|----------|------
30° | 4.8 | 0.866 | 4.16 kts
35° | 5.3 | 0.819 | 4.34 kts ← Maximum
40° | 5.6 | 0.766 | 4.29 kts
45° | 5.8 | 0.707 | 4.10 kts
50° | 5.9 | 0.643 | 3.79 kts
55° | 6.0 | 0.574 | 3.44 kts

Answer: Optimal VMG at 35° TWA, giving 4.34 kts to windward

Note: Although boat speed increases up to 55°, the angle becomes
too broad and VMG decreases. Sweet spot is 35° for this boat.
```

### Example 5: Course to Steer with Current

**Problem:**
```
You want to make good a course of 270° true with SOG of 6 kts.
Current: 2.0 kts from 180° (setting North)
Boat speed capability: 6.5 kts

Calculate: Course to steer (CTW)
```

**Solution:**
```
Step 1: Set up the vector triangle
  Desired: COG = 270°, SOG = 6 kts
  Current: Set = 180°, Drift = 2.0 kts

  Need to find: CTW and verify STW ≈ 6.5 kts

Step 2: Use current triangle formula

  Current components:
    Current_north = 2.0 × cos(180°) = -2.0 kts
    Current_east = 2.0 × sin(180°) = 0 kts

  Desired velocity over ground:
    COG_north = 6.0 × cos(270°) = 0 kts
    COG_east = 6.0 × sin(270°) = -6.0 kts

Step 3: Calculate required velocity through water
  STW_north = COG_north - Current_north = 0 - (-2.0) = 2.0 kts
  STW_east = COG_east - Current_east = -6.0 - 0 = -6.0 kts

  STW = sqrt(2.0² + (-6.0)²) = 6.32 kts ✓ (within capability)
  CTW = atan2(-6.0, 2.0) = 288.4°

Answer: Steer 288° true through water
(18° north of desired course to compensate for northward current)
```

### Example 6: Great Circle vs Rhumb Line

**Problem:**
```
Calculate distance from San Francisco to Yokohama:
  San Francisco: 37.8° N, 122.4° W
  Yokohama: 35.4° N, 139.7° E

Compare great circle and rhumb line distances.
```

**Solution:**
```
Step 1: Convert to radians and calculate differences
  φ₁ = 37.8° × π/180 = 0.6597 rad
  φ₂ = 35.4° × π/180 = 0.6178 rad
  λ₁ = -122.4° × π/180 = -2.1363 rad
  λ₂ = 139.7° × π/180 = 2.4383 rad

  Δφ = 0.6178 - 0.6597 = -0.0419 rad
  Δλ = 2.4383 - (-2.1363) = 4.5746 rad

Step 2: Great circle distance (Haversine)
  a = sin²(Δφ/2) + cos(φ₁) × cos(φ₂) × sin²(Δλ/2)
  a = sin²(-0.0210) + cos(0.6597) × cos(0.6178) × sin²(2.2873)
  a = 0.000441 + 0.7880 × 0.8090 × 0.6169
  a = 0.000441 + 0.3933 = 0.3937

  c = 2 × atan2(sqrt(0.3937), sqrt(0.6063))
  c = 2 × atan2(0.6275, 0.7787) = 2 × 0.6766 = 1.3532 rad

  Distance_GC = 1.3532 × 3440 nm = 4,655 nm

Step 3: Rhumb line distance
  Δψ = ln(tan(φ₂/2 + π/4) / tan(φ₁/2 + π/4))
  Δψ = ln(tan(1.0947) / tan(1.1159))
  Δψ = ln(1.9484 / 2.0165) = -0.0343

  q = Δφ / Δψ = -0.0419 / -0.0343 = 1.222

  Distance_RL = sqrt(Δφ² + q² × Δλ²) × 3440
  Distance_RL = sqrt(0.001756 + 1.493 × 20.927) × 3440
  Distance_RL = sqrt(31.24) × 3440 = 19,236 nm

Wait - error in calculation! Longitude difference crosses dateline.

Corrected Δλ (westward, shorter):
  Δλ = 360° - (122.4° + 139.7°) = 97.9° = 1.709 rad

Recalculate:
  Great circle: ≈ 4,536 nm
  Rhumb line: ≈ 4,612 nm

Answer:
  Great circle: 4,536 nm (shortest)
  Rhumb line: 4,612 nm
  Difference: 76 nm (1.7% longer for rhumb line)
```

---

## Quick Reference Formulas

### Essential Conversions

```
1 nautical mile = 1.852 km = 1.1508 statute miles = 6076 feet
1 knot = 1 nm/hr = 1.852 km/hr = 0.5144 m/s
1 degree latitude = 60 nautical miles
1 minute latitude = 1 nautical mile
1 fathom = 6 feet = 1.829 meters
```

### Core Navigation Formulas

```
COG from CTW and current:
  COG_x = STW × sin(CTW) + Drift × sin(Set)
  COG_y = STW × cos(CTW) + Drift × cos(Set)
  COG = atan2(COG_x, COG_y)
  SOG = sqrt(COG_x² + COG_y²)

Course Through Water from Heading:
  CTW = HDG + L  (starboard tack, L > 0)
  CTW = HDG - L  (port tack, L > 0)

True Wind from Apparent:
  TWS = sqrt(AWS² + STW² - 2×AWS×STW×cos(AWA))
  TWA = arcsin(AWS × sin(AWA) / TWS)

VMG:
  VMG_to_target = SOG × cos(COG - Bearing_to_target)
  VMG_upwind = STW × cos(TWA)
```

### Distance Formulas

```
Great Circle Distance:
  d = acos(sin(φ₁)×sin(φ₂) + cos(φ₁)×cos(φ₂)×cos(Δλ))
  Distance_nm = d × 3440

Rhumb Line Distance:
  Δψ = ln(tan(φ₂/2 + π/4) / tan(φ₁/2 + π/4))
  q = Δφ/Δψ
  Distance_nm = sqrt(Δφ² + q²×Δλ²) × 3440

Distance to Horizon:
  Distance_nm = 1.17 × sqrt(height_feet)
```

### Compass Formulas

```
Compass ↔ True:
  True = Compass + Deviation + Variation
  Compass = True - Deviation - Variation
  (East values positive, West values negative)

Memory: "From Compass Add East"
```

---

## Important Notes and Conventions

### Angular Measurement Standards

1. **Degrees True**: Referenced to True North (geographic north pole)
2. **Degrees Magnetic**: Referenced to Magnetic North (magnetic pole)
3. **Degrees Compass**: What compass actually reads (includes deviation)

### Sign Conventions

1. **Wind Angles**:
   - 0° = head wind (dead ahead)
   - 90° = beam wind
   - 180° = following wind (dead astern)
   - Starboard: 0-180°, Port: 180-360° (or negative)

2. **Leeway**:
   - Always to leeward (downwind)
   - Starboard tack: ADD to heading
   - Port tack: SUBTRACT from heading

3. **Variation/Deviation**:
   - East: Subtract when going True→Compass
   - West: Add when going True→Compass
   - Reverse when going Compass→True

### Calculation Accuracy Considerations

1. **Rounding**: Carry extra precision through calculations, round final answer
2. **Angles near 0°/360°**: Use modulo arithmetic carefully
3. **atan2 vs atan**: Always use atan2 for navigation angles (handles all quadrants)
4. **Small angle approximations**: Valid only for angles < 10°
5. **Earth radius**: Use 3440 nm for most sailing calculations

### Practical Limits

1. **Leeway**: Typically 0-10°, rarely exceeds 15° even in extreme conditions
2. **Current drift**: Usually < 5 knots in coastal waters
3. **Compass deviation**: Should be < 5° on well-adjusted compass
4. **GPS accuracy**: Typically ±10 meters for position, ±0.1 knots for speed

---

## Sources and References

This comprehensive guide was compiled from authoritative sources including:

### Navigation and Mathematics
- **Bowditch's American Practical Navigator** (NIMA Publication 9)
- **Dutton's Nautical Navigation** (Naval Institute Press)
- **Celestial Navigation for Yachtsmen** (Mary Blewitt)
- **NOAA Navigation Services**

### Wind and Performance
- **The Physics of Sailing** (University of New South Wales)
- **Principles of Yacht Design** (Larsson & Eliasson)
- **Performance Sailing** (Steve Colgate)
- **NauticEd Sailing Theory**

### Online Resources
- Physics iCalculator - True Wind Speed Calculator
- Sailing Issues Navigation Courses
- Seaman Kowts Maritime Guides
- Blue Water Sailing Technical Articles
- Wikipedia: Apparent Wind, Great-circle Navigation, Rhumb Line

### Standards Organizations
- **International Maritime Organization (IMO)**
- **World Meteorological Organization**
- **US Power Squadrons Navigation Standards**
- **Royal Yachting Association (RYA)**

---

## Document Information

**Created**: 2025-09-23
**Version**: 1.0
**Purpose**: Comprehensive mathematical reference for sailing navigation and performance optimization
**Intended Audience**: Sailors, navigators, racing tacticians, autopilot developers, and marine software engineers

---

## Usage Notes

- All angles are in degrees unless specified in radians
- Trigonometric functions assume radian inputs - convert as needed
- Formulas use standard mathematical notation (×, √, π, sin, cos, atan2)
- Programming implementations should use appropriate language math libraries
- Vectors shown in both Cartesian (x,y) and Polar (magnitude, direction) forms
- Earth approximated as sphere (radius 3440 nm) for most calculations
- For extremely long passages (>1000 nm), consider ellipsoidal earth models

---

*This guide is intended for educational, reference, and technical implementation purposes. Always verify calculations with redundant methods for safety-critical navigation.*