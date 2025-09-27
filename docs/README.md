# AutoTiller5000 Documentation Index

Comprehensive reference documentation for building an Arduino-based marine autopilot system.

## Table of Contents

### Arduino Development
| Document | Description | Key Topics |
|----------|-------------|------------|
| [Arduino Programming](./research/arduino-programming.md) | Complete Arduino language reference | Setup/loop structure, data types, I/O operations, memory management, interrupts, Serial communication, timing functions |
| [Arduino TDD](./research/arduino-tdd.md) | Practical test-driven development guide | Testing frameworks (PlatformIO/Unity, AUnit), hardware mocking, testable code patterns, CI/CD setup. See `../test/` for current implementation |
| [Arduino Reliability](./research/arduino-reliability.md) | Best practices for reliable embedded systems | Watchdog timers, power management, fail-safe design, environmental hardening, safety-critical patterns |

### Marine Autopilot Systems
| Document | Description | Key Topics |
|----------|-------------|------------|
| [Autotillers](./research/autotillers.md) | Marine autopilot systems overview | Types (tiller/wheel/below-deck), PID control, sensors, commercial systems, DIY projects, NMEA integration |
| [Seek and Hunt](./research/seek-and-hunt.md) | Hunting behavior analysis and solutions | Control tuning, anti-hunting measures, PID implementation |
| [Miura Sailboat](./research/miura-sailboat.md) | Target vessel specifications | Miura 31 specs, tiller steering, autopilot installation considerations |

### Sailing Knowledge
| Document | Description | Key Topics |
|----------|-------------|------------|
| [Nautical Terms](./research/nautical-terms.md) | Comprehensive sailing and nautical terminology | Directions, measurements, navigation, safety, weather, rigging, points of sail, racing, crew commands |
| [Sailing Mathematics](./research/sailing-mathematics.md) | Navigation and sailing calculations | Heading/course math, wind angles, VMG, apparent/true wind, leeway, vector math, compass calculations |
| [Sailing Safety](./research/sailing-safety.md) | Safety procedures and best practices | Emergency procedures, safety equipment, weather tactics, COLREGs, medical emergencies, fire/flooding response |

### Code Reviews
| Document | Date | Summary |
|----------|------|---------|
| [Code Review 2025-09-23](./code_reviews/code-review-2025-09-23.md) | 2025-09-23 | Initial comprehensive review - 19 issues identified (7 critical, 5 moderate, 7 low priority) |

### AI Context Management
| Directory | Purpose |
|-----------|---------|
| [ai/context/active/](./ai/context/active/) | Current work-in-progress context and active tasks |
| [ai/context/archive/](./ai/context/archive/) | Completed work and historical context |
| [ai/context/templates/](./ai/context/templates/) | Templates for context documentation |

## Quick Reference by Task

### Building an Autotiller
1. Start with [Arduino Programming](./research/arduino-programming.md) for language basics
2. Review [Autotillers](./research/autotillers.md) for control theory and sensor integration
3. Apply [Arduino Reliability](./research/arduino-reliability.md) for robust hardware/software design
4. Implement [Arduino TDD](./research/arduino-tdd.md) for testable, maintainable code

### Understanding Sailing Concepts
1. Learn basic terminology from [Nautical Terms](./research/nautical-terms.md)
2. Study [Sailing Mathematics](./research/sailing-mathematics.md) for navigation calculations
3. Review [Sailing Safety](./research/sailing-safety.md) for critical safety knowledge

### System Integration
- **Sensors & Control**: [Autotillers](./research/autotillers.md) → [Arduino Programming](./research/arduino-programming.md)
- **Navigation Math**: [Sailing Mathematics](./research/sailing-mathematics.md) → [Arduino Programming](./research/arduino-programming.md)
- **Safety Systems**: [Sailing Safety](./research/sailing-safety.md) → [Arduino Reliability](./research/arduino-reliability.md)
- **Testing**: [Arduino TDD](./research/arduino-tdd.md) for all code development

## Document Statistics

| Document | Focus | Approx. Pages |
|----------|-------|---------------|
| arduino-programming.md | Reference | 20-25 |
| arduino-tdd.md | Practical Guide | 15-20 |
| arduino-reliability.md | Best Practices | 25-30 |
| autotillers.md | Technical Overview | 20-25 |
| seek-and-hunt.md | Control Theory | 15-20 |
| miura-sailboat.md | Context | 3-5 |
| nautical-terms.md | Reference | 30-35 |
| sailing-mathematics.md | Technical Reference | 15-20 |
| sailing-safety.md | Procedures | 30-35 |

## AI Navigation Tips

When assisting with AutoTiller5000 development:

1. **For code questions**: Reference `research/arduino-programming.md` for syntax, `research/arduino-reliability.md` for patterns
2. **For control algorithms**: Consult `research/autotillers.md` PID section, `research/seek-and-hunt.md` for anti-hunting techniques, `research/sailing-mathematics.md` for calculations
3. **For terminology**: Use `research/nautical-terms.md` for precise sailing vocabulary
4. **For safety-critical features**: Cross-reference `research/sailing-safety.md` and `research/arduino-reliability.md`
5. **For testing**: Follow patterns in `research/arduino-tdd.md` with mocking examples
6. **For code reviews**: Check `code_reviews/` for previous analysis and improvement recommendations

## Project Context

This documentation supports development of a DIY Arduino-based autotiller for a Miura 31 sailboat. The system uses:
- BNO055 IMU for compass heading
- Servo motor for tiller control
- Proportional control algorithm
- EEPROM for calibration storage

Safety and reliability are paramount for marine autopilot systems operating in harsh saltwater environments.