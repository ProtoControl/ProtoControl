// No additional library needed

// https://lastminuteengineers.com/joystick-interfacing-arduino-processing/

// ADC board address at 0x48



#include <Adafruit_ADS1X15.h>
Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */

const int SW_pin = 8; // digital pin connected to switch output
const int Y_pin = 0; // analog pin connected to Y output (20 to 26790)
const int X_pin = 1; // analog pin connected to X output (20 to 26790)


void setup() {
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);

  Serial.begin(9600);
  Serial.println("Hello!");

  Serial.println("Getting single-ended readings from AIN0..3");
  Serial.println("ADC Range: +/- 6.144V (1 bit = 3mV/ADS1015, 0.1875mV/ADS1115)");

  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
   ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV

  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }

}

void loop() {
  int16_t adc0, adc1;
  int16_t adc0_last = 0; 
  int16_t adc1_last = 0;
  float volts0, volts1;

  adc0 = ads.readADC_SingleEnded(X_pin);
  adc1 = ads.readADC_SingleEnded(Y_pin);

  volts0 = ads.computeVolts(adc0);
  volts1 = ads.computeVolts(adc1);

  if ((abs(adc0_last - adc0) >= 32) || (abs(adc1_last - adc1) >= 32)) {
    adc0_last = adc0;
    adc1_last = adc1;
    Serial.print("X-axis: ");
    Serial.print(adc0);
    Serial.print(" | ");
    Serial.print("Y-axis: ");
    Serial.print(adc1);
    Serial.println(" | ");
  }
  delay (200);
// convert range to 0 to 100 
}
