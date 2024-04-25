
//arduino LED reciver

String msg;
#define led 12
#define button 7

bool buttonState = LOW;
bool lastButtonState = LOW;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("ready");
  pinMode(led,OUTPUT);
  pinMode(button,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    msg = Serial.readString();
    Serial.println(msg);
    if(msg.startsWith("0")){
      digitalWrite(led,LOW);
      Serial.println("of");
    }
    if(msg.startsWith("1")){
      digitalWrite(led,HIGH);
      Serial.println("on");
    }
  }

    // read the state of the pushbutton value:
  buttonState = digitalRead(button);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    if(lastButtonState){
      lastButtonState = LOW;
      Serial.println("U");
      delay(1000);
    }else{
      lastButtonState = HIGH;
      Serial.println("N");
      delay(1000);
    }
    
  }

  
}
