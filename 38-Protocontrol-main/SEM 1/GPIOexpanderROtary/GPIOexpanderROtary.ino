#include <Arduino.h>
#include <Adafruit_PCF8574.h>


#include <Wire.h>

Adafruit_PCF8574 pcf;
const int encoderPinA = 0; // Define the pins for the encoder
const int encoderPinB = 1;
int encoderValue = 0;
int lastEncoderValue = 0;

void setup() {
  Wire.begin(); // Initialize the I2C communication
  pcf.begin(0x20, &Wire);

  pcf.pinMode(encoderPinA, INPUT);
  //pcf.pullUp(encoderPinA, HIGH); // Enable internal pull-up resistor
  pcf.pinMode(encoderPinB, INPUT);
  //pcf.pullUp(encoderPinB, HIGH); // Enable internal pull-up resistor

  Serial.begin(9600);
  Serial.println("Rotary Encoder with MCP23017 Example");
}

void loop() {
  int encoderStateA = pcf.digitalRead(encoderPinA);
  int encoderStateB = pcf.digitalRead(encoderPinB);

  if (encoderStateA != encoderStateB) {
    // Clockwise rotation
    if (encoderStateA == HIGH) {
      encoderValue++;
    }
    // Counter-clockwise rotation
    else {
      encoderValue--;
    }
  }

  // Check for a change in value
  if (encoderValue != lastEncoderValue) {
    Serial.print("Encoder Value: ");
    Serial.println(encoderValue);
    lastEncoderValue = encoderValue;
  }
}
