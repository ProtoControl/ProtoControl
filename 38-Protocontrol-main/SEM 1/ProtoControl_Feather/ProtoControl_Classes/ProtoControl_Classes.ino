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

char con[3][16];
char previousDisplay[3][16];
int slide = 60;
int prevSlide =0 ;
int ymap = 60;



PushButton b1 = PushButton(20,180,120,50,ILI9341_GREEN,ILI9341_WHITE,"PRESS");
PushButton b2 = PushButton(200,20,150,60,ILI9341_BLUE,ILI9341_WHITE,"PRESS");

void drawFrame()
{
  tft.drawRect(FRAME_X, FRAME_Y, FRAME_W, FRAME_H, ILI9341_BLACK);
}
void redBtn()
{
  tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, ILI9341_RED);
  tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, ILI9341_BLUE);
  drawFrame();
  tft.setCursor(GREENBUTTON_X + 6 , GREENBUTTON_Y + (GREENBUTTON_H/2));
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("ON");
  RecordOn = false;
}
void greenBtn()
{
  tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, ILI9341_GREEN);
  tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, ILI9341_BLUE);
  drawFrame();
  tft.setCursor(REDBUTTON_X + 6 , REDBUTTON_Y + (REDBUTTON_H/2));
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("OFF");
  RecordOn = true;
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
  redBtn();
  b1.drawButton(tft);
 // b2.drawButton(tft);
  console(20,20,150,80);
}
void loop()
{
  
  display(con);
  // b1.drawButton();
  // b1.handleTouch(10,10);
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
    Serial.print("P.x: ");
    Serial.println(p.x);
    Serial.print("P.y: ");
    Serial.println(p.y);
    if (RecordOn)
    {
      if((x > REDBUTTON_X) && (x < (REDBUTTON_X + REDBUTTON_W))) {
        if ((y > REDBUTTON_Y) && (y <= (REDBUTTON_Y + REDBUTTON_H))) {
          Serial.println("Red btn hit");
          redBtn();
          strcpy(con[0],"red");
          //display(con);
        }
      }
    }
    else //Record is off (RecordOn == false)
    {
      if((x > GREENBUTTON_X) && (x < (GREENBUTTON_X + GREENBUTTON_W))) {
        if ((y > GREENBUTTON_Y) && (y <= (GREENBUTTON_Y + GREENBUTTON_H))) {
          Serial.println("Green btn hit");
          greenBtn();
          strcpy(con[2],"blu");
          //display(con);
        }
      }
    }
//  pressBtn(20,180,120,50,ILI9341_GREEN,ILI9341_WHITE,"PRESS");
//pressbutton touch detection
/*
    if((p.y < 130) && p.y > 20 ){
      if((p.x > 5) && (p.x < 50)){
        //Serial.println("PRESSERNSNKDNO");
        if(pressed){
          pressed = false;
          strcpy(con[2],"none");
          display(con);
          delay(50);
        }else{
          pressed = true;
          strcpy(con[2],"press");
          display(con);
          delay(50);

        }

      }
    }
*/
    b1.handleTouch(p.x,p.y);
    if(b1.pressed){
      strcpy(con[2],"press");
      //b1.updateDisplay(con,tft);
      delay(50);
    }else{
      strcpy(con[2],"none");
      //b1.updateDisplay(con,tft);
      delay(50);
    }
    // b2.handleTouch(p.x,p.y);
    // if(b2.pressed){
    //   strcpy(con[1],"pss");
    //   b2.updateDisplay(con,tft);
    //   delay(50);
    // }else{
    //   strcpy(con[1],"noe");
    //   b2.updateDisplay(con,tft);
    //   delay(50);
    // }

    if(p.x > 75 && p.x < 125 && p.y > 20 && p.y < 300){
      tft.fillCircle(ymap+20,280,27,ILI9341_CYAN);
      slide = p.y;
      if(slide > prevSlide){
        tft.fillRect(prevSlide,125,slide-prevSlide,30,ILI9341_RED);
        }
      else{
        tft.fillRect(slide,125,prevSlide-slide,30,ILI9341_CYAN);
      }
      prevSlide = slide;
      ymap = slide;
      char s[5] = {'\0'};
      strcpy(con[2],"Slider: ");
      itoa(ymap,s,10);
      strcat(con[2], s);
      //display(con);
  }
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


