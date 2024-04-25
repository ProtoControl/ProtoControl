//This example implements a simple sliding On/Off button. The example
// demonstrates drawing and touch operations.
//
//Thanks to Adafruit forums member Asteroid for the original sketch!
//
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>
#include "elements.h"
#include <SD.h>
#include "Adafruit_seesaw.h"
#include <seesaw_neopixel.h>
#define SEESAW_ADDR          0x38
//#include <vector>
#include <PCF8574.h>

#include "Arduino.h"
#include "PCF8574.h"
Adafruit_seesaw ss;
PCF8574 pcf8574(0x20);
// The FT6206 uses hardware I2C (SCL/SDA)
Adafruit_FT6206 ts = Adafruit_FT6206();
#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
boolean RecordOn = false;
bool pressed = false;
#define FRAME_X 210
#define FRAME_Y 180
#define FRAME_W 100
#define FRAME_H 50
#define REDBUTTON_X FRAME_X
#define REDBUTTON_Y FRAME_Y
#define REDBUTTON_W (FRAME_W/2)
#define REDBUTTON_H FRAME_H
#define GREENBUTTON_X (REDBUTTON_X + REDBUTTON_W)
#define GREENBUTTON_Y FRAME_Y
#define GREENBUTTON_W (FRAME_W/2)
#define GREENBUTTON_H FRAME_H
#define background ILI9341_LIGHTGREY
#define BOX 218
#define cardSelect 4


//dummy classes

PushButton b1;
Toggle t1;
increment i1;
bool lastState = false;
File logfile;
char con[3][16];
char previousDisplay[3][16];
String parse;
String command;
int slide = 60;
int prevSlide =0 ;
int ymap = 60;
int LED = 13;
char dist[10];
int32_t encoder_position;
#define SS_SWITCH        24
#define SS 6
bool on;
bool status = false;
int recVal = 0;
byte Mastereceive;
byte Mastersend = 1;
String inits[15][15];
char *decode;
char *values;
String message;
//PushButton b1 = PushButton(20,20,ILI9341_GREEN,ILI9341_WHITE,"PRESS");
//PushButton b2 = PushButton(180,90,ILI9341_BLUE,ILI9341_WHITE,"PRESS");


// Toggle t1 = Toggle(210,180,ILI9341_WHITE);
// Toggle t2 = Toggle(20,50,ILI9341_BLACK);
// increment i1 = increment(20,50,ILI9341_WHITE);
// increment i2 = increment(100,30,ILI9341_WHITE);
bool readD = false;
bool readL = false;
char filename[15] = "data.txt";
int indexD = 0;
int inc = 0;
int line = 0;
void drawFrame()
{
  tft.drawRect(FRAME_X, FRAME_Y, FRAME_W, FRAME_H, ILI9341_BLACK);
}

void pressBtn(int x,int y, int w,int h, int color,int textColor,char text[]){
  tft.fillRect(x,y,w,h,color);
  tft.drawRect(x,y,w,h,ILI9341_BLACK);
  tft.setCursor(x+15,y+(h/3));
  tft.setTextColor(textColor);
  tft.setTextSize(3);
  tft.println(text);
}
void console(int x,int y, int w,int h){
  tft.fillRect(x,y,w,h,ILI9341_BLACK);
  tft.drawRect(x,y,w,h,ILI9341_WHITE);
}
void setup(void)
{
  Serial.begin(115200);
  Serial1.begin(115200);
  SPI.begin();
  digitalWrite(SS,HIGH);     
  pinMode(LED, OUTPUT); // input button 2
  pcf8574.pinMode(P0, OUTPUT);
  pcf8574.pinMode(P1, INPUT_PULLUP);
  pcf8574.begin();
  Serial.println("SetupComplete");
  tft.begin();
  if (!ts.begin(40)) {
    Serial.println("Unable to start touchscreen.");
   }
  else {
    Serial.println("Touchscreen started.");
  }
  tft.fillScreen(background);
  // origin = left,top landscape (USB left upper)
  tft.setRotation(1);
  // b1.drawButton(tft);
  // b2.drawButton(tft);
  // //t1.drawToggle(tft);
  // //t2.drawToggle(tft);
  // i1.drawIncrement(tft);
  // i2.drawIncrement(tft);
  //console(20,20,150,80);
  delay(1000);
  console(20,20,130,75);
  //SD card init
  if (!SD.begin(cardSelect)) {
    Serial.println("Card init. failed!");
  }
  //init ss rotary encoder over i2c
  if (!ss.begin(0x38)) {
    Serial.println("Couldn't find seesaw on default address");
    //while(1) delay(10);
  }
  Serial.println("seesaw started");
  //encoder_position = ss.getEncoderPosition();
  Serial.println("Turning on interrupts");
  delay(10);
  ss.setGPIOInterrupts((uint32_t)1 << SS_SWITCH, 1);
  ss.enableEncoderInterrupt();


  inc = 0;
  line = 0;
          logfile = SD.open(filename);
        if(!logfile) {
          Serial.print("Couldn't open ");
          Serial.println(filename);
        } else {
          Serial.print("reading from ");
          Serial.println(filename);
        }
        inc = 0;
        line = 0;
        while (logfile.available()) {
            
            parse = logfile.readStringUntil(',');
            //Serial.write(parse);
            Serial.println(parse);
            inits[line][inc] = parse;
            inc++;
            if(parse == "|"){
              line++;
              inc = 0;
              Serial.print(parse.c_str());
              Serial.print("^");
            }
            
            
          }
      
        Serial.println(line);
        Serial.println(inits[1][0]);
        Serial.println(inits[1][1]);
        Serial.println(inits[1][2]);
        Serial.println(inits[1][3]);
        // Serial.print("<-");
        for(int i = 0;i<=line;i++){
              String cmp = "PushButton";
              if(inits[i][0] == cmp){
                b1 = PushButton(inits[i][1].toInt(),inits[i][2].toInt(),strtoul(inits[i][3].c_str(),NULL,16),strtoul(inits[i][4].c_str(),NULL,16),inits[i][5].c_str());
                b1.drawButton(tft);
              }
              cmp = "Toggle";
              if(inits[i][0] == cmp){
                  t1 = Toggle(inits[i][1].toInt(),inits[i][2].toInt(),strtoul(inits[i][3].c_str(),NULL,16));
                  t1.drawToggle(tft);
              }
              cmp = "increment";
              if(inits[i][0] == cmp){
                  i1 = increment(inits[i][1].toInt(),inits[i][2].toInt(),strtoul(inits[i][3].c_str(),NULL,16));
                  i1.drawIncrement(tft);
              }
        }
      
  
}
  

  //delay(500);




