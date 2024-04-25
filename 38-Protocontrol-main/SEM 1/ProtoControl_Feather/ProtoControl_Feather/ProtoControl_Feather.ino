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

//#include <vector>


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

PushButton b1;
Toggle t1;
increment i1;

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

#define SS 6
bool on;
bool status = false;
int recVal = 0;
byte Mastereceive;
byte Mastersend = 1;
String inits[15][15];
char *values;
char message;
// PushButton b1 = PushButton(80,180,ILI9341_GREEN,ILI9341_WHITE,"PRESS");
// PushButton b2 = PushButton(180,90,ILI9341_BLUE,ILI9341_WHITE,"PRESS");
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
  //SD card init
  if (!SD.begin(cardSelect)) {
    Serial.println("Card init. failed!");
  }
  
  
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
  
  // tft.setCursor(20,20);
  // tft.setTextSize(3);
  // tft.println(inits[0]);
  //delay(500);




void loop()
{
  
  
  if (Serial1.available()){
    message = Serial1.read(); // read message 
    Serial.println(message);
    //strcpy(con[2],&message);
    if (message == '+')     // if the message contain character one 
    {
      if(on){
        Serial.println("TURNED ON");
        on = false;
      }else{
        Serial.println("TURNED OFF");
        on = true;
      }
      
    }
    if(message == 'D'){
      readD = true;
    }
   
  }
  
  //testing protocal on Serial, not Serial1 to update display with loaded digital components
  if(Serial.available()){
    message = Serial.read();
    if(message == 'U'){
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
    // Serial.println(p.y);

    //Factory.handleComponents();
    // void handleComponents(){
//    for component in components
    //}












    //strcpy(message,con[2]);

    //P
    b1.handleTouch(p.x,p.y,tft);
    // b2.handleTouch(p.x,p.y,tft);
    t1.handleTouch(p.x,p.y,tft);
    // t2.handleTouch(p.x,p.y,tft);
    //i1.handleTouch(p.x,p.y,tft);

//slider
 
    //Serial.println(RecordOn);
  }
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

// void generate(String arr[15]){
//   switch(arr[0]){
//     case "PushButton":
//       PushButton b1 = PushButton(arr[1],arr[2],arr[3],arr[4],arr[5]);
      
//     break;
//   }
// }


