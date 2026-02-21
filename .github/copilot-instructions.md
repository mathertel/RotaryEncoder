# Copilot Instructions for RotaryEncoder

## Project Expectations

RotaryEncoder is a lightweight Arduino library for handling rotary encoder input across
multiple microcontroller platforms (Arduino, ESP8266, ESP32). The library prioritizes
reliability, minimal resource usage, and compatibility with embedded systems.

- **Target Platforms**: Arduino (AVR), ESP8266, ESP32, and compatible boards
- **Primary Use Case**: Non-blocking rotary encoder event detection in applications with tight timing constraints
- **Target Users**: Embedded systems developers, IoT projects, Arduino hobbyists
- **Quality Bar**: Production-ready code suitable for battery-powered and resource-constrained devices

## Coding Style

- **Language**: C++ with compatible C patterns
- **Compatibility**: Must compile with Arduino IDE and PlatformIO toolchains
- **Indentation**: 2-space indentation for consistency with Arduino conventions
- **Naming Conventions**:
  - Classes: `PascalCase` (e.g., `RotaryEncoder`)
  - Methods/Functions: `camelCase` (e.g., `begin()`, `getPosition()`)
  - Private/Protected Members: prefix with `_` (e.g., `_pinA`, `_pinB`)
  - Constants: `UPPER_CASE` (e.g., `ENCODER_STEPS`)
- **Code Clarity**: Prioritize readable, self-documenting code over complexity
- **Comments**: Include comments for non-obvious logic; explain the "why," not the "what"

## Library-Specific Rules

- **No Dynamic Allocation**: All data structures must be statically allocated. No `new`, `malloc()`, or dynamic arrays. Use fixed-size arrays instead.
- **API Stability**: Maintain backward compatibility. Only add methods; never remove or change existing public signatures.
- **Non-Blocking Patterns**: All library functions must be non-blocking. No `delay()`, `millis()` waits, or blocking I/O in the main library code.
- **Interrupt Safety**: Code must be safe to call from both interrupt handlers and main loop. Use volatile variables appropriately.
- **Minimal Dependencies**: No external libraries or cloud dependencies. Only use Arduino core APIs.
- **Memory Efficiency**: Minimize RAM and flash usage. Suitable for devices with as little as 16KB RAM.
- **Platform Abstraction**: Use Arduino core abstractions (e.g., `digitalWrite()`, `attachInterrupt()`) for compatibility across boards.

## Examples and Documentation

- **Location**: See `/examples/` directory for working examples:
  - `SimplePollRotator.ino` — Basic polling-based usage
  - `InterruptRotator.ino` — Interrupt-driven implementation
  - `AcceleratedRotator.ino` — Acceleration detection
  - `LimitedRotator.ino` — Position limiting with boundary checks
- **Example Standards**: All examples must:
  - Be minimal and focused (< 100 lines when possible)
  - Include clear comments explaining each step
  - Work without modification on Arduino boards
  - Demonstrate a single feature/use case

## How Copilot Should Respond

- **Code Suggestions**: Provide implementations that follow Arduino conventions and embedded C++ best practices
- **API Design**: Suggest additions that maintain backward compatibility; mark breaking changes clearly
- **Documentation**: Include JavaDoc-style comments for public methods (/** ... */)
- **Testing Suggestions**: Recommend test scenarios that cover interrupt safety and different platform behavior
- **Examples**: When suggesting new features, provide minimal, runnable examples
- **Performance**: Highlight memory usage and timing implications of suggestions
- **Problem-Solving**: Ask clarifying questions about platform targets and hardware constraints
- **Refactoring**: Suggest improvements that maintain the non-blocking guarantee and static allocation requirement

## Do Not

- ❌ Use `malloc()`, `new`, `delete`, or `free()`. Use stack and static allocation only.
- ❌ Add `delay()` or other blocking calls to library code.
- ❌ Introduce external library dependencies (beyond Arduino core).
- ❌ Change existing public method signatures without deprecation warnings.
- ❌ Use c++17 or later features; maintain c++11/14 compatibility.
- ❌ Implement cloud connectivity or network features.
- ❌ Add threads or RTOS features that aren't part of Arduino core.
- ❌ Make assumptions about global state; support multiple encoder instances independently.
- ❌ Use platform-specific code without appropriate `#ifdef` guards (e.g., `#ifdef ESP8266`).
- ❌ Perform heavy computation in interrupt handlers; defer processing to `loop()`.
- ❌ Commit build artifacts (see `.gitignore`); only commit source files.
- ❌ Remove backward compatibility without explicit version bump and migration guide.

## Additional Context

- **Build System**: Arduino IDE standard; also compatible with PlatformIO
- **License**: LGPL-2.1 (see LICENSE file)
- **Changelog**: Document significant changes in `CHANGELOG.md`
- **Testing**: Manual testing on target boards is preferred; automated tests must not depend on hardware
