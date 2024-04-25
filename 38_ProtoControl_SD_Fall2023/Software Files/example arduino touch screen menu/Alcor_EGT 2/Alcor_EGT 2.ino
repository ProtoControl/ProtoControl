  /*
 Name:    Waveshare_4_inch_Touchscreen.ino
 Created: 11-10-2021           
 Author:  Paul DeMint
*/
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_MAX31856.h>
#include <Adafruit_GFX.h>
#include <Waveshare4InchTftShield.h>
#include <ADS1X15.h>

ADS1115 ADS(0x49);

const float I_R_Divide = 3.3/680.70;
const float A_D_Scale = 0.256/32767;
float Probe_Voltage;
float L_Probe_Resistance;
float R_Probe_Resistance;

// Serial data variables ------------------------------------------------------
const byte kNumberOfChannelsFromExcel = 6; //Incoming Serial Data Array                                                       
// IMPORTANT: This must be equal to number of channels set in Data Streamer

const char kDelimiter = ',';    // Data Streamer expects a comma delimeter
const int kSerialInterval = 50;   // Interval between serial writes
unsigned long serialPreviousTime; // Timestamp to track serial interval

char* arr[kNumberOfChannelsFromExcel];



//Check_Touch (uint16_t L_x, uint16_t R_x, uint16_t U_y, uint16_t L_y)
//if(Check_Touch (15, 205, 180, 275))  Left Side  (Box=220 wide, 95 Tall
//if(Check_Touch (220, 430, 180, 275)) Right Side (Box=220 wide, 95 Tall

#define LeftBox_x_Begin       4   // Left Hand Side of Display to Check
#define LeftBox_x_End       234   // Right Hand Side of Display to Check

#define RightBox_x_Begin    244   // Right Hand Side of Display to Check
#define RightBox_x_End      474   // Right Hand Side of Display to Check

#define Mode_Box_y_Begin    165   // Top Side of Display to Check
#define Mode_Box_y_End      235   // Bottom Side of Display to Check

#define Temp_Box_y_Begin    245   // Top Side of Display to Check
#define Temp_Box_y_End      315   // Bottom Side of Display to Check

#define BoxWidth      230   // (LeftBox_x_End-LeftBox_x_Begin)
#define BoxHeight     70   // (Mode_Box_y_End-Mode_Box_y_Begin)

#define Off         0
#define OFF         0
#define On          1
#define ON          1


int Left_ThermocoupleTemperature;
int Left_TC_Temperature_Last;
int Right_ThermocoupleTemperature;
int Right_TC_Temperature_Last;
 

#define ClockIn_Divide_OFF    0;
#define ClockIn_Divide_1      1;
#define ClockIn_Divide_8      2;
#define ClockIn_Divide_64     3;
#define ClockIn_Divide_256    4;
#define ClockIn_Divide_1024   5;
#define ClockIn_T1_Fall       6;
#define ClockIn_T1_Rise       7;

// PortB Bit
#define On_Brd_LED        0x80

volatile uint8_t Timer_1_Divider;
volatile uint8_t Initial_Start;
bool mSec10_FF = 0;
bool mSec250_FF = 0;
bool mSec1000_FF = 0;
bool Refresh_Edit_Box_L_FF;
bool Refresh_Edit_Box_R_FF;
bool Refresh_Temp_Box_L_FF;
bool Refresh_Temp_Box_R_FF;
bool Left_Mode_Box_Touch_FF;
bool Right_Mode_Box_Touch_FF;
bool Left_Temp_Box_Touch_FF;
bool Right_Temp_Box_Touch_FF;
bool Screen_Touch_FF;
bool  L_EGT;
bool  L_CHT;
bool  L_UnGND;
bool  R_EGT;
bool  R_CHT;
bool  R_UnGND;

//bool  L_TC_K_Type;

volatile uint16_t cx,cy;

volatile uint8_t Left_TC_Type;
volatile uint8_t Right_TC_Type;
#define TC_Fault          0 
#define B_Type            1 
#define E_Type            2 
#define J_Type            3 
#define K_Type            4 
#define N_Type            5 
#define R_Type            6 
#define S_Type            7 
#define T_Type            8 


