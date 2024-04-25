
#include <ESP32Servo.h>

Servo myservo_13;  // Servo on GPIO 13
Servo myservo_12;  // Servo on GPIO 12

int pos = 0;
int servoPin_13 = 13;
int servoPin_12 = 12;
int cameraCenterX = 960;
int cameraCenterY = 540;
bool isMoving = false;  // State to track whether servos are moving


void setup() {
  pinMode(2, OUTPUT); // LED pin
  Serial.begin(115200); // Start serial communication
  Serial2.begin(115200);
  Serial.println("Setup complete. Waiting for data...");

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  myservo_13.setPeriodHertz(50);
  myservo_13.attach(servoPin_13, 500, 2500); // Set range for servo 13
  myservo_12.setPeriodHertz(50);
  myservo_12.attach(servoPin_12, 500, 2500); // Set range for servo 12

  // Set initial position of both servos to 90 degrees
  myservo_13.write(90);
  myservo_12.write(90);
}

void loop() {
  //ProtoControl Integratiion
  if(Serial2.available()){
    char id = Serial2.read();
    int value = Serial2.read();
    if(id == 'A'){
      myservo_13.write(90);
      myservo_12.write(90);
    }
    if(id == 'O'){
      myservo_13.write(value);
    }
    if(id == 'P'){
      myservo_12.write(value);
    }
  }

  
  if (Serial.available() > 0 && !isMoving) {
    String data = Serial.readString();
    data.trim(); // Clean up data
    Serial.print("Received: ");
    Serial.println(data);
    
    // Process the incoming data to control servos
    int x = getValue(data, 'X', 'Y'); // Parse X coordinate
    int y = getValue(data, 'Y', 'W'); // Parse Y coordinate
    int width = getValue(data, 'W', 'H'); // Optionally parse WIDTH
    int height = getValue(data, 'H', 'Z'); // Optionally parse HEIGHT

    // Calculate center of the bounding box
    int bboxCenterX = x + width / 2;
    int bboxCenterY = y + height / 2;
    //myservo_13.write(x); // Move servo 13 based on X
    //myservo_12.write(y); // Move servo 12 based on Y
    // Calculate error (difference between center of bbox and center of camera frame)
    int errorX = cameraCenterX - bboxCenterX;
    int errorY = cameraCenterY - bboxCenterY;

    // Control servos to minimize error
    controlPan(errorX);
    controlTilt(errorY);
    //Serial1.println("+MESSAGE");
    digitalWrite(2, HIGH); // Indicate reception
    delay(100);
    digitalWrite(2, LOW);
  }
}

// Helper function to parse integer values between specified start and end characters
int getValue(String data, char start, char end) {
  int startIndex = data.indexOf(start) + 1;
  int endIndex = data.indexOf(end);
  if (startIndex > 0 && endIndex > 0) {
    return data.substring(startIndex, endIndex).toInt();
  }
  return 0; // Default to 0 if parsing fails
}

// Function to control the pan servo (horizontal movement)
// Function to ramp the servo position smoothly with controlled speed
void rampServoToPosition(Servo &servo, int startPos, int endPos, int delayMs) {
    isMoving = true;  // Set the moving state to true
    int step = (endPos > startPos) ? 1 : -1; // Determine the direction of the ramp
    while (startPos != endPos) {
        if (Serial.available() > 0) {  // Check if new data is available
            break;  // Break the loop to handle new data immediately
        }
        servo.write(startPos);
        startPos += step;
        delay(delayMs); // Delay to allow the servo to move; adjust as necessary for smoothness
    }
    isMoving = false;
}


void controlPan(int error) {
    Servo &servo = myservo_13;
    int currentPos = servo.read();
    int stepSize;

    if (error > 180) {
        stepSize = 7.5;
    } else if (error < -180) {
        stepSize = -7.5;
    } else {
        if( error > 90 || error < -90 ){
        stepSize = error / 7.5; // Adjust proportional control for finer movements
        } else {
          stepSize = 0;
        }
    }

    int newPos = currentPos + stepSize;
    newPos = constrain(newPos, 0, 180);

    // Ramp to the new position with a controlled delay of 15ms
    rampServoToPosition(servo, currentPos, newPos, 50);
}

void controlTilt(int error) {
    Servo &servo = myservo_12;
    int currentPos = servo.read();
    int stepSize;

    if (error > 180) {
        stepSize = 7.5;
    } else if (error < -180) {
        stepSize = -7.5;
    } else {
        if( error > 90 || error < -90 ){
        stepSize = error / 15; // Adjust proportional control for finer movements
        } else {
          stepSize = 0;
        }
    }

    int newPos = currentPos + stepSize;
    newPos = constrain(newPos, 0, 180);

    // Ramp to the new position with a controlled delay of 15ms
    rampServoToPosition(servo, currentPos, newPos, 50);
}
