// -----
// RotaryEncoder.cpp - Library for using rotary encoders.
// This class is implemented for use with the Arduino environment.
//
// Copyright (c) by Matthias Hertel, http://www.mathertel.de
//
// This work is licensed under a BSD 3-Clause style license,
// https://www.mathertel.de/License.aspx.
//
// More information on: http://www.mathertel.de/Arduino
// -----
// Changelog: see RotaryEncoder.h
// -----

#include "RotaryEncoder.h"
#include "Arduino.h"

#define LATCH0 0  // input state at position 0
#define LATCH3 3  // input state at position 3


// The array holds the values �1 for the entries where a position was decremented,
// a 1 for the entries where the position was incremented
// and 0 in all the other (no change or not valid) cases.

const int8_t KNOBDIR[] = {
  0, -1, 1, 0,
  1, 0, 0, -1,
  -1, 0, 0, 1,
  0, 1, -1, 0
};


// positions: [3] 1 0 2 [3] 1 0 2 [3]
// [3] is the positions where my rotary switch detends
// ==> right, count up
// <== left,  count down


// ----- Initialization and Default Values -----

/**
 * @brief Default constructor that initializes the RotaryEncoder with non-hardware specific setup.
 *
 * This constructor creates a RotaryEncoder instance with only software initialization.
 * No pins are configured or reserved. This is useful for scenarios where:
 * - Pins will be managed externally
 * - Hardware-specific tick() will be called with explicit pin values
 * - Deferred or dynamic pin configuration is needed
 *
 * @param mode The latch mode defining the encoder sensitivity.
 *   See RotaryEncoder.h for details on the available modes.
 *
 * Initialization sets:
 * - Encoder mode
 * - Position counter to 0
 * - Internal state variables to initial values (no motion detected)
 * - No pins are reserved (_pin1 and _pin2 set to NO_PIN)
 * - Timestamp tracking for rotation speed calculation
 *
 * @note To use this constructor effectively, call tick(sig1, sig2) with explicit pin values
 *       in your main loop, or use the two-parameter constructor if you need automatic pin handling.
 *
 * @see RotaryEncoder(int pin1, int pin2, LatchMode mode) for hardware-managed pin setup
 * @see tick(int sig1, int sig2) for software-managed pin input
 */
RotaryEncoder::RotaryEncoder(LatchMode mode) {
  _mode = mode;

  // No Hardware specific setup here.
  // use the ...
  _pin1 = _pin2 = NO_PIN;

  // start with position 0;
  _position = 0;
  _oldState = 0;
  _positionExtPrev = _positionExt = 0;
  _positionExtTimePrev = _positionExtTime = millis();
}  // RotaryEncoder()



/**
 * @brief Constructor that initializes the RotaryEncoder with hardware pin setup.
 *
 * This constructor creates a RotaryEncoder instance with full default hardware initialization.
 * It configures the specified pins, enables internal pull-up resistors, and reads their
 * current state to establish the initial encoder position.
 *
 * @param pin1 First encoder pin (typically pin A). Use a value 0 or greater for a valid pin.
 *             A negative value or NO_PIN will skip hardware configuration.
 * @param pin2 Second encoder pin (typically pin B). Use a value 0 or greater for a valid pin.
 *             A negative value or NO_PIN will skip hardware configuration.
 * @param mode The latch mode defining the encoder sensitivity.
 *   See RotaryEncoder.h for details on the available modes.
 *
 * Hardware Setup:
 * - Configures both pins as INPUT_PULLUP for reliable signal detection
 * - Reads the initial state of pin1 and pin2 using digitalRead()
 * - Establishes the initial position based on current pin values
 * - Stores pin numbers for use with the non-parameterized tick() method
 *
 * Interrupt-Safe Usage:
 * This constructor is suitable for both polling and interrupt-driven modes:
 * - For polling: call tick() periodically in your main loop
 * - For interrupts: attach interrupt handlers to these pins and call tick(sig1, sig2)
 *                   from the interrupt handler with explicit pin values for better performance
 *
 * Initial State:
 * - Position counter initialized to 0
 * - Internal state variables set based on reading the actual pin values
 * - Timestamp tracking initialized for rotation speed calculation
 *
 * @note If both pins are negative or not in the valid range [0, MAX_PIN], the hardware
 *       setup is skipped but the encoder still initializes with software defaults.
 * @note The pins must support INPUT_PULLUP mode on your microcontroller.
 * @note For maximum interrupt responsiveness, consider using the parameterized tick(sig1, sig2)
 *       variant and reading pins directly in your interrupt handler.
 */