volatile uint8_t Operating_Mode = 0;
#define Start_Up        0   //
#define Wait_For_Start  1   //
#define Idle            2   //

volatile uint8_t Test_Left_Mode = 0;
volatile uint8_t Test_Right_Mode = 0;
#define Standby         0   //
#define Begin_Test      1   //
#define Temp_Reached    2   //
#define Test_Complete   3   //

// Use software SPI:                               CS, SDI, SDO, CLK
// Use software SPI:                               CS,MOSI,MISO, CLK
Adafruit_MAX31856 maxthermo_0 = Adafruit_MAX31856(A15, A11, A12, A13);    //using Software simulated  SPI port
//Adafruit_MAX31856 maxthermo_0 = Adafruit_MAX31856(A11, A15, A14, A13);    //using Software simulated  SPI port
//  Adafruit_MAX31856 maxthermo_0 = Adafruit_MAX31856(48);                //using   Hardware          SPI port

// Use software SPI:                               CS,MOSI,MISO, CLK
//Adafruit_MAX31856 maxthermo_1 = Adafruit_MAX31856(PL3, PG1, PL7, PL5);     //using Software simulated  SPI port
Adafruit_MAX31856 maxthermo_1 = Adafruit_MAX31856(A14, A11, A12, A13);     //using Software simulated  SPI port
//Adafruit_MAX31856 maxthermo_1 = Adafruit_MAX31856(49);                  //using    Hardware         SPI port


// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
Waveshare4InchTftShield Waveshield;
Adafruit_GFX &tft = Waveshield;
// ________________________________________________________________________________________



// ****************************************************************************************
uint16_t color565(uint8_t r, uint8_t g, uint8_t b)
{
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
}
// ________________________________________________________________________________________



// ****************************************************************************************
// initialize timer 1, interrupt and variable
void Timer1_init(void)
{
  cli();
  TCCR1A  = 0;
  TCCR1B |= ClockIn_Divide_64;  // set up timer with pre-scaler = / 64 = 4uSecs Clock 
  TCNT1 = -2500;                // Preload Timer Counter => 4uSecs * 2500 = 2mSecs = 0.002Secs
  TIMSK1 |= (1 << TOIE1);       // enable overflow interrupt
  sei();                        // enable global interrupts
  Timer_1_Divider = 0;          // initialize overflow counter variable
}
// ________________________________________________________________________________________


// ****************************************************************************************
/*
tft.drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color)
Effect:
Draw a square outline with round corner on the TFT screen
Parameters:
x0：x coordinate of the starting point
y0：y coordinate of the starting point
w：the length of the square
h：the width of the square
radius：the radius of the round corner
color：the color of the square
*/
unsigned long RoundRects(){
int     w, i, i2, blue = 10, step,
        cx = tft.width() / 2 - 1,   // 480/2=240, -1 = 239  // Start AT Display Center
        cy = tft.height() / 2 -8;  // 320/3=106, -5 = 101   //  tft.fillScreen(BLACK);
  w = max(tft.width(), tft.height());   // 480 
  step = (256 * 12) / w;                 // 256*6=1536, / 480 = 3
   for (i = 0; i < w; i += 12)
  {
    i2 = i / 2;
    blue += step;
    tft.drawRoundRect(cx - i2, cy - ((i/3)-6), i, i/8, i / 40, color565(0, 0, blue));
  }
  
}
// ________________________________________________________________________________________



// ****************************************************************************************
unsigned long FilledRoundRects(){
unsigned long start;
int     i, i2, blue, step,
        cx = tft.width() / 2 - 1,
        cy = tft.height() / 3 + 5;

  blue = 256;
  step = (256 * 6) / min(tft.width(), tft.height());
  for (i = min(tft.width(), tft.height()); i > 20; i -= 6)
  {
    i2 = i / 2;
    blue -= step;
    tft.fillRoundRect(cx - i2, cy - i2, i, i/6, i / 16, color565(0, 0, blue));
  }
}
// ________________________________________________________________________________________



