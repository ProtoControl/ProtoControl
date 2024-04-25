

#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>
#include "Adafruit_HX8357.h"
#include "TouchScreen.h"
#include "Adafruit_seesaw.h"
#include <seesaw_neopixel.h>

#define SS_SWITCH        24
#define SS_NEOPIX        6

#define SEESAW_ADDR          0x36

Adafruit_seesaw ss;
seesaw_NeoPixel sspixel = seesaw_NeoPixel(1, SS_NEOPIX, NEO_GRB + NEO_KHZ800);



int32_t encoder_position;

// These are the four touchscreen analog pins
#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 8   // can be a digital pin
// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 110
#define TS_MINY 80
#define TS_MAXX 900
#define TS_MAXY 940

#define MINPRESSURE 10
#define MAXPRESSURE 1000

#define RECTWIDTH 200
#define RECTHEIGHT 100
#define RECTXORIGIN 20
#define RECTYORIGIN 20
// The display uses hardware SPI, plus #9 & #10
#define tft_RST -1  // dont use a reset pin, tie to arduino RST if you like
#define tft_DC 9
#define tft_CS 10
bool on = false;;
Adafruit_HX8357 tft = Adafruit_HX8357(tft_CS, tft_DC, tft_RST);
int ymap = 60;
// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
int slide = 60;
// Size of the color selection boxes and the paintbrush size
#define BOXSIZE 40
#define PENRADIUS 3
bool firstRun = true;
char con[3][16];
int oldcolor, currentcolor;
int rad = PENRADIUS;

void setup(void) {
 // used for leonardo debugging


  Serial.begin(115200);
  //Serial.println(F("Touch Paint!"));
  if (! ss.begin(SEESAW_ADDR) || ! sspixel.begin(SEESAW_ADDR)) {
    Serial.println("Couldn't find seesaw on default address");
  }else{
      Serial.println("seesaw started");
  }
  //init LED and set pin for button
  sspixel.setBrightness(20);
  sspixel.show();
  ss.pinMode(SS_SWITCH, INPUT_PULLUP);


  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  tft.begin();
  tft.fillScreen(HX8357_CYAN);

  // Generate RED PRESS BUTTON
  tft.fillRect(20, 20, 100, 200, HX8357_GREEN);
  tft.drawRect(20, 20, 100, 200, HX8357_BLACK);
  tft.setRotation(3);
  tft.setCursor(300,55);
  tft.setTextColor(HX8357_WHITE);
  tft.setTextSize(4);
  tft.println("Press");

  //draw text window
  tft.fillRect(20, 20, 220, 100, HX8357_BLACK);
  tft.drawRect(20,20,220,100,HX8357_WHITE);
  //draw off button
  tft.fillRect(260, 140, 200, 100, HX8357_BLUE);
  tft.drawRect(260, 140, 200, 100, HX8357_BLACK);
  tft.setCursor(320,170);
  tft.setTextColor(HX8357_WHITE);
  tft.setTextSize(4);
  tft.println("Off");


  //slider
  tft.fillRect(20,265,450,30,HX8357_RED);
  tft.fillCircle(slide,280,25,HX8357_BLACK);
  //get encoder starting position
  encoder_position = ss.getEncoderPosition();
  delay(10);
  ss.setGPIOInterrupts((uint32_t)1 << SS_SWITCH, 1);
  ss.enableEncoderInterrupt();

  //init console array
  strcpy(con[1],"Analog In: ");

}

void loop(){
  if (! ss.digitalRead(SS_SWITCH)) {
    Serial.println("Button pressed!");
    encoder_position = 0;
  }
  int32_t new_position = ss.getEncoderPosition();
  // did we move arounde?
  if (encoder_position != new_position) {
    Serial.println(new_position);         // display new position
    char s[5] = {'\0'};
    strcpy(con[1],"Analog In: ");
    itoa(encoder_position,s,10);
    strcat(con[1], s);
    console(con); // cal console function to update screen with con array
    sspixel.setPixelColor(0, Wheel(new_position & 0xFF));
    sspixel.show();
    encoder_position = new_position;      // and save for next round
  }


  // Retrieve a point
  TSPoint p = ts.getPoint();

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  if (p.z < MINPRESSURE || p.z > MAXPRESSURE) {
     return;
  }

  if (! ss.digitalRead(SS_SWITCH)) {
    Serial.println("Button pressed!");
  }


  // Scale from ~0->1000 to tft.width using the calibration #'s
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());


  Serial.print("X = "); Serial.println(p.x);
  Serial.print("Y = "); Serial.println(p.y);
//EDIT
  tft.setCursor(40,40);
  tft.setTextColor(HX8357_WHITE);
  tft.setTextSize(2);
  if ((p.y < 140 && p.y > 20) && (p.x < 190 && p.x > 20)) {
    tft.fillRect(20, 20, 220, 100, HX8357_BLACK);
    tft.drawRect(20,20,220,100,HX8357_WHITE);


    if(!on){
        //tft.fillRect(100, 100, 50, 50, HX8357_GREEN);
        strcpy(con[0],"GREEN");
        console(con);
        digitalWrite(3,HIGH);
        digitalWrite(4,LOW);
        on = true;
        delay(150);

    }else{
      //tft.fillRect(100, 100, 50, 50, HX8357_YELLOW);
      strcpy(con[0],"RED");
      console(con);
      digitalWrite(3,LOW);
      digitalWrite(4,HIGH);
      on = false;
      delay(150);
    }

    //Serial.println("PRESSED");
  }

if ((p.y < 142 && p.y > 20) && (p.x < 370 && p.x > 230)) {
  
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
  strcpy(con[0],"OFF");
  console(con);
  }


 // tft.fillCircle(slide,280,25,HX8357_BLACK);
  if(p.x > 400 && p.x < 455 && p.y > 16 && p.y < 306){
      if (firstRun){
        firstRun = false;
        tft.fillCircle(slide,280,25,HX8357_CYAN);

      }
      else{
        tft.fillCircle(ymap+20,280,27,HX8357_CYAN);
      }
      slide = 306 - p.y;
      ymap = map(slide,20,280,20,430);

      //tft.println(slide);
      tft.fillRect(20,265,450,30,HX8357_RED);
      tft.fillCircle(ymap+20,280,25,HX8357_BLACK);
      char s[5] = {'\0'};
      strcpy(con[2],"Slider: ");
      itoa(ymap,s,10);
      strcat(con[2], s);
      console(con);
  }

  delay(10);
}
// Takes position of the rotary encoder and updates the color of the LED accordingly 
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return sspixel.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return sspixel.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return sspixel.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void console(char c[3][16]){
  tft.fillRect(20, 20, 220, 100, HX8357_BLACK);//will remve to screen check
  tft.drawRect(20,20,220,100,HX8357_WHITE);//will remoce to screen check
  tft.setCursor(40,40);
  tft.setTextColor(HX8357_WHITE);
  tft.setTextSize(2);
  tft.println(c[0]);
  tft.setCursor(40,60);
  tft.println(c[1]);
  tft.setCursor(40,80);
  tft.println(c[2]);
}