void loop()
{
  
  
  if (Serial1.available()){
    message = Serial1.readStringUntil('\n'); // read message 
    Serial.println(message);
    
    
    if (message.startsWith("+")){
      Serial.println("COMM");
      strcpy(con[2],message.substring(3).c_str());
    }
    
   
  }
  char msg;
  //testing protocal on Serial, not Serial1 to update display with loaded digital components
  if(Serial.available()){
    msg = Serial.read();
    if(msg == 'U'){
        File logfile;
        logfile = SD.open(filename);
        if(!logfile) {
          Serial.print("Couldn't open ");
          Serial.println(filename);
        } else {
          Serial.print("reading from ");
          Serial.println(filename);
        }
        inc = 0;
        line = 0;
        while (logfile.available()) {
            
            parse = logfile.readStringUntil(',');
            //Serial.write(parse);
            Serial.println(parse);
            inits[line][inc] = parse;
            inc++;
            if(parse == "|"){
              line++;
              inc = 0;
              Serial.print(parse.c_str());
              Serial.print("^");
            }
            
            
          }
      
        Serial.println(line);
        Serial.println(inits[1][0]);
        Serial.println(inits[1][1]);
        Serial.println(inits[1][2]);
        Serial.println(inits[1][3]);
        // Serial.print("<-");
        for(int i = 0;i<=line;i++){
              String cmp = "PushButton";
              if(inits[i][0] == cmp){
                b1 = PushButton(inits[i][1].toInt(),inits[i][2].toInt(),strtoul(inits[i][3].c_str(),NULL,16),strtoul(inits[i][4].c_str(),NULL,16),inits[i][5].c_str());
                b1.drawButton(tft);
              }
              cmp = "Toggle";
              if(inits[i][0] == cmp){
                  t1 = Toggle(inits[i][1].toInt(),inits[i][2].toInt(),strtoul(inits[i][3].c_str(),NULL,16));
                  t1.drawToggle(tft);
              }
              cmp = "increment";
              if(inits[i][0] == cmp){
                  i1 = increment(inits[i][1].toInt(),inits[i][2].toInt(),strtoul(inits[i][3].c_str(),NULL,16));
                  i1.drawIncrement(tft);
              }
        }
    }
  }

  
  
  tft.setCursor(200,20);
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_BLACK);
  
  if (! ss.digitalRead(SS_SWITCH)) {
    //Serial.println("Button pressed!");
  }

  int32_t new_position = ss.getEncoderPosition();
  // did we move arounde?
  if (encoder_position != new_position) {
    Serial.println(new_position);         // display new position
    encoder_position = new_position;      // and save for next round
  }
  if(pcf8574.digitalRead(P1)){
    strcpy(con[0],"PRESSED");
  }else{
    strcpy(con[0],"NONE");
  }

  
  //Serial.println(encoder_position);
  //itoa(encoder_position,con[0],10);
  display(con);

  // See if there's any  touch data for us
  if (ts.touched())
  {
    // Retrieve a point
    TS_Point p = ts.getPoint();
    // rotate coordinate system
    // flip it around to match the screen.
    p.x = map(p.x, 0, 240, 240, 0);
    p.y = map(p.y, 0, 320, 320, 0);
    int y = tft.height() - p.x;
    int x = p.y;
    //swap vars
    int temp = p.y;
    p.y = p.x;
    p.x = temp;
    // Serial.print("P.x: ");
    // Serial.println(p.x);
    // Serial.print("P.y: ");


    b1.handleTouch(p.x,p.y,tft);
    // b2.handleTouch(p.x,p.y,tft);
    t1.handleTouch(p.x,p.y,tft);
    if(t1.state != lastState){
      if(t1.state){
        Serial1.println("+o|on");
        Serial.println("on");
      }else{
        Serial1.println("+o|off");
        Serial.println("off");
        }
      lastState = t1.state;
    }
    }
    // t2.handleTouch(p.x,p.y,tft);
    //i1.handleTouch(p.x,p.y,tft);

//slider
 
    //Serial.println(RecordOn);
  }


void display(char c[3][16]){
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 16; j++) {
      if (c[i][j] != previousDisplay[i][j]) {
        // Update the display for that character
        tft.setCursor(25 + j * 12, 25 + i * 10);  // 12 is approx width of a character with textSize(2), adjust as necessary
        tft.setTextColor(ILI9341_BLACK);
        tft.setTextSize(2);
        tft.print((char)BOX);
        tft.setCursor(25 + j * 12, 25 + i * 10);
        tft.setTextColor(ILI9341_WHITE);
        tft.print(c[i][j]);
        // Update the previous display content
        previousDisplay[i][j] = c[i][j];
      }
    }
  }
}