RotaryEncoder::RotaryEncoder(int pin1, int pin2, LatchMode mode) : RotaryEncoder(mode) {
  int sig1 = 0;
  int sig2 = 0;

  // Remember Hardware Setup
  _pin1 = pin1;
  _pin2 = pin2;

  // Setup the input pins and turn on pullup resistor
  if ((pin1 >= 0) && (pin2 >= 0)) {
    pinMode(pin1, INPUT_PULLUP);
    pinMode(pin2, INPUT_PULLUP);
    // when not started in motion, the current state of the encoder should be 3
    sig1 = digitalRead(_pin1);
    sig2 = digitalRead(_pin2);
  }

  _oldState = sig1 | (sig2 << 1);
}  // RotaryEncoder()


long RotaryEncoder::getPosition() {
  return _positionExt;
}  // getPosition()


RotaryEncoder::Direction RotaryEncoder::getDirection() {
  RotaryEncoder::Direction ret = Direction::NOROTATION;

  if (_positionExtPrev > _positionExt) {
    ret = Direction::COUNTERCLOCKWISE;
    _positionExtPrev = _positionExt;
  } else if (_positionExtPrev < _positionExt) {
    ret = Direction::CLOCKWISE;
    _positionExtPrev = _positionExt;
  } else {
    ret = Direction::NOROTATION;
    _positionExtPrev = _positionExt;
  }

  return ret;
}


void RotaryEncoder::setPosition(long newPosition) {
  switch (_mode) {
    case LatchMode::FOUR3:
    case LatchMode::FOUR0:
      // only adjust the external part of the position.
      _position = ((newPosition << 2) | (_position & 0x03L));
      _positionExt = newPosition;
      _positionExtPrev = newPosition;
      break;

    case LatchMode::TWO03:
      // only adjust the external part of the position.
      _position = ((newPosition << 1) | (_position & 0x01L));
      _positionExt = newPosition;
      _positionExtPrev = newPosition;
      break;
  }  // switch

}  // setPosition()


// Slow, but Simple Variant by directly Read-Out of the Digital State within loop-call
void RotaryEncoder::tick(void) {
  int sig1 = digitalRead(_pin1);
  int sig2 = digitalRead(_pin2);
  tick(sig1, sig2);
}  // tick()


// When a faster method than digitalRead is available you can _tick with the 2 values directly.
void RotaryEncoder::tick(int sig1, int sig2) {
  unsigned long now = millis();
  int8_t thisState = sig1 | (sig2 << 1);

  if (_oldState != thisState) {
    _position += KNOBDIR[thisState | (_oldState << 2)];
    _oldState = thisState;

    switch (_mode) {
      case LatchMode::FOUR3:
        if (thisState == LATCH3) {
          // The hardware has 4 steps with a latch on the input state 3
          _positionExt = _position >> 2;
          _positionExtTimePrev = _positionExtTime;
          _positionExtTime = now;
        }
        break;

      case LatchMode::FOUR0:
        if (thisState == LATCH0) {
          // The hardware has 4 steps with a latch on the input state 0
          _positionExt = _position >> 2;
          _positionExtTimePrev = _positionExtTime;
          _positionExtTime = now;
        }
        break;

      case LatchMode::TWO03:
        if ((thisState == LATCH0) || (thisState == LATCH3)) {
          // The hardware has 2 steps with a latch on the input state 0 and 3
          _positionExt = _position >> 1;
          _positionExtTimePrev = _positionExtTime;
          _positionExtTime = now;
        }
        break;
    }  // switch
  }  // if
}  // tick()


unsigned long RotaryEncoder::getMillisBetweenRotations() const {
  return (_positionExtTime - _positionExtTimePrev);
}

unsigned long RotaryEncoder::getRPM() {
  // calculate max of difference in time between last position changes or last change and now.
  unsigned long timeBetweenLastPositions = _positionExtTime - _positionExtTimePrev;
  unsigned long timeToLastPosition = millis() - _positionExtTime;
  unsigned long t = max(timeBetweenLastPositions, timeToLastPosition);
  return 60000.0 / ((float)(t * 20));
}


// End