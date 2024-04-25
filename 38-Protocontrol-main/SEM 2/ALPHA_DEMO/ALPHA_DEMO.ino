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
#include "I2CKeyPad.h"

const uint8_t KEYPAD_ADDRESS = 0x20;

I2CKeyPad keyPad(KEYPAD_ADDRESS);

uint32_t start, stop;
uint32_t lastKeyPressed = 0;

const char keys[17] = "0147*N2580N369#";

#define ssid "ncsu" //ncsu
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



int pushIndex = 0;
PushButton buttons[12];
int incIndex = 0;
increment increments[12];
int togIndex = 0;
Toggle toggles[12];
int conIndex = 0;
String communication;

String commands[12][10];
Console con = Console(110,120);
Slider slide = Slider(5,5,false,0,100);

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
  Wire.begin();
  Wire.setClock(400000);
  if (keyPad.begin() == false)
  {
    Serial.println("\nERROR: cannot communicate to keypad.\nPlease reboot.\n");
    
  }

  keyPad.setKeyPadMode(I2C_KEYPAD_5x3);
  //sendGetRequest();
  updateProfile(tft);
  
  for(int i = 0;i < pushIndex;i++){
    buttons[i].drawButton(tft);
  }
  for(int i = 0;i < incIndex;i++){
    increments[i].drawIncrement(tft);
  }
  for(int i = 0;i < togIndex;i++){
    toggles[i].drawToggle(tft);
  }
  //con.drawConsole(tft);
  //slide.drawSlider(tft);
  //con.displayMessage("Light Level: 21",tft);
 // Slider s1 = Slider(110,5,true,0,100);
 // s1.drawSlider(tft);
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
    for(int i = 0;i < pushIndex;i++){
      buttons[i].handleTouch(p.x,p.y,tft);
    }
    for(int i = 0;i < incIndex;i++){
      increments[i].handleTouch(p.x,p.y,tft);
    }
    for(int i = 0;i < togIndex;i++){
      toggles[i].handleTouch(p.x,p.y,tft);
    }
    
  }
  if(Serial1.available()){
    communication = Serial1.readStringUntil('\n');
    Serial.println(communication);
    con.displayMessage(communication.c_str(),tft);
  }
  //i2c keypad
  uint8_t index = keyPad.getKey();
  if (index < 16){
    lastKeyPressed = index;
    Serial1.println(keys[index]);
    delay(100);
    //Serial.print("\n");
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
  String input = httpClient.responseBody();

  Serial.println("Response status code:");
  Serial.println(statusCode);
  Serial.println("Response body:");
  Serial.println(input);

  int row = 0;
  int col = 0;
  int startIndex = 0;

  
  for (int i = 0; i < input.length(); i++) {
    char currentChar = input.charAt(i);
    if (currentChar == ',' || currentChar == '|') {
      if (i > startIndex) {
        commands[row][col++] = input.substring(startIndex, i);
      } 
      startIndex = i + 1;
    }
    if (currentChar == '|') {
      row++;
      col = 0;
      startIndex = i + 1;
    }
  }

  //  for (int i = 0; i < 3; i++) {
  //   Serial.print("row");
  //   Serial.print(i);
  //   Serial.print(": ");
  //   for (int j = 0; j < 6; j++) {
  //     Serial.print("\"");
  //     Serial.print(commands[i][j]);
  //     Serial.print("\"");
  //     if (j < 5) {
  //       Serial.print(",");
  //     }
  //   }
  //   Serial.println();
  // }
  


  for (int i = 0; i < row; i++) {
    
    if (commands[i][0] == "PushButton") {
        buttons[pushIndex] = PushButton(commands[i][1].toInt(), commands[i][2].toInt(), strtoul(commands[i][3].c_str(), NULL, 16), strtoul(commands[i][4].c_str(), NULL, 16), commands[i][5].c_str());
        pushIndex++;
    }else if(commands[i][0] == "Increment"){
        increments[i] = increment(commands[i][1].toInt(), commands[i][2].toInt(),strtoul(commands[i][3].c_str(), NULL, 16));
        incIndex++;
    }else if(commands[i][0] == "ToggleButton"){
        toggles[i] = Toggle(commands[i][1].toInt(), commands[i][2].toInt(),strtoul(commands[i][3].c_str(), NULL, 16));
        togIndex++;
    }else if(commands[i][0] == "Console"){
        // consoles[12] = Console(commands[i][1].toInt(), commands[i][2].toInt());
        // conIndex++;
    }

  }
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