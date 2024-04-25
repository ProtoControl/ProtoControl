// new arduino user system
// includes
#include <Adafruit_NeoPixel.h>  


// constants
#define PIN            6               // LED on digital pin 6
#define NUMLEDS        1               // Use total of 1 LED
#define FALSE          0
#define TRUE           1
#define BuzzerPin      13


// Keypad Things
char ourCode[4] = "1974";      // Set the required PIN code.
int index = 0;     // Keeps track of the position of the code entered.
char userEnteredPin[4];


// Ultrasonic Sensor Things
const int trigPin = 9;
const int echoPin = 10;
int uss_value;


// RGB LED Things
Adafruit_NeoPixel rgb_led = Adafruit_NeoPixel(NUMLEDS, PIN, NEO_GRB + NEO_KHZ800);


// define variables
long duration;
bool alarm_state = false;
bool armed = false;

void getPinCode(char key) {

     userEnteredPin[index] = key;
    ++index;
    Serial.println(userEnteredPin);
    if (index == 4) {
	    index = 0;
	    if (strcmp(userEnteredPin, ourCode) == 0 ) {
        disarmAlarm();
      armed = false;
      Serial.println("DISARMED");
	  }
   	else {
     		invalidCode();                           
    }
    }

}


long getSensorData() {
 // Clears the trigPin
 digitalWrite(trigPin, LOW);
 delayMicroseconds(2);
 // Sets the trigPin on HIGH state for 10 micro seconds
 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPin, LOW);
 // Reads the echoPin, returns the sound wave travel time in microseconds
 duration = pulseIn(echoPin, HIGH);
 return duration;
}


void alarm() {
 // change the display in J to be alarm ON
 // buzzer on
 
  alarm_state = true;
  digitalWrite(BuzzerPin, HIGH);
 // Red Light On
  rgb_led.setPixelColor(0, rgb_led.Color(255, 0, 0));  // Color mix for LED
  rgb_led.show();                                 // Set the LED
  delay(5);
 
                                    // Delay for visual effect
 }


void invalidCode(){
  Serial.println("invalid code");
 // display code incorrect
}


void disarmAlarm(){
 alarm_state = false;
 // turn the lights off
 rgb_led.setPixelColor(0, rgb_led.Color(0, 0, 0));  // Turn off LED
 rgb_led.show();                                 // Set new value
 delay(5);


 // buzzer off
 digitalWrite(BuzzerPin, LOW);


 // maybe temporarily freeze uss_value while the person moves away?
}


// serial.printline
void setup() {
 Serial.begin(115200);


 // pin modes
 pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
 pinMode(echoPin, INPUT); // Sets the echoPin as an Input
 pinMode(BuzzerPin, OUTPUT);


 // initialize the rgb led
 rgb_led.begin();     


 //Make sure LED is off to demonstrate start of sketch
 rgb_led.setPixelColor(0, rgb_led.Color(0, 0, 0));  // Turn off LED
 rgb_led.show();                                 // Set new value


 Serial.println("Setup Complete");


}


void loop() {
// read sensor1 value; display to A
 uss_value = getSensorData();
 //Serial.print("UltraSonic Sensor Value: ");
 //Serial.println(uss_value);
 // delay(10);
 if (uss_value <= 800) {
  if(armed){
   //Serial.println("alarm on");
   alarm();
  }
 }
 else if (uss_value > 800) {
   //Serial.println("alarm off");
   disarmAlarm();
 }
 

if(Serial.available()){
   String communication = Serial.readStringUntil('\n');
   Serial.println(communication);
   if(communication[0] == 'M'){
     communication.remove(0,1);
     if(communication[0] == '*'){
      armed = true;
      Serial.println("ARMED");
     }else{
      char key_value = communication[0];
      getPinCode(key_value);
     }
   }
 }

}


