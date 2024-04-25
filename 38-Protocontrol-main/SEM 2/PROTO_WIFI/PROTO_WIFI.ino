#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>
// #include <WiFi.h>
#include <ArduinoHttpClient.h>
#include "I2CKeyPad.h"
#include "elements.h"
#include "PCF8574.h"
#include <seesaw_neopixel.h>
#include "SPIFFS.h"
#include <Adafruit_ADS1X15.h>

//for AP and wifi stuff
  // #include <AsyncTCP.h>
  // #include <ESP8266WiFi.h>
  // #include <ESPAsyncTCP.h>
  #include <WiFiClient.h>
  #include <WiFiAP.h>
  #include <ESPAsyncWebServer.h>
  #include "FS.h"
  //#include "SPIFFS.h"
// ======================

//include

#define FORMAT_SPIFFS_IF_FAILED true
#define TIMEOUT 300

// #define ssid "ncsu" //ncsu
// #define pass ""

#define TFT_CS 10
#define TFT_DC 9

#define SEESAW_ADDR          0x38
#define SS_SWITCH            24
#define SS                    6
#define LED                   13

#define cardSelect            4
PCF8574 pcf8574(0x3A);
int encoder = false;
Adafruit_FT6206 ts = Adafruit_FT6206();
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

WiFiClient client;
HttpClient httpClient = HttpClient(client, "protocontrol.pythonanywhere.com", 80);


Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */

// for AP and wifi & SPIFFS
  String inputParam1;
  String inputParam2;
  String inputssid = "none";
  String inputpassword = "none"; 

const char* wl_status_to_string(wl_status_t status) {
  switch (status) {
    case WL_NO_SHIELD: return "WL_NO_SHIELD";
    case WL_IDLE_STATUS: return "WL_IDLE_STATUS";
    case WL_NO_SSID_AVAIL: return "WL_NO_SSID_AVAIL";
    case WL_SCAN_COMPLETED: return "WL_SCAN_COMPLETED";
    case WL_CONNECTED: return "WL_CONNECTED";
    case WL_CONNECT_FAILED: return "WL_CONNECT_FAILED";
    case WL_CONNECTION_LOST: return "WL_CONNECTION_LOST";
    case WL_DISCONNECTED: return "WL_DISCONNECTED";
    default: return "INVALID STATUS RETURN CHECK DOCUMENTATION";
  }
}

  // Where the webpage values are stored.
const char* PARAM_ssid = "ssid";
const char* PARAM_password = "password";

  const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>WiFi Setup</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <script>
    function eraseMessage() {
      alert("Erased credentials from ESP SPIFFS");
      setTimeout(function(){ document.location.reload(false); }, 500);   
    }
  </script>
  <script>
    function submitMessage() {
      alert("Saved credentials to ESP SPIFFS");
      setTimeout(function(){ document.location.reload(false); }, 500);   
    }
  </script></head><body>
  <form action="/get" target="hidden-form">
   SSID (last saved: %ssid%):<br><input type="text" name="ssid"><br>
  Password:<br><input type="password" name="password"><br><br>
    <input type="submit" value="Submit" onclick="submitMessage()">
  </form>
  <iframe style="display:none" name="hidden-form"></iframe>
</body></html>)rawliteral";

  // AP SSID & Password
const char *ssid = "ProtoAP";
const char *password = "password";

bool connected = false;

/// END AP and SPIFFS and HTML defines////////


//const int SW_pin = 8; // digital pin connected to switch output
const int Y_pin = 0; // analog pin connected to Y output (20 to 26790)
const int X_pin = 1; // analog pin connected to X output (20 to 26790)

bool joystick = false;
const int pButton = 5; 
const int uButton = 6;
bool updating = false;
bool updating_wifi = false;
const int MAX = 255;
const int MIN = 0; 
int n;
int running_count_new;
int running_count_old;
int raw_val_old = 0;
int raw_val_new; 

int16_t adc0, adc1;
int16_t adc0_last = 0; 
int16_t adc1_last = 0;
float volts0, volts1;


int pushIndex = 0;
PushButton* buttons[12];
int incIndex = 0;
increment* increments[12];
int togIndex = 0;
Toggle* toggles[12];
int conIndex = 0;
Slider* sliders[12];
int sldIndex = 0;

String communication;

String commands[12][10];
Console con;
Slider slide = Slider(5,5,false,0,255);

AsyncWebServer server(80);



//i2c components
//keypad
const uint8_t KEYPAD_ADDRESS = 0x39;

I2CKeyPad keyPad(KEYPAD_ADDRESS);
int keypads = false;
uint32_t start, stop;
uint32_t lastKeyPressed = 0;

const char keys[17] = "0147*N2580N369#";


//AP and SPIFFS Functions

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\r\n", path);
    if(fs.remove(path)){
        Serial.println("- file deleted");
    } else {
        Serial.println("- delete failed");
    }
}

String processor(const String& var){
  //Serial.println(var);
  if(var == "ssid"){
    return readFile(SPIFFS, "/ssid.txt");
  }
  else if(var == "password"){
    return readFile(SPIFFS, "/password.txt");
  }
  return String();
}

String readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if(!file || file.isDirectory()){
    Serial.println("- empty file or failed to open file");
 
    return "no";
  }
  Serial.println("- read from file:");
  String fileContent;
  while(file.available()){
    fileContent+=String((char)file.read());
  }
  file.close();
  Serial.println(fileContent);
  return fileContent;
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}

void startAP(const char *ssid, const char *password) {

  Serial.println("Configuring access point...");
  WiFi.mode(WIFI_AP_STA);

  if (!WiFi.softAP(ssid, password)) {
    log_e("Soft AP creation failed.");
    while(1);
  }

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  SetupScreen();
  

  Serial.println("Access Point started");

}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void startServer(){

  Serial.println("----------Start Server----------");
    // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send_P(200, "text/html", html, dataProcessor);
  // });
  // Send a GET request to <ESP_IP>/get?ssid=<inputMessage>
  /* the code within the server.on() block execute after the 'submit' button is pressed on the webpage */  //TODO: confirm thats true, it seemed so to me at least
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {

    // String inputssid, inputpassword;
    // String inputParam1, inputParam2;

    // GET ssid value on <ESP_IP>/get?ssid=<inputMessage>
    if (request->hasParam(PARAM_ssid) and request->hasParam(PARAM_password)) { //This if statement is true when the 'ssid' and 'password' field are NOT empty on the webpage when 'submit' is pressed
      inputssid = request->getParam(PARAM_ssid)->value();
      inputParam1 = PARAM_ssid;
      inputpassword = request->getParam(PARAM_password)->value();
      inputParam2 = PARAM_password;

      writeFile(SPIFFS, "/ssid.txt", inputssid.c_str());
      writeFile(SPIFFS, "/password.txt", inputpassword.c_str());

      stationMode();

    }
    // GET input2 value on <ESP_IP>/get?password=<inputMessage>
  /*  else if (request->hasParam(PARAM_password)) {
      inputMessage = request->getParam(PARAM_password)->value();
      inputParam = PARAM_password;
    } */
    else { // One or both 'ssid' or 'password' were left empty on the client's webpage
      inputssid = "none";
      inputpassword = "none";
      /* if the else condition runs and you think it shouldn't, make sure you didn't change 'PARAM_ssid' or 'PARAM_password' (this will cause the html text entries to not be pointed to by your Arduino code, meaning it'll return as empty/null even if text was entered on the webpage)
         ^ if you did change 'PARAM_ssid' or PARAM_password's value, make sure you also changed it here in the html code too:
              ssid: <input type="text" name="THIS_NEEDS_TO_BE_THE_SAME_AS_THE_VALUE_OF_PARAM_ssid"><br>
          The same is true for 'PARAM_password' too.
      */
    }
    Serial.println(inputssid);
    Serial.println(inputpassword);
    request->send(200, "text/text", inputssid);
    // request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
    //                                  + inputParam1 + ") with value: " + inputssid +
    //                                  " field ("
    //                                  + inputParam2 + ") with value: " + inputpassword +
    //                                  "<br><a href=\"/\">Return to Home Page</a>");                             
  });
  server.onNotFound(notFound);
  server.begin();
  Serial.println("Server Set Up");
}

void initWiFi() {

  inputssid = readFile(SPIFFS, "/ssid.txt");
  inputpassword = readFile(SPIFFS, "/password.txt");
  //WiFi.mode(WIFI_AP_STA);
  WiFi.disconnect();
  delay(100);
  WiFi.begin(inputssid, inputpassword);
  Serial.println();
  Serial.println();
  Serial.print("ESP Board MAC Address: ");
  Serial.println(WiFi.macAddress());
  Serial.print("Connecting to WiFi ");
  Serial.print(inputssid);
  Serial.print(" ..");
  int i = 0;
  for(; (WiFi.status() != WL_CONNECTED && WiFi.status() != WL_CONNECT_FAILED) && i<TIMEOUT; i++) {
    Serial.print('.');
    //checkResetButton(); //uncomment when button is enabled
    delay(1000);
  }
  Serial.print("\n");
  if (i == TIMEOUT){
    Serial.print("Connection Timed out after ");
    Serial.print(TIMEOUT);
    Serial.println(" seconds.");
  }
  switch (WiFi.status()) {
    case WL_CONNECT_FAILED:
    // statements
      Serial.print("Connection Failed! "); 
      setUpUserWiFi();
    break;
    case WL_CONNECTED:
      Serial.print("Connected IP Address: ");
      Serial.println(WiFi.localIP());
      Serial.print("RSSI: ");
      Serial.println(WiFi.RSSI());

      updating_wifi = false;
      connected = true;
      updateProfile(tft);
    break;
    default:
    // statements
      Serial.print("Wifi Status: ");
      Serial.println(wl_status_to_string(WiFi.status()));
    break;
  }
}

void stationMode(){

  Serial.println("Station Mode");
// Set up WiFi
  initWiFi();

  // while(!connected){
  //   // checkResetButton();  //uncommment when you have a button
  // }
  delay(100);
  
// Disconnect Access Point
  WiFi.softAPdisconnect(false);
  delay(100);
  Serial.println("AP Disconnected");
  //startDataServer();
}

// UNCOMMENT BELOW WHEN YOU HAVE A BUTTON 

// void checkResetButton(){
//   if (button1.pressed){
//     deleteFile(SPIFFS, "/ssid.txt");
//     deleteFile(SPIFFS, "/password.txt");
//     inputssid = "none";
//     inputpassword = "none";
//     button1.pressed = false;
//     connected = false;
//     setUpUserWiFi();
//   }
// }

void setUpUserWiFi(){
  // Create Access Point
  startAP(ssid, password);
  
  // Start a Server
  startServer();
  // // Wait for ssid & password input 
  // while(inputssid == "none" && inputpassword == "none");

  // Connect to router
  //stationMode();
}

void SetupScreen(){

  tft.fillScreen(ILI9341_BLACK);

  tft.setCursor(10, 10);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(3);
  tft.println("PROTOCONTROL");

  tft.setCursor(5, 50);
  tft.setTextColor(ILI9341_CYAN);  tft.setTextSize(2);
  tft.println("WiFi Setup");

  tft.setCursor(5, 115);
  tft.setTextColor(ILI9341_BLUE);  tft.setTextSize(2);
  tft.println("Access Point IP:");
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(3);
  tft.println(WiFi.softAPIP());

}

void WriteProfiles(int ID, const char* param){

  switch (ID){
    case 1:
      writeFile(SPIFFS, "/P1.txt", param);
    break;

    case 2:
      writeFile(SPIFFS, "/P2.txt", param);
    break;
      
    case 3:
      writeFile(SPIFFS, "/P3.txt", param);
    break;

    case 4:
      writeFile(SPIFFS, "/P4.txt", param);
    break;

    case 5:
      writeFile(SPIFFS, "/P5.txt", param);
    break;
  }
}

String ReadProfile(int ID){

String x;

  switch (ID){
    case 1:
      x = readFile(SPIFFS, "/P1.txt");
      return x;
    break;

    case 2:
      x = readFile(SPIFFS, "/P2.txt");
      return x;
    break;
      
    case 3:
      x = readFile(SPIFFS, "/P3.txt");
      return x;
    break;

    case 4:
      x = readFile(SPIFFS, "/P4.txt");
      return x;
    break;

    case 5:
      x = readFile(SPIFFS, "/P5.txt");
      return x;
    break;
  }

}

void setup(void) {
  Serial.begin(115200);
  Serial1.begin(115200);
  SPI.begin();
  pinMode(LED, OUTPUT);
  tft.begin();
  ts.begin(40);
  tft.setRotation(1);
  

  Serial.println("+++++++++START+++++++++");
  
  ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
 

  pinMode(uButton, INPUT_PULLUP);
  pinMode(pButton, INPUT_PULLUP);
  // WiFi.begin(ssid, pass);
  // while (WiFi.status() != WL_CONNECTED)
  //   delay(500);

  // Serial.println("Connected to WiFi");
  // printWifiStatus();

  //SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED);

  if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  else{
    Serial.println("SPIFFS Mounted");
  }

  String file = readFile(SPIFFS, "/ssid.txt");

  if (file == "no"){
      setUpUserWiFi();

      Serial.println("Start AP & WiFi Setup");
      
  }
  else{

    initWiFi();

    Serial.println("Connect to WiFi from saved parameters");
  }

  // String x = readFile(SPIFFS, "/ssid.txt");
  // if(x!=""){
  //   inputssid = readFile(SPIFFS, "/ssid.txt");
  //   inputpassword = readFile(SPIFFS, "/password.txt");
  //   WiFi.begin(inputssid,inputpassword);
  // }
 
   // setUpUserWiFi();
  
  
  //sendGetRequest();
  //updateProfile(tft);
  
  

  //con.drawConsole(tft);
  //slide.drawSlider(tft);
  //con.displayMessage("Light Level: 21",tft);
 // Slider s1 = Slider(110,5,true,0,100);
 // s1.drawSlider(tft);
}

void loop() {

   if(!digitalRead(uButton) && !digitalRead(pButton) && !updating_wifi){
    SetupScreen();
    updating_wifi = true;
    Serial.println("PRESSED WIFI");
    deleteFile(SPIFFS, "/ssid.txt");
    deleteFile(SPIFFS, "/password.txt");
    inputssid = "none";
    inputpassword = "none";
    connected = false;
    setUpUserWiFi();
  }

  // if still setting up wifi dont change screen //

  if(!digitalRead(uButton) && digitalRead(pButton) && !updating && !updating_wifi){
    updating = true;
    Serial.println("PRESSED");
    updateProfile(tft);
  }
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
      buttons[i]->handleTouch(p.x,p.y,tft);
    }
    for(int i = 0;i < incIndex;i++){
      increments[i]->handleTouch(p.x,p.y,tft);
    }
    for(int i = 0;i < togIndex;i++){
      toggles[i]->handleTouch(p.x,p.y,tft);
    }
    for(int i = 0;i < sldIndex;i++){
      sliders[i]->handleTouch(p.x,p.y,tft);
    }

    
  }
  if(keypads){
      uint8_t index = keyPad.getKey();
      if (index < 16){
        lastKeyPressed = index;
        Serial1.println("M"+(String)keys[index]);
        Serial.println("M"+(String)keys[index]);
        delay(100);
        //Serial.print("\n");
      }
  }
  if(encoder){
    raw_val_new = pcf8574.readEncoderValue(P1, P2); 
    if (raw_val_old != raw_val_new) {
      if (raw_val_new == 1) {
        running_count_new++;
        //Serial.println(running_count_new);
      }
      else if (raw_val_new == -1) {
        running_count_new--;
        //Serial.println(running_count_new);
      }
      Serial1.println("N"+(String)running_count_new);
      Serial.println("N"+(String)running_count_new);
    }
    raw_val_old = raw_val_new;
  }
  if(joystick){
    adc0 = ads.readADC_SingleEnded(X_pin);
    adc1 = ads.readADC_SingleEnded(Y_pin);

    volts0 = ads.computeVolts(adc0);
    volts1 = ads.computeVolts(adc1);

    if ((adc0 >= 14000) || (adc0 <= 12000)) {
      //O and P for x and y
      //map(value, fromLow, fromHigh, toLow, toHigh)
      adc0 = map(adc0,18,26271,0,200);
      Serial1.println("O"+(String)adc0);
      Serial.println("O"+(String)adc0);
      adc1 = map(adc1,18,26271,0,200);
      Serial1.println("P"+(String)adc1);
      Serial.println("P"+(String)adc1);

      // Serial.print("X-axis: ");
      // Serial.print(adc0);
      // Serial.print(" | ");
      // Serial.print("Y-axis: ");
      // Serial.print(adc1);
      // Serial.println(" | ");
    }
  }
  if(Serial1.available()){
    communication = Serial1.readStringUntil('\n');
    Serial.println(communication);
    if(communication[0] == '+'){
      communication.remove(0,1);
      if(conIndex > 0){
        con.displayMessage(communication.c_str(),tft);
      }
      //con.displayMessage(communication.c_str(),tft);
    }
    
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
  Serial.println(WiFi.macAddress());
}

void updateProfile(Adafruit_ILI9341 &tft) {
  Wire.begin();
  Wire.setClock(400000);
  tft.fillScreen(ILI9341_LIGHTGREY);
  keypads = true;
  if (keyPad.begin() == false)
  {
    Serial.println("\nERROR: cannot communicate to keypad.\nPlease reboot.\n");
    Serial.println(KEYPAD_ADDRESS);
    //while(1);
    keypads = false;
  }
  

  keyPad.setKeyPadMode(I2C_KEYPAD_5x3);

  pcf8574.pinMode(P2, INPUT);
  pcf8574.pinMode(P1, INPUT);
  Serial.println("SetupStarted");

  if (pcf8574.begin()) {
    Serial.println("Rotary Encoder Connected");
    encoder = true;
  }else{
    Serial.println("Rotary Encoder Not Connected");
    
  }

  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
  }else{
    joystick = true;
  }

//delete memory
  for(int i = 0;i < pushIndex;i++){
    delete buttons[i];
    buttons[i] = NULL;
  }
  for(int i = 0;i < incIndex;i++){
    delete increments[i];
    increments[i] = NULL;
  }
  for(int i = 0;i < togIndex;i++){
    delete increments[i];
    increments[i] = NULL;
  }
  for(int i = 0;i< sldIndex;i++){
    delete sliders[i];
    sliders[i] = NULL;
  }
  conIndex = 0;
  pushIndex = 0;
  incIndex = 0;
  togIndex = 0;
  sldIndex = 0;


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




  for (int i = 0; i < row; i++) {
    
    if (commands[i][0] == "PushButton") {
        buttons[pushIndex] = new PushButton(commands[i][1].toInt(), commands[i][2].toInt(), strtoul(commands[i][3].c_str(), NULL, 16), strtoul(commands[i][4].c_str(), NULL, 16), commands[i][5].c_str());
        pushIndex++;
    }else if(commands[i][0] == "Increment"){
        increments[incIndex] = new increment(commands[i][1].toInt(), commands[i][2].toInt(),strtoul(commands[i][3].c_str(), NULL, 16));
        incIndex++;
    }else if(commands[i][0] == "ToggleButton"){
        toggles[togIndex] = new Toggle(commands[i][1].toInt(), commands[i][2].toInt(),strtoul(commands[i][3].c_str(), NULL, 16));
        togIndex++;
    }else if(commands[i][0] == "Terminal"){
        con = Console(commands[i][1].toInt(), commands[i][2].toInt());
        conIndex++;
    }else if(commands[i][0] == "Slider"){
        sliders[sldIndex] = new Slider(commands[i][1].toInt(), commands[i][2].toInt(),commands[i][3].toInt(),commands[i][4].toInt(),commands[i][5].toInt());
        sldIndex++;
    }

  }
  for(int i = 0;i < pushIndex;i++){
    buttons[i]->drawButton(tft);
  }
  for(int i = 0;i < incIndex;i++){
    increments[i]->drawIncrement(tft);
  }
  for(int i = 0;i < togIndex;i++){
    toggles[i]->drawToggle(tft);
  }
  for(int i = 0;i< sldIndex;i++){
    sliders[i]->drawSlider(tft);
  }for(int i = 0;i<conIndex;i++){
    con.drawConsole(tft);
    
  }
  updating = false;
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