// ****************************************************************************************
//  Timer 1 Interrupt Routine
// called whenever TCNT1 overflows
ISR(TIMER1_OVF_vect)
{
  TCNT1 = -2500;          // Preload Timer Counter => 4uSecs * 2500 = 5mSecs = 0.010Secs
  PORTA ^= PA0;           // PORTA5 bit = Toggle 
  mSec10_FF = 1; 
  if(Timer_1_Divider %25 == 0){ 
    mSec250_FF   ^= 1;    // If 0.25 Second......
  }
  if(Timer_1_Divider %50 == 0){
    PORTB ^= On_Brd_LED; // If 0.5 Second......Blink LED
  }
  if(++Timer_1_Divider == 100) {  //
    Timer_1_Divider = 0;    // reset counter
    mSec1000_FF = On;
  }
}
// ________________________________________________________________________________________



// ****************************************************************************************
void Analog_2_Digital_Init(void){
    //       GAIN
  // 0  =  ±6.144V  default
  // 1  =  ±4.096V
  // 2  =  ±2.048V
  // 4  =  ±1.024V
  // 8  =  ±0.512V
  // 16 =  ±0.256V
  ADS.begin();
  ADS.setGain(16);      // .256 volt
  ADS.setDataRate(7);  // fast
  ADS.setMode(0);      // continuous mode
  ADS.readADC(0);      // first read to trigger
}
// _______________END Analog_2_Digital_Init________________________________________________



// ****************************************************************************************
void Thermocouple_Init(void){
  maxthermo_0.begin();
  maxthermo_0.setThermocoupleType(MAX31856_TCTYPE_K); 
  switch (maxthermo_0.getThermocoupleType() ) {
    case MAX31856_TCTYPE_B: Left_TC_Type = B_Type; break;
    case MAX31856_TCTYPE_E: Left_TC_Type = E_Type; break;
    case MAX31856_TCTYPE_J: Left_TC_Type = J_Type; break;
    case MAX31856_TCTYPE_K: Left_TC_Type = K_Type; break;
    case MAX31856_TCTYPE_N: Left_TC_Type = N_Type; break;
    case MAX31856_TCTYPE_R: Left_TC_Type = R_Type; break;
    case MAX31856_TCTYPE_S: Left_TC_Type = S_Type; break;
    case MAX31856_TCTYPE_T: Left_TC_Type = T_Type;  break;
    case MAX31856_VMODE_G8: break;
    case MAX31856_VMODE_G32: break;
    default: Left_TC_Type = OFF; ; break;
  }
  maxthermo_0.setConversionMode(MAX31856_ONESHOT_NOWAIT);

  maxthermo_1.begin();
  maxthermo_1.setThermocoupleType(MAX31856_TCTYPE_K);
  switch (maxthermo_1.getThermocoupleType() ) {
    case MAX31856_TCTYPE_B: Right_TC_Type = B_Type; break;
    case MAX31856_TCTYPE_E: Right_TC_Type = E_Type; break;
    case MAX31856_TCTYPE_J: Right_TC_Type = J_Type; break;
    case MAX31856_TCTYPE_K: Right_TC_Type = K_Type; break;
    case MAX31856_TCTYPE_N: Right_TC_Type = N_Type; break;
    case MAX31856_TCTYPE_R: Right_TC_Type = R_Type; break;
    case MAX31856_TCTYPE_S: Right_TC_Type = S_Type; break;
    case MAX31856_TCTYPE_T: Right_TC_Type = T_Type;  break;
    case MAX31856_VMODE_G8: break;
    case MAX31856_VMODE_G32: break;
    default: Right_TC_Type = OFF; ; break;
  }
  maxthermo_1.setConversionMode(MAX31856_ONESHOT_NOWAIT);
}
// ________________________________________________________________________________________



