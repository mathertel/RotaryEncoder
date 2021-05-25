// -----
// RotaryEncoder.h - Library for using rotary encoders.
// This class is implemented for use with the Arduino environment.
//
// Copyright (c) by Matthias Hertel, http://www.mathertel.de
//
// This work is licensed under a BSD 3-Clause style license,
// https://www.mathertel.de/License.aspx.
//
// More information on: http://www.mathertel.de/Arduino
// -----
// 18.01.2014 created by Matthias Hertel
// 16.06.2019 pin initialization using INPUT_PULLUP
// 10.11.2020 Added the ability to obtain the encoder RPM
// 29.01.2021 Options for using rotary encoders with 2 state changes per latch.
// -----

#ifndef RotaryEncoder_h
#define RotaryEncoder_h

class RotaryEncoder;

extern "C" {
typedef void (*callbackFunctionPosDir)(const long position, const int dir);
typedef void (*callbackFunctionClassRef)(RotaryEncoder & rotary_encoder);
typedef void (* callbackFunctionPointerParameter)(void * parm);
}

#include "Arduino.h"

class RotaryEncoder
{
public:
  enum class Direction {
    NOROTATION = 0,
    CLOCKWISE = 1,
    COUNTERCLOCKWISE = -1
  };

  enum class LatchMode {
    FOUR3 = 1, // 4 steps, Latch at position 3 only (compatible to older versions)
    FOUR0 = 2, // 4 steps, Latch at position 0 (reverse wirings)
    TWO03 = 3  // 2 steps, Latch at position 0 and 3 
  };

  // ----- Constructor -----
  RotaryEncoder(int pin1, int pin2, LatchMode mode = LatchMode::FOUR0);

  // retrieve the current position
  long getPosition();

  // simple retrieve of the direction the knob was rotated at. 0 = No rotation, 1 = Clockwise, -1 = Counter Clockwise
  Direction getDirection();

  // adjust the current position
  void setPosition(long newPosition);

  // call this function every some milliseconds or by using an interrupt for handling state changes of the rotary encoder.
  void tick(void);

  // Returns the time in milliseconds between the current observed
  unsigned long getMillisBetweenRotations() const;

  // Returns the RPM
  unsigned long getRPM();

  void attachOnChange(callbackFunctionPosDir on_change);  //Attach a function that takes position and direction as arguments
  void attachOnChange(callbackFunctionClassRef on_change); //Attach a function that takes the class instance as an argument
  void attachOnChange(callbackFunctionPointerParameter on_change, void * parameter); //Attach a function that takes a void * parameter

private:
  int _pin1, _pin2; // Arduino pins used for the encoder.
  
  LatchMode _mode; // Latch mode from initialization

  volatile int8_t _oldState;

  volatile long _position;        // Internal position (4 times _positionExt)
  volatile long _positionExt;     // External position
  volatile long _positionExtPrev; // External position (used only for direction checking)

  unsigned long _positionExtTime;     // The time the last position change was detected.
  unsigned long _positionExtTimePrev; // The time the previous position change was detected.

  callbackFunctionPosDir _on_change_pos_dir = NULL;    // This function is invoked whenever there is a change in position
  callbackFunctionClassRef _on_change_class_ref = NULL; // This function is invoked whenever there is a change in position
  callbackFunctionPointerParameter _on_change_pointer_parameter = NULL; // This function is invoked whenever there is a change in position
  void * _on_change_paramater = NULL;  //This value is passed to the on_change_pointer_parameter function 
};

#endif

// End