
//protocontrol 1 sketch, for cdr
//parker sexton, 1/16/24


#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>
#include "elements.h"
#include <SD.h>
#include "Adafruit_seesaw.h"
#include <seesaw_neopixel.h>
#include <WiFi.h>
#include <ArduinoHttpClient.h>

#define ssid "ncsu"
#define pass ""

char server[] = "10.152.43.98";
char path[]   = "/random-coords";

#define SEESAW_ADDR          0x38

//#include <vector>
#include <PCF8574.h>

#include "Arduino.h"
#include "PCF8574.h"

PCF8574 pcf8574(0x20);

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

#define background ILI9341_LIGHTGREY
#define BOX 218
#define cardSelect 4

bool mode; // for CDR tech demo
int encoder = 123;
WiFiClient client;

HttpClient httpClient = HttpClient(client, server, 5000);

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
// PushButton but = PushButton(5,10,ILI9341_GREEN,ILI9341_WHITE,"PUSH");
// PushButton but2 = PushButton(110,10,ILI9341_BLUE,ILI9341_WHITE,"PUSH");
// PushButton but3 = PushButton(215,10,ILI9341_BLUE,ILI9341_BLACK,"PUSH");
// PushButton but4 = PushButton(5,65,ILI9341_BLUE,ILI9341_BLACK,"PUSH");
// PushButton but5 = PushButton(5,120,ILI9341_BLUE,ILI9341_BLACK,"PUSH");
// PushButton but6 = PushButton(5,180,ILI9341_BLUE,ILI9341_BLACK,"PUSH");
//Toggle tog1 = Toggle(210,180,ILI9341_WHITE);
// Toggle t2 = Toggle(20,50,ILI9341_BLACK);
//increment in1 = increment(20,80,ILI9341_WHITE);
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


void console(int x,int y, int w,int h){
  tft.fillRect(x,y,w,h,ILI9341_BLACK);
  tft.drawRect(x,y,w,h,ILI9341_WHITE);
}
void setup(void)
{

  //Wire.begin();
  Serial.begin(115200);
  Serial1.begin(115200);
  delay(20);
  SPI.begin();
  digitalWrite(SS,HIGH);     
  pinMode(LED, OUTPUT); // input button 2
  //i2c init
  pcf8574.pinMode(P0, INPUT);
  pcf8574.pinMode(P1, INPUT);
  pcf8574.pinMode(P2,INPUT_PULLUP);
  if(pcf8574.begin()){
    Serial.println("CHIP DETECTED");
  }

  
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
  Serial.println(WiFi.macAddress());
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  printWifiStatus();

  delay(100);
  if (client.connect(server, 5000)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.print("GET "); client.print(path); client.println(" HTTP/1.1");
    client.print("Host: "); client.println(server);
    client.println("Connection: close");
    client.println();
    
  }
  //console(20,20,130,75);
  //SD card init
  // if (!SD.begin(cardSelect)) {
  //   Serial.println("Card init. failed!");
  // }
  //init ss rotary encoder over i2c
  // if (!ss.begin(0x38)) {
  //   Serial.println("Couldn't find seesaw on default address");
  //   //while(1) delay(10);
  // }
  //i2c read
  //encoder reads value
  sendGetRequest();
  updateProfile(tft);
  if(pcf8574.digitalRead(P1) == LOW){
    encoder++;
    delay(10);
  }
  tft.setCursor(120, 150);
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_BLACK);
  tft.println(encoder);

//drawing default components

  // but.drawButton(tft);
  // but2.drawButton(tft);
  // but3.drawButton(tft);
  // but4.drawButton(tft);
  // but5.drawButton(tft);
  // but6.drawButton(tft);
  //tog1.drawToggle(tft);
  //in1.drawIncrement(tft);
  Serial1.println("test");
}
  

  




void loop()
{
  
  //testing protocal on Serial, not Serial1 to update display with loaded digital components
  

  
//scanner functiin

  //display(con);

  
  // See if there's any  touch data for us
  if (ts.touched()){
    //Serial.println("touched");
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

    //sendGetRequest();
    
    // but.handleTouch(p.x,p.y,tft);
    // but2.handleTouch(p.x,p.y,tft);
    // but3.handleTouch(p.x,p.y,tft);
    // but4.handleTouch(p.x,p.y,tft);
    // but5.handleTouch(p.x,p.y,tft);
    // but6.handleTouch(p.x,p.y,tft);
    //tog1.handleTouch(p.x,p.y,tft);
    //in1.handleTouch(p.x,p.y,tft);
    // if(tog1.state != lastState){
    //   if(tog1.state){
    //     Serial1.println("1");
    //     Serial.println("on");
        
    //   }else{
    //     Serial1.println("0");
    //     Serial.println("off");
    //     }
    //   lastState = tog1.state;
    // }
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



void drawComponents(Adafruit_ILI9341 &tft){
  File logfile;
  logfile = SD.open(filename);
        if(!logfile) {
          Serial.print("Couldn't open ");
          Serial.println(filename);
        } else {
          Serial.print("reading from ");
          Serial.println(filename);
        }
        int inc = 0;
        int line = 0;
        while (logfile.available()) {
            
            String parse = logfile.readStringUntil(',');
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

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void updateProfile(Adafruit_ILI9341 &tft){
        httpClient.get(path);
        int statusCode = httpClient.responseStatusCode();
        String response = httpClient.responseBody();

        Serial.println("Response status code:");
        Serial.println(statusCode);
        Serial.println("Response body:");
        Serial.println(response);
        String commands[10];
        int index = 0;
        int fromIndex = 0;
        int toIndex;
        while ((toIndex = response.indexOf(',', fromIndex)) != -1) {
          // Extract substring between fromIndex and toIndex
          commands[index++] = response.substring(fromIndex, toIndex);
          // Move fromIndex to the next character after the comma
          fromIndex = toIndex + 1;
        }
     
        b1 = PushButton(commands[1].toInt(),commands[2].toInt(),strtoul(commands[3].c_str(),NULL,16),strtoul(commands[4].c_str(),NULL,16),commands[5].c_str());
        b1.drawButton(tft);
}

void sendGetRequest() {
    httpClient.get(path);
    int statusCode = httpClient.responseStatusCode();
    String response = httpClient.responseBody();

    Serial.println("Response status code:");
    Serial.println(statusCode);
    Serial.println("Response body:");
    Serial.println(response);

    
}
