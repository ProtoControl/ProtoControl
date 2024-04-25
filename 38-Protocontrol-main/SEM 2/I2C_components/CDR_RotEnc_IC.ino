#include "Arduino.h"
#include <PCF8574.h>
#include "PCF8574.h"

// Set the i2c HEX address
PCF8574 pcf8574(0x20);

//#define encoder0PinA  0 
//#define encoder0PinB  1 
  int encoder0Pos = 0;
  int encoder0PinALast = 0;
  int n = 0;


void setup(){
  Serial.begin(9600);
  // Try with 115200

  // Set the pinModes
  pcf8574.pinMode(P0, INPUT);
  pcf8574.pinMode(P1, INPUT);
  // pcf8574.pinMode (encoder0PinA, INPUT);
  // pcf8574.pinMode (encoder0PinB, INPUT);
  pcf8574.pinMode(P2, INPUT_PULLUP);
  Serial.println("SetupStarted");

  pcf8574.begin();
  Serial.println("SetupComplete");

  //Serial.println(encoder0Pos);
  //Serial.println(n);
  //Serial.println(encoder0PinALast);
}

void loop() {
  //n = pcf8574.digitalRead(encoder0PinA);
  n = pcf8574.digitalRead(P1);
  //m = pcf.digitalRead(encoder0PinB);
  //  if ((encoder0PinALast == 0) && (n == 1)) {
  //    if (pcf8574.digitalRead(P1) == 0) {
  //      encoder0Pos--;
  //      Serial.println(encoder0Pos);
  //        encoder0PinALast = n;
  //    } else {
  //      encoder0Pos++;
  //      Serial.println(encoder0Pos);
  //    }
  //  }
  Serial.print("C:");
  Serial.println(n);
  //encoder0PinALast = n;
  if (pcf8574.digitalRead(P1)) {
    Serial.println("Button pressed ");
  }
}


// check voltages again DONE (very finnicky and not worth repeating)
// A4/A5 vs SDA/SCL DONE (doesn't make a diff)
// input_pullup vs input (pullup type w/o res, input w/o res, pullup w/ res, input w/ res)
// add pullup resistor DONE
// read the value of P0 and P1 on hello world
// add pull down resistor DONE

