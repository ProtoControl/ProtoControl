//This sketch is to connect the 10k slide potentiometer directly to the arduino uno
// single leg goes to ground (bottom right leg)
// top right leg goes to 5v
// top left leg toes to A0
// https://forum.arduino.cc/t/how-to-wire-slide-potentiometer/283146
// https://forum.arduino.cc/t/10k-slide-potentiometer-wiring/674234
// https://mschoeffler.com/2020/11/14/arduino-tutorial-slide-potentiometer-slide-pot-controls-led-strip-ws2812b/

#define PIN_SLIDE_POT_A A0


void setup() {
  Serial.begin(9600);
  pinMode(PIN_SLIDE_POT_A, INPUT );
}

void loop() {
  int value_slide_pot_last = 0;
  int value_slide_pot_a = analogRead(PIN_SLIDE_POT_A);

  if (abs(value_slide_pot_last - value_slide_pot_a) >= 4) {
    value_slide_pot_last = value_slide_pot_a;
    Serial.print("Slide Pot value: ");
    Serial.println(value_slide_pot_a);
  }
  delay(200);

}
