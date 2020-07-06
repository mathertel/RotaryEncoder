// -----
// RotaryEncoder.h - Library for using rotary encoders.
// This class is implemented for use with the Arduino environment.
// Copyright (c) by Matthias Hertel, http://www.mathertel.de
// This work is licensed under a BSD style license. See http://www.mathertel.de/License.aspx
// More information on: http://www.mathertel.de/Arduino
// -----
// 18.01.2014 created by Matthias Hertel
// 16.06.2019 pin initialization using INPUT_PULLUP
// -----
// 06.07.2020 Debounce is now opt-in by adding the following to the sketch: #define ky40btn_debounce

#ifndef RotaryEncoder_h
#define RotaryEncoder_h

#include "Arduino.h"

#define LATCHSTATE 3

class RotaryEncoder
{
public:
  enum class Direction { NOROTATION = 0, CLOCKWISE = 1, COUNTERCLOCKWISE = -1};

  // ----- Constructor -----
  #if defined ky40btn_debounce
  RotaryEncoder(int pin1, int pin2, uint8_t buttonPin, unsigned long interval_millis); //Constructor with push button debounce
  #endif
  RotaryEncoder(int pin1, int pin2);
  
  // retrieve the current position
  long  getPosition();
  
  // simple retrieve of the direction the knob was rotated at. 0 = No rotation, 1 = Clockwise, -1 = Counter Clockwise
  Direction getDirection();

  // adjust the current position
  void setPosition(long newPosition);

  // call this function every some milliseconds or by using an interrupt for handling state changes of the rotary encoder.
  void tick(void);

  // Returns the time in milliseconds between the current observed 
  unsigned long getMillisBetweenRotations() const;
  
  //  Push Button debounce
  #if defined ky40btn_debounce

  void interval(unsigned long interval_millis);
  // Updates the pin
  // Returns 1 if the state changed
  // Returns 0 if the state did not change
  int update();
  // Forces the pin to signal a change (through update()) in X milliseconds
  // even if the state does not actually change
  // Example: press and hold a button and have it repeat every X milliseconds
  void rebounce(unsigned long interval);
  // Returns the updated pin state
  int read();
  // Sets the stored pin state
  void write(int new_state);
    // Returns the number of milliseconds the pin has been in the current state
  unsigned long duration();

  #endif
//   End Push Button debounce



private:
  int _pin1, _pin2; // Arduino pins used for the encoder. 
  
  volatile int8_t _oldState;
  
  volatile long _position;         // Internal position (4 times _positionExt)
  volatile long _positionExt;      // External position
  volatile long _positionExtPrev;  // External position (used only for direction checking)

  unsigned long _positionExtTime;     // The time the last position change was detected.
  unsigned long _positionExtTimePrev; // The time the previous position change was detected.
};

#endif

// End