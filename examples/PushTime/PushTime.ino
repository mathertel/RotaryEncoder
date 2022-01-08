#include "RotaryEncoder.h"

#define PIN_IN1 2
#define PIN_IN2 3
#define PIN_IN3 4

#define PRESS_TIME_SHORT   40 // [ms] override default short press (20)
#define PRESS_TIME_LONG  3000 // [ms] override default long  press (2000)

RotaryEncoder *encoder = nullptr;

void checkPosition()
{
  encoder->tick(); // just call tick() to check the state.
}

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("InterruptRotator example for the RotaryEncoder library.");

  // setup the rotary encoder functionality

  // use FOUR3 mode when PIN_IN1, PIN_IN2 signals are always HIGH in latch position.
  // encoder = new RotaryEncoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::FOUR3);

  // use FOUR0 mode when PIN_IN1, PIN_IN2 signals are always LOW in latch position.
  // encoder = new RotaryEncoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::FOUR0);

  // use TWO03 mode when PIN_IN1, PIN_IN2 signals are both LOW or HIGH in latch position.
  // encoder = new RotaryEncoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::TWO03);

  // Push Button Example
  encoder = new RotaryEncoder(PIN_IN1, PIN_IN2, PIN_IN3, RotaryEncoder::LatchMode::TWO03);

  // register interrupt routine
  attachInterrupt(digitalPinToInterrupt(PIN_IN1), checkPosition, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_IN2), checkPosition, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_IN3), checkPosition, CHANGE);
}

void loop() {
  static int pos = 0;

  encoder->tick(); // just call tick() to check the state.

  int newPos = encoder->getPosition();
  if (pos != newPos) {
    Serial.print("pos:");
    Serial.print(newPos);
    Serial.print(" dir:");
    Serial.println((int)(encoder->getDirection()));
    pos = newPos;
  } // if

  if (encoder->getPushState() == encoder->PushTime::PUSHLONG) {
    Serial.println("long push");
  } else if (encoder->getPushState() == encoder->PushTime::PUSHSHORT) {
    Serial.println("short push");
  }
}