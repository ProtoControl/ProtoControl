//example user system for testing
//parker sexton 2/20/24

#include <Servo.h>

int sensorPin = A0;   // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor
int lastValue;
Servo arm;
String communication;

//RGB led init variables
int redPin= 5;
int greenPin = 6;
int  bluePin = 3;
int red = 0;
int green = 0;
int blue = 0;
//function to change color of LED based on input vars
void setColor(int redValue, int greenValue,  int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin,  greenValue);
  analogWrite(bluePin, blueValue);
}
void setup() {
  // declare the ledPin as an OUTPUT:
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  arm.attach(9);

  //led setup
  pinMode(redPin,  OUTPUT);              
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(4,OUTPUT);
}

void loop() {
  // set the color with local ints
  setColor(red,green,blue);

  //read the value from the light sensor
  sensorValue = analogRead(sensorPin); //max 715 min 0
  sensorValue = map(sensorValue,0,800,0,100);
//if its changed, call send data to update display
  if(abs(lastValue - sensorValue) >= 4){
    lastValue = sensorValue;
    //sendData(sensorValue);
    delay(200);
  }
  if(Serial.available()){
    char id = Serial.read();
    int val = Serial.read();
    Serial.println(id+(String)val);
    if(val){
      Serial.println("PRESS");
      sendData("RESEET");
    }
    //communication = Serial.read();
    //Serial.println(communication);
    //arm.write(communication.toInt()*10);
    if(communication[0] == 'A'){
      communication.remove(0,1);
      red = communication.toInt();
    }
    if(communication[0] == 'D'){
      communication.remove(0,1);
      green = communication.toInt();
    }
    if(communication[0] == 'G'){
      communication.remove(0,1);
      blue = communication.toInt();
    }
    if(communication[0] == 'M'){
      communication.remove(0,1);
      arm.write(communication.toInt()*10);
    }
    if(communication[0] == 'N'){
      communication.remove(0,1);
      arm.write(communication.toInt());
    }
    if(communication[0] == 'J'){
      communication.remove(0,1);
      digitalWrite(4,communication.toInt());
    }
    

    
  }

}


void sendData(String data){
  Serial.println("+"+data);
  //Serial.println(data);
}



