#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>
#include <WiFi.h>
#include <ArduinoHttpClient.h>

#include "elements.h"
#include "PCF8574.h"
#include <seesaw_neopixel.h>

#define ssid "ncsu"
#define pass ""

#define TFT_CS 10
#define TFT_DC 9

#define SEESAW_ADDR          0x38
#define SS_SWITCH            24
#define SS                    6
#define LED                   13

#define cardSelect            4

Adafruit_FT6206 ts = Adafruit_FT6206();
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

WiFiClient client;
HttpClient httpClient = HttpClient(client, "protocontrol.pythonanywhere.com", 80);
componentFactory builder;
String recive;
std::vector<Component*> Component::profile;

 //

char filename[15] = "data.txt";

//test components
PushButton com = PushButton(110,10,65535,63488,"LED");

void setup(void) {
  Serial.begin(115200);
  Serial1.begin(115200);
  SPI.begin();
  pinMode(LED, OUTPUT);

 

  tft.begin();
  ts.begin(40);
  tft.setRotation(1);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
    delay(500);

  Serial.println("Connected to WiFi");
  printWifiStatus();

  //sendGetRequest();
  updateProfile(tft);
  for (Component* comp : Component::profile) {
    comp->draw(tft);
    
    // Add similar blocks for other derived classes (Increment, Slider)...
}

  //com.draw(tft);


}

void loop() {
  if (ts.touched()) {
    TS_Point p = ts.getPoint();
    p.x = map(p.x, 0, 240, 240, 0);
    p.y = map(p.y, 0, 320, 320, 0);
    int y = tft.height() - p.x;
    int x = p.y;
    int temp = p.y;
    p.y = p.x;
    p.x = temp;
    //com.handleTouch(p.x,p.y,tft);
    //call handletouch functions
    // if(com.pressed){
    //   Serial1.println("D:1");
    // }else{
    //   Serial1.println("D:0");
    // }
  }
  if(Serial1.available()){
    recive = Serial1.readStringUntil('\n');
    Serial.println(recive);
    tft.setCursor(110, 50);
    tft.println(recive);
  }
  
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void updateProfile(Adafruit_ILI9341 &tft) {
  httpClient.get("/latest-entry");
  int statusCode = httpClient.responseStatusCode();
  String response = httpClient.responseBody();

  Serial.println("Response status code:");
  Serial.println(statusCode);
  Serial.println("Response body:");
  Serial.println(response);
  builder.build(response);
  
}


void sendGetRequest() {
  httpClient.get("/random-coords");
  int statusCode = httpClient.responseStatusCode();
  String response = httpClient.responseBody();

  Serial.println("Response status code:");
  Serial.println(statusCode);
  Serial.println("Response body:");
  Serial.println(response);
}




// void drawComponents(Adafruit_ILI9341 &tft){
//   File logfile;
//   logfile = SD.open(filename);
//         if(!logfile) {
//           Serial.print("Couldn't open ");
//           Serial.println(filename);
//         } else {
//           Serial.print("reading from ");
//           Serial.println(filename);
//         }
//         int inc = 0;
//         int line = 0;
//         while (logfile.available()) {
            
//             String parse = logfile.readStringUntil(',');
//             //Serial.write(parse);
//             Serial.println(parse);
//             inits[line][inc] = parse;
//             inc++;
//             if(parse == "|"){
//               line++;
//               inc = 0;
//               Serial.print(parse.c_str());
//               Serial.print("^");
//             }
            
            
//           }
      
//         Serial.println(line);
//         Serial.println(inits[1][0]);
//         Serial.println(inits[1][1]);
//         Serial.println(inits[1][2]);
//         Serial.println(inits[1][3]);
//         // Serial.print("<-");
//         for(int i = 0;i<=line;i++){
//               String cmp = "PushButton";
//               if(inits[i][0] == cmp){
//                 b1 = PushButton(inits[i][1].toInt(),inits[i][2].toInt(),strtoul(inits[i][3].c_str(),NULL,16),strtoul(inits[i][4].c_str(),NULL,16),inits[i][5].c_str());
//                 b1.drawButton(tft);
//               }
//               cmp = "Toggle";
//               if(inits[i][0] == cmp){
//                   t1 = Toggle(inits[i][1].toInt(),inits[i][2].toInt(),strtoul(inits[i][3].c_str(),NULL,16));
//                   t1.drawToggle(tft);
//               }
//               cmp = "increment";
//               if(inits[i][0] == cmp){
//                   i1 = increment(inits[i][1].toInt(),inits[i][2].toInt(),strtoul(inits[i][3].c_str(),NULL,16));
//                   i1.drawIncrement(tft);
//               }
//         }
// }
