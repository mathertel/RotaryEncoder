#include <RotaryEncoder.h>

#define ROTARYSTEPS 1
#define ROTARYMIN -1
#define ROTARYMAX 36
#define START_POSITION 0

#define ENCODER_CW 6
#define ENCODER_CCW 5

#define ROTARY_BUTTON 8



// Setup a RoraryEncoder

RotaryEncoder encoder(ENCODER_CW, ENCODER_CCW, ROTARY_BUTTON, 10);  // with push button...
//RotaryEncoder encoder(ENCODER_CW, ENCODER_CCW);                   // ...or without


// Last known rotary position.
int lastPos = -1;

void setup() {

  Serial.begin(9600);

  encoder.setPosition(START_POSITION / ROTARYSTEPS);

}


void loop() {


  encoder.tick();

  // get the current physical position and calc the logical position
  int newPos = encoder.getPosition() * ROTARYSTEPS;

  if (newPos < ROTARYMIN) {
    encoder.setPosition(ROTARYMIN / ROTARYSTEPS);
    newPos = ROTARYMIN;

  } else if (newPos > ROTARYMAX) {
    encoder.setPosition(ROTARYMAX / ROTARYSTEPS);
    newPos = ROTARYMAX;

  }


  if (lastPos != newPos) {

    if (newPos>lastPos){

    Serial.println("Encoder rotated one position in one direction");

      }else{

    Serial.println("Encoder rotated one position in the other direction");
        }

    Serial.print(newPos);
    Serial.println();
    lastPos = newPos;


  }

  if (encoder.update() && encoder.read()==LOW){   // If push button is pressed...

  Serial.println("button pushed");

  }

}
