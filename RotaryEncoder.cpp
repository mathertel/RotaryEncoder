// -----
// RotaryEncoder.cpp - Library for using rotary encoders.
// This class is implemented for use with the Arduino environment.
// Copyright (c) by Matthias Hertel, http://www.mathertel.de
// This work is licensed under a BSD style license. See http://www.mathertel.de/License.aspx
// More information on: http://www.mathertel.de/Arduino
// -----
// 18.01.2014 created by Matthias Hertel
// 17.06.2015 minor updates.
// -----
// 26.01.2020 Added Debounce functionality for the pus button. Based upon the Bounce library by Thomas Frederics https://playground.arduino.cc/Code/Bounce/
// 06.07.2020 Debounce is now opt-in by adding the following to the sketch: #define ky40btn_debounce

#include "Arduino.h"
#include "RotaryEncoder.h"


// The array holds the values �1 for the entries where a position was decremented,
// a 1 for the entries where the position was incremented
// and 0 in all the other (no change or not valid) cases.

const int8_t KNOBDIR[] = {
  0, -1,  1,  0,
  1,  0,  0, -1,
  -1,  0,  0,  1,
0,  1, -1,  0  };


// positions: [3] 1 0 2 [3] 1 0 2 [3]
// [3] is the positions where my rotary switch detends
// ==> right, count up
// <== left,  count down


// ----- Initialization and Default Values -----

#if defined ky40btn_debounce
RotaryEncoder::RotaryEncoder(int pin1, int pin2, uint8_t buttonPin, unsigned long interval_millis) {
#else
RotaryEncoder::RotaryEncoder(int pin1, int pin2) {
#endif
  
  // Remember Hardware Setup
  _pin1 = pin1;
  _pin2 = pin2;
  
  // Setup the input pins and turn on pullup resistor
  pinMode(pin1, INPUT_PULLUP);
  pinMode(pin2, INPUT_PULLUP);

  // when not started in motion, the current state of the encoder should be 3
  _oldState = 3;

  // start with position 0;
  _position = 0;
  _positionExt = 0;
  _positionExtPrev = 0;

  #if defined ky40btn_debounce  
  // Push Button debounce

  pinMode(buttonPin, INPUT_PULLUP);
  interval(interval_millis);
  previous_millis = millis();
  state = digitalRead(buttonPin);
    this->buttonPin = buttonPin;

  // End Push Button debounce
  #endif
} // RotaryEncoder()

#if defined ky40btn_debounce
RotaryEncoder::RotaryEncoder(int pin1, int pin2) {

  // Remember Hardware Setup
  _pin1 = pin1;
  _pin2 = pin2;
  //_buttonPin = buttonPin;

  // Setup the input pins and turn on pullup resistor
  pinMode(pin1, INPUT_PULLUP);
  pinMode(pin2, INPUT_PULLUP);
    // NEW ++++++++++++

  // when not started in motion, the current state of the encoder should be 3
  _oldState = 3;

  // start with position 0;
  _position = 0;
  _positionExt = 0;
  _positionExtPrev = 0;

} // RotaryEncoder()
#endif

long  RotaryEncoder::getPosition() {
  return _positionExt;
} // getPosition()


RotaryEncoder::Direction RotaryEncoder::getDirection() {

    RotaryEncoder::Direction ret = Direction::NOROTATION;
    
    if( _positionExtPrev > _positionExt )
    {
        ret = Direction::COUNTERCLOCKWISE;
        _positionExtPrev = _positionExt;
    }
    else if( _positionExtPrev < _positionExt )
    {
        ret = Direction::CLOCKWISE;
        _positionExtPrev = _positionExt;
    }
    else 
    {
        ret = Direction::NOROTATION;
        _positionExtPrev = _positionExt;
    }        
    
    return ret;
}



void RotaryEncoder::setPosition(long newPosition) {
  // only adjust the external part of the position.
  _position = ((newPosition<<2) | (_position & 0x03L));
  _positionExt = newPosition;
  _positionExtPrev = newPosition;
} // setPosition()


void RotaryEncoder::tick(void)
{
  int sig1 = digitalRead(_pin1);
  int sig2 = digitalRead(_pin2);
  int8_t thisState = sig1 | (sig2 << 1);

  if (_oldState != thisState) {
    _position += KNOBDIR[thisState | (_oldState<<2)];
    
    if (thisState == LATCHSTATE) {
      _positionExt = _position >> 2;
      _positionExtTimePrev = _positionExtTime;
      _positionExtTime = millis();
    }
    
    _oldState = thisState;
  } // if
} // tick()

unsigned long RotaryEncoder::getMillisBetweenRotations() const
{
  return _positionExtTime - _positionExtTimePrev; 
}

// Push Button debounce
#if defined ky40btn_debounce
void RotaryEncoder::write(int new_state)
       {
         this->state = new_state;
        digitalWrite(buttonPin,state);
       }


void RotaryEncoder::interval(unsigned long interval_millis)
{
  this->interval_millis = interval_millis;
  this->rebounce_millis = 0;
}

int RotaryEncoder::update()
{
  if ( debounce() ) {
        rebounce(0);
        return stateChanged = 1;
    }

     // We need to rebounce, so simulate a state change

  if ( rebounce_millis && (millis() - previous_millis >= rebounce_millis) ) {
        previous_millis = millis();
     rebounce(0);
     return stateChanged = 1;
  }

  return stateChanged = 0;
}


unsigned long RotaryEncoder::duration()
{
  return millis() - previous_millis;
}


int RotaryEncoder::read()
{
  return (int)state;
}


// Protected: debounces the pin
int RotaryEncoder::debounce() {

  uint8_t newState = digitalRead(buttonPin);
  if (state != newState ) {
      if (millis() - previous_millis >= interval_millis) {
        previous_millis = millis();
        state = newState;
        return 1;
  }
  }

  return 0;

}


// End Push Button debounce

#endif

// End