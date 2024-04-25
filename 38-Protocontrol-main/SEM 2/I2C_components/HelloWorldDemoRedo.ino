#include <Arduino.h>
#include <Adafruit_PCF8574.h>
#include <Wire.h>

Adafruit_PCF8574 pcf; // initialize instance of the pin expander

#define encoder0PinA  0 // 
#define encoder0PinB  1 // 
#define LED_PIN   7  // MCP23XXX pin LED is attached to
#define BUTTON_UP 6 // IN sw5?
#define BUTTON_LEFT 3 // sw2?
#define BUTTON_DOWN 4 // sw3?
#define BUTTON_RIGHT 5 // sw4?
#define BUTTON_IN 2 //center, sw1?

int val;
int encoder0Pos = 0;
int encoder0PinALast = LOW;
int n = LOW;
//int m = HIGH;

void setup() {
  delay (1000);
  Serial.begin(9600);
  Serial.println("Rotary Encoder with PCF8574 Example");

 // Initialize the I2C communication
  pcf.begin(0x20, &Wire);

 
  pcf.pinMode (encoder0PinA, INPUT);
  pcf.pinMode (encoder0PinB, INPUT);

  pcf.pinMode(LED_PIN, OUTPUT);
  // Pin configurations for msp pins

  // configure all button pins for input with pull up
  pcf.pinMode(BUTTON_UP, INPUT_PULLUP);
  pcf.pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pcf.pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pcf.pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pcf.pinMode(BUTTON_IN, INPUT_PULLUP);
 
  
  
}

void loop() {
  n = pcf.digitalRead(encoder0PinA);
  //m = pcf.digitalRead(encoder0PinB);
  if ((encoder0PinALast == LOW) && (n == HIGH)) {
    if (pcf.digitalRead(encoder0PinB) == LOW) {
      encoder0Pos--;
    } else {
      // why does this work, shouldn't encoder0Pos stay the same if encoder0PinB is not LOW?
      encoder0Pos++;
    }
    Serial.println(encoder0Pos);
  }
  encoder0PinALast = n;

  pcf.digitalWrite(LED_PIN, !pcf.digitalRead(BUTTON_IN));
  if (!pcf.digitalRead(BUTTON_UP)) {
    //Serial.println("Up button pressed ");
  }
  if (!pcf.digitalRead(BUTTON_LEFT)) {
    //Serial.println("Left button pressed ");
  }
  if (!pcf.digitalRead(BUTTON_DOWN)) {
    //Serial.println("Down button pressed ");
  }
  if (!pcf.digitalRead(BUTTON_RIGHT)) {
    //Serial.println("Right button pressed ");
  }
  if (!pcf.digitalRead(BUTTON_IN)) {
    //Serial.println("Center button pressed ");
  }
}

// How do we get both directions? 
// This code is successful in detecting a change in position but not a change in direction
// 

