# Changelog

All notable changes to this project will be documented in this file starting 2021.

## [1.6.0] -- 2025-10-25

* New since 1.5.2

  * ADD: Optional debounce support (setDebounceMillis()) to filter contact bounce without external components.
  * ADD: New example sketches: RotaryWithButton and AcceleratedRotatorAdvanced (showing debounce and acceleration together).
  * CI: Expanded unit/integration tests and updated CI configuration for newer Arduino cores and PlatformIO.
  * DOCS: Updated README and examples for IDE 2.x and core compatibility; clarified LatchMode behavior.

* Fixes and improvements since 1.5.2

  * FIX: Robust interrupt handling across cores (AVR, SAMD, ESP32, ESP8266) — consistent use of digitalPinToInterrupt() and edge configuration to avoid missed steps on non‑AVR boards.
  * FIX: Race conditions when reading counts from ISR — atomic access and reduced critical sections.
  * FIX: Reduced CPU usage in ISR and polling paths (lower interrupt overhead).
  * CHANGE: Improved API stability notes and migration guide in README.


## [1.5.2] - 2021-07-04

minor fixes found in ci integration.


## [1.5.1] - 2021-04-23

FIX: attachInterrupt() with digitalPinToInterrupt() required for Arduino UNO etc.


## [1.5.0] - 2021-02-06

* CHANGELOG created.

* Many thanks to the improvements included from **@deltaphi** (#12), **@rprediger** (#22) and **@giupo** (#24) 

* The function **getMillisBetweenRotations()** was added. See example AcceleratedRotator.

* The function **getRPM()** was added.


### Examples

* The examples also compile with ESP8266 and Every boards. (the library worked already).

* new **LatchMode** in initialization supporting rotary encoders with 2 state changes from one latch to the next (LatchMode).

* The **InterruptRotator** example now uses attachInterrupt() instead of ATMega registers and ISR. (The old code is still in comments).