// ***************************************************************************************
// Left_x, Right_x, Upper_y, Lower_y
void Check_Touch (void){
  TSPoint p = Waveshield.getPoint();    //  Get raw touchscreen values.
  Waveshield.normalizeTsPoint(p);       //  Remaps raw touchscreen values to screen co-ordinates.
  if( (p.x < 480) && (p.y > 0) ){
    if(Screen_Touch_FF == OFF){
      Screen_Touch_FF = ON;
      if((p.x > LeftBox_x_Begin) && (p.x < LeftBox_x_End)){       // Touch is on LEFT side
        if    ((p.y > Mode_Box_y_Begin) && (p.y < Mode_Box_y_End))    Left_Mode_Box_Touch_FF = 1;
        else if((p.y > Temp_Box_y_Begin) && (p.y < Temp_Box_y_End))   Left_Temp_Box_Touch_FF = 1;
      }
      else if((p.x > RightBox_x_Begin) && (p.x < RightBox_x_End)){// Touch is on RIGHT side
        if    ((p.y > Mode_Box_y_Begin) && (p.y < Mode_Box_y_End))    Right_Mode_Box_Touch_FF = 1;
        else if((p.y > Temp_Box_y_Begin) && (p.y < Temp_Box_y_End))   Right_Temp_Box_Touch_FF = 1;
      }
    }
  }
  else if(Screen_Touch_FF)    Screen_Touch_FF = OFF;
}
// _________Check_Touch___________________________________________________________________





// ***************************************************************************************
void Print_Justify(uint16_t value_){
  if(value_ <  1000) tft.print(" ");
  if(value_ <   100) tft.print(" ");
  if(value_ <    10) tft.print(" ");
  tft.print(value_);
  tft.print("F");
}
// _______Print_Justify___________________________________________________________________

// ***************************************************************************************
void Draw_Left_Temp_Box (int16_t Box_Color, uint16_t Text_Color){
  //  tft.fillRoundRect(Xstart, YStart, Xwidth, Yheight, corner Radius, color);
  tft.fillRoundRect(LeftBox_x_Begin, Temp_Box_y_Begin, BoxWidth, BoxHeight, 8, Box_Color);  //color565(0, 0, 255));
  tft.setTextColor(Text_Color);
  tft.setCursor(LeftBox_x_Begin+10,292);
  tft.setTextSize(2);
}
// _______Draw_Left_Temp_Box______________________________________________________________

// ***************************************************************************************
void Draw_Right_Temp_Box(uint16_t Box_Color, uint16_t Text_Color){
  tft.fillRoundRect(RightBox_x_Begin, Temp_Box_y_Begin, BoxWidth, BoxHeight, 8, Box_Color);  //color565(0, 0, 255));
  tft.setTextColor(Text_Color);
  tft.setCursor(RightBox_x_Begin+10,292);
  tft.setTextSize(2);
}
// _______END Draw_Right_Temp_Box_________________________________________________________

// ***************************************************************************************
void HighLight_Left_Temp_Box(uint16_t Box_Color){
  tft.drawRoundRect(LeftBox_x_Begin, Temp_Box_y_Begin, BoxWidth, BoxHeight, 8, Box_Color );    // Print Boarder (White)
}
// _______END HighLight_Left_Temp_Box____________________________________________________

// ***************************************************************************************
void HighLight_Right_Temp_Box(uint16_t Box_Color){
  tft.drawRoundRect(RightBox_x_Begin, Temp_Box_y_Begin, BoxWidth, BoxHeight, 8, Box_Color );    // Print Boarder (White)
}
// _______END HighLight_Right_Temp_Box____________________________________________________

// ***************************************************************************************
void Check_Left_Refresh(uint16_t Box_Color, uint16_t Text_Color){
  if(Refresh_Edit_Box_L_FF){
    Refresh_Edit_Box_L_FF =0;
    //  tft.fillRoundRect(Xstart, YStart, Xwidth, Yheight, corner Radius, color);
    tft.fillRoundRect(LeftBox_x_Begin, Mode_Box_y_Begin, BoxWidth, BoxHeight, 8, Box_Color);  //color565(0, 0, 255));
    tft.setTextColor(Text_Color);
    tft.setTextSize(4);
    tft.setCursor(LeftBox_x_Begin+10,Mode_Box_y_Begin+6);
    tft.print("  Press  ");
    tft.setTextSize(3);
    tft.setCursor(LeftBox_x_Begin+10,Mode_Box_y_Begin+41);
    if(Test_Left_Mode == Begin_Test)  tft.print("  to RETURN");
    else                tft.print("  to Start ");
  }
}
// _______ End Check_Left_Refresh_________________________________________________________



// ***************************************************************************************
void Check_Right_Refresh(uint16_t Box_Color, uint16_t Text_Color){
  if(Refresh_Edit_Box_R_FF){
    Refresh_Edit_Box_R_FF =0;
    //  tft.fillRoundRect(Xstart, YStart, Xwidth, Yheight, corner Radius, color);
    tft.fillRoundRect(RightBox_x_Begin, Mode_Box_y_Begin, BoxWidth, BoxHeight, 8, Box_Color);  //color565(0, 0, 255));
    tft.setTextColor(Text_Color);
    tft.setTextSize(4);
    tft.setCursor(RightBox_x_Begin+10,Mode_Box_y_Begin+6);
    tft.print("  Press  ");
    tft.setTextSize(3);
    tft.setCursor(RightBox_x_Begin+10,Mode_Box_y_Begin+41);
    if(Test_Right_Mode == Begin_Test) tft.print("  to RETURN");
    else                tft.print("  to Start ");
  }
}
// _______ End Check_Right_Refresh_________________________________________________________



// OUTGOING SERIAL DATA PROCESSING CODE----------------------------------------


// ***************************************************************************************
void sendDataToSerial()
{
  // Send data out separated by a comma (kDelimiter)
  // Repeat next 2 lines of code for each variable sent:
  Serial.print(Left_ThermocoupleTemperature);Serial.print(kDelimiter);
  Serial.print(Right_ThermocoupleTemperature);Serial.print(kDelimiter);
  Serial.print(L_Probe_Resistance);Serial.print(kDelimiter);
  Serial.print(R_Probe_Resistance);Serial.print(kDelimiter);
  
  Serial.println(); // Add final line ending character only once
}
//-----------sendDataToSerial------------------------------------------------------------



// ***************************************************************************************
void processOutgoingSerial()
{
   // Enter into this only when serial interval has elapsed
  if((millis() - serialPreviousTime) > kSerialInterval) 
  {
    serialPreviousTime = millis(); // Reset serial interval timestamp
    sendDataToSerial(); 
  }
}
//-----------processOutgoingSerial--------------------------------------------------------




// INCOMING SERIAL DATA PROCESSING CODE----------------------------------------



// ***************************************************************************************
void processIncomingSerial()
{
  if(Serial.available()){
    parseData(GetSerialData());
  }
}

//-----------processIncomingSerial--------------------------------------------------------



// ***************************************************************************************
// Gathers bytes from serial port to build inputString
char* GetSerialData()
{
  static char inputString[64]; // Create a char array to store incoming data
  memset(inputString, 0, sizeof(inputString)); // Clear the memory from a pervious reading
  while (Serial.available()){
    Serial.readBytesUntil('\n', inputString, 64); //Read every byte in Serial buffer until line end or 64 bytes
  }
  return inputString;
}
//-----------END_GetSerialData-------l----------------------------------------------------



// ***************************************************************************************
// Seperate the data at each delimeter
void parseData(char data[])
{
    char *token = strtok(data, ","); // Find the first delimeter and return the token before it
    int index = 0; // Index to track storage in the array
    while (token != NULL){ // Char* strings terminate w/ a Null character. We'll keep running the command until we hit it
      arr[index] = token; // Assign the token to an array
      token = strtok(NULL, ","); // Conintue to the next delimeter
      index++; // incremenet index to store next value
    }
}
//-----------parseData--------------------------------------------------------------------


// ****************************************************************************************
void setup()
{
//  pinMode(47, OUTPUT);
//  digitalWrite(47, 1);
  Analog_2_Digital_Init();
  Timer1_init();  // Setup Timer 1 for 10mSecs

  Serial.begin(9600);
  while (!Serial) delay(10);
  
  SPI.begin();
  Waveshield.begin();

  Thermocouple_Init();
}
// _____________End_Setup_________________________________________________________________



// ***************************************************************************************
// the loop function runs over and over again until power down or reset
void loop(){
  if(mSec10_FF){
    mSec10_FF = 0;            // Reset Flip Flop
    Check_Touch();
    ADS.readADC(0);      // Read Channel 0
    L_Probe_Resistance = (ADS.getValue()*A_D_Scale)/I_R_Divide;
    ADS.readADC(1);      // Read Channel 1
    R_Probe_Resistance = (ADS.getValue()*A_D_Scale)/I_R_Divide;
  }
  if(mSec1000_FF){
    mSec1000_FF=Off;
    sendDataToSerial();
    Thermocouple_Init();
  }

    
  switch (Operating_Mode){        // Check Sequence
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    case Start_Up:    // Initial Power Mode State
      tft.fillScreen(BLACK);
      tft.setRotation(1);
      RoundRects();
      tft.setCursor(0, 20);
      tft.setTextSize(3);
      tft.setTextColor(WHITE);
      tft.print(" Alcor Thermocouple Tester");
      // tft.fillRoundRect(Xstart, YStart, Xwidth, Yheight, corner Radius, color);
      tft.fillRoundRect(19, 160, 440, 140, 10, RED);  //color565(0, 0, 255));
      Operating_Mode = Wait_For_Start;
    break;
// ~ ~ ~ End Of Start_Up ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case Wait_For_Start:      // Waiting for Red Start Button to be Pressed
      tft.setTextSize(3);         //  tft.setTextSize(3);
      tft.setCursor(60, 220);       //  
      if(mSec250_FF == 0)   tft.setTextColor(RED);// Overwrite in red to erase old text
      else          tft.setTextColor(WHITE); 
      tft.print("Touch Screen TO Begin");
      if(Screen_Touch_FF){
        Operating_Mode = Idle;      // Move Operating_Mode to the Idle mode
        tft.fillScreen(BLACK);                // Start Screen over
        Refresh_Temp_Box_L_FF = 1;              // force display update
        Refresh_Temp_Box_R_FF = 1;              // force display update
        Refresh_Edit_Box_L_FF = 1;              // Set Flip Flop to update Left Box
        Refresh_Edit_Box_R_FF = 1;              // Set Flip Flop to update Right Box
        while(Screen_Touch_FF) Check_Touch();
      }
    break;  
// ~ ~ ~ ~ ~ ~ End Case Wait for Start ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case Idle:      // Waiting for screen to be touched
    if (Screen_Touch_FF){         //  Check if the screen has been touched
    //      tft.fillScreen(BLACK);
      tft.setRotation(1);
      tft.drawRoundRect(0, 0, 479, 319, 3, WHITE );    // Print Boarder (WHITE)
      maxthermo_0.triggerOneShot();           // Initiate Temperature Read Channel 0
      maxthermo_1.triggerOneShot();
      Left_TC_Temperature_Last = 1;           // Zero last temp value to force display update
      Right_TC_Temperature_Last = 1;          // Zero last temp value to force display update
      Refresh_Edit_Box_L_FF = 1;              // Set Flip Flop to update Left Box
      Refresh_Edit_Box_R_FF = 1;              // Set Flip Flop to update Right Box
      tft.setTextColor(BLACK);
      while(Screen_Touch_FF) Check_Touch();
    break;  
    }
// ~ ~ ~ ~ ~ ~ End Case Idle ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~





// ***************************************************************************************
    switch (Test_Left_Mode){
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case Standby: 
  // """""""""""  Draw Begin Test Button Area   """""""""""""""""""""""""""""""
        Check_Left_Refresh(CYAN, BLACK);
        if(Left_Mode_Box_Touch_FF){              //  Check if the Left Button has been touched
          Left_Mode_Box_Touch_FF = Off;
          Refresh_Edit_Box_L_FF = 1;            // Set Flip Flop to update Left Box
          while(Screen_Touch_FF) Check_Touch();
          Test_Left_Mode = Begin_Test;          // Index to Next Step
        }
      break;
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case Begin_Test:
        Check_Left_Refresh(RED, WHITE);
        if(Left_Mode_Box_Touch_FF){              //  Check if the Left Button has been touched
          Left_Mode_Box_Touch_FF = Off;
          Refresh_Edit_Box_L_FF = 1;            // Set Flip Flop to update Left Box
          while(Screen_Touch_FF) Check_Touch();
          Test_Left_Mode = Standby;           // Index to Next Step
        }
      break;
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    }
// ***************************************************************************************
    switch (Test_Right_Mode){
      case Standby: 
        Check_Right_Refresh(CYAN, BLACK);
        if(Right_Mode_Box_Touch_FF){
          Right_Mode_Box_Touch_FF = Off;
          Refresh_Edit_Box_R_FF=1;
          while(Screen_Touch_FF) Check_Touch();
          Test_Right_Mode = Begin_Test;
        }       
      break;
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case Begin_Test:
        Check_Right_Refresh(RED, WHITE);
        if(Right_Mode_Box_Touch_FF){
          Right_Mode_Box_Touch_FF = Off;
          Refresh_Edit_Box_R_FF=1;
          while(Screen_Touch_FF) Check_Touch();
          Test_Right_Mode = Standby;
        }
      break;
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    }
    
//---------------------------------------------------------------------------------------
    if (maxthermo_0.conversionComplete()){  // check for conversion complete and read temperature 
      Left_ThermocoupleTemperature = (maxthermo_0.readThermocoupleTemperature()*1.8)+32;
      if(Left_ThermocoupleTemperature != Left_TC_Temperature_Last){
        Left_TC_Temperature_Last = Left_ThermocoupleTemperature;
        Refresh_Temp_Box_L_FF = ON;
      }
      maxthermo_0.triggerOneShot();
    }
    if (maxthermo_1.conversionComplete()){
      Right_ThermocoupleTemperature = (maxthermo_1.readThermocoupleTemperature()*1.8)+32;
      if(Right_ThermocoupleTemperature != Right_TC_Temperature_Last){
        Right_TC_Temperature_Last = Right_ThermocoupleTemperature;
        Refresh_Temp_Box_R_FF = ON;
      }
      maxthermo_1.triggerOneShot();
    }

    
    if(Refresh_Temp_Box_L_FF == ON){
      Refresh_Temp_Box_L_FF = OFF;
      switch (Left_TC_Type){
        case K_Type:   Draw_Left_Temp_Box(YELLOW,BLACK);tft.print("K-TYPE "); break;
        case T_Type:   Draw_Left_Temp_Box(BLUE,WHITE);  tft.print("T-TYPE "); break;
        case J_Type:   Draw_Left_Temp_Box(BLACK,WHITE);HighLight_Left_Temp_Box(WHITE); tft.print("J-TYPE "); break;
        default:       Draw_Left_Temp_Box(RED,WHITE);   tft.print("TC_Fault");break;
      //case TC_Fault: Draw_Left_Temp_Box(RED,WHITE);   tft.print("TC_Fault");break;
      }
      tft.setCursor(LeftBox_x_Begin+10,292);
      tft.setTextSize(2);
      tft.setCursor(LeftBox_x_Begin+100,Temp_Box_y_Begin+6);
      tft.setTextSize(4);
      if(Left_ThermocoupleTemperature < 2450) Print_Justify(Left_ThermocoupleTemperature);
      else  tft.print("OPEN ");
    }
   
    if(Refresh_Temp_Box_R_FF == ON){
      Refresh_Temp_Box_R_FF = OFF;
      switch (Right_TC_Type){
        case K_Type:    Draw_Right_Temp_Box(YELLOW,BLACK);tft.print("K-TYPE "); break;
        case T_Type:    Draw_Right_Temp_Box(BLUE,WHITE); tft.print("T-TYPE "); break;
        case J_Type:    Draw_Right_Temp_Box(BLACK,WHITE);HighLight_Right_Temp_Box(WHITE); tft.print("J-TYPE "); break;
        default:        Draw_Right_Temp_Box(RED,WHITE);  tft.print("TC_Fault");break;
        //case TC_Fault:  Draw_Right_Temp_Box(RED,WHITE);  tft.print("TC_Fault");break;
      }
      tft.setCursor(RightBox_x_Begin+100,Temp_Box_y_Begin+6);
      tft.setTextSize(4);
      if(Right_ThermocoupleTemperature < 2450) Print_Justify(Right_ThermocoupleTemperature);
      else  tft.print("OPEN ");
      }    
  break;  
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

} // __________End of Switch Operating Mode______________________________________________

}// __________End of LOOP__________________________________________________________________
