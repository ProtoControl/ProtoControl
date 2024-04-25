



String message;
// defines pins numbers
const int trigPin = 9;
const int echoPin = 10;
const int ledPin = 13;
const int motorPin = 5;
// defines variables
long duration;
int distance;
int prev;
int prevlight;
int speed;
int light;
int ret = false;

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(ledPin,OUTPUT);
  Serial.begin(115200); // Starts the serial communication
}

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  if(Serial.available()){
      message = Serial.readString();

  }
    

  itoa(speed,message.c_str(),10);
  
  // Prints the distance on the Serial Monitor
  //speed = analogRead(A5);
  //speed = map(speed,0,1023,0,255);
  prevlight = analogRead(A0) / 4;
  if(prevlight != light){
     light = prevlight;
    Serial.print("+L: ");
    Serial.print(light);
    ret = true;
 }
    
  if (speed >= 0 && speed <= 255) {
         analogWrite(motorPin, speed);
      }
 // Serial.println(speed);
  // if(Serial.available()){
  //   message = Serial.readString();
  //   Serial.println(message);
  // }
  if(ret){
    ret = false;
    Serial.println();
  }
  delay(300);
}