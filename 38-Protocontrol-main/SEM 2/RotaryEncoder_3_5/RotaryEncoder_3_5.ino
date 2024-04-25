#include "Arduino.h"
#include <PCF8574.h>
#include "PCF8574.h"

// Set the i2c HEX address
PCF8574 pcf8574(0x21);
// change address lines

// parameters
const int MAX = 255;
const int MIN = 0; 
int n;
int running_count_new;
int running_count_old;
int raw_val_old = 0;
int raw_val_new; 

void setup(){
  Serial.begin(9600);

  // Set the pinModes
  pcf8574.pinMode(P0, INPUT);
  pcf8574.pinMode(P1, INPUT);
  Serial.println("SetupStarted");

  if (pcf8574.begin()) {
    Serial.println("SetupComplete");
  }

}

void loop() {
  raw_val_new = pcf8574.readEncoderValue(P0, P1);

  // convert 0 and -1 to a 
    if (raw_val_old != raw_val_new) {
      if (raw_val_new == 1) {
        running_count_new++;
        Serial.println(running_count_new);
      }
      else if (raw_val_new == -1) {
        running_count_new--;
        Serial.println(running_count_new);
      }
    }
  raw_val_old = raw_val_new;

  // keep track of running value
  // incorporate some kind of delay
  // direction??
  // if 0, then nothing has changed so we don't change our running val
  // if -1, then we need to either +/- 1 from the running total
  // We need to have a max and min
  // then we need this to control the rgb led
  // sample

  /* Adjust values: Min and Max
    if (running_count > MAX) {
      running_count  = 0;
    }

    if (running_count < MIN ) {
      running_count = 0;
    }

  */

  /* Detect a Change
    if (running_count_new != running_count_old) {
      println (running_count_new);
    }

  */

  // save old val
  // old_val = new_val; 

}

// Integrating Rotary Encoder with Protocontrol
// Scan function runs upon implementation 
// Rotary Encoder Class 
// Create instance of all pcf chips 
// case statement to figure out which begin statement 
// one pin expander gives two kinds of data
// Rotary Encoder: N"encodervalue" 
// Min/Max


