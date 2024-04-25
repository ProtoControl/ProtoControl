#include <Adafruit_GFX.h>
#include <SPI.h>
#include "Adafruit_HX8357.h"
#include "TouchScreen.h"
#include "Adafruit_seesaw.h"
#include <seesaw_neopixel.h>

// Constants
#define SS_SWITCH        24
#define SS_NEOPIX        6
#define SEESAW_ADDR      0x36
#define TS_MINX          110
#define TS_MINY          80
#define TS_MAXX          900
#define TS_MAXY          940
#define MINPRESSURE      10
#define MAXPRESSURE      1000
#define BOXSIZE          40
#define PENRADIUS        3
#define tft_RST          -1

// Touchscreen pins
#define YP A2
#define XM A3
#define YM 7
#define XP 8

// Display settings
Adafruit_HX8357 tft = Adafruit_HX8357(tft_CS, tft_DC, tft_RST);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Seesaw and Neopixel
Adafruit_seesaw ss;
seesaw_NeoPixel sspixel = seesaw_NeoPixel(1, SS_NEOPIX, NEO_GRB + NEO_KHZ800);
int32_t encoder_position;

// Application-specific variables
bool on = false;
int slide = 60;
char con[3][16];




void setup() {
    initSerial();
    initSeesaw();
    initDisplay();
    initTouchButtons();
}

void loop() {
    TSPoint p = ts.getPoint();
    handleButtonPress();
    handleEncoderChange();
    handleTouchInput();
}

void initSerial() {
    while (!Serial);
    Serial.begin(115200);
    Serial.println(F("Touch Paint!"));
}

void initSeesaw() {
    if (!ss.begin(SEESAW_ADDR) || !sspixel.begin(SEESAW_ADDR)) {
        Serial.println("Couldn't find seesaw on default address");
    } else {
        Serial.println("seesaw started");
        sspixel.setBrightness(20);
        sspixel.show();
        ss.pinMode(SS_SWITCH, INPUT_PULLUP);
        encoder_position = ss.getEncoderPosition();
        delay(10);
        ss.setGPIOInterrupts((uint32_t)1 << SS_SWITCH, 1);
        ss.enableEncoderInterrupt();
    }
}

void initDisplay() {
    tft.begin();
    tft.fillScreen(HX8357_CYAN);
}

void initTouchButtons() {
    drawButton(20, 20, 100, 200, HX8357_GREEN, "Press");
    drawButton(260, 140, 200, 100, HX8357_BLUE, "Off");
    // Additional GUI elements like sliders can be initialized here
    strcpy(con[0], "Analog In: ");
}

void drawButton(int x, int y, int w, int h, uint16_t color, const char* label) {
    tft.fillRect(x, y, w, h, color);
    tft.drawRect(x, y, w, h, HX8357_BLACK);
    tft.setCursor(x + w / 4, y + h / 4);
    tft.setTextColor(HX8357_WHITE);
    tft.setTextSize(4);
    tft.println(label);
}

void handleButtonPress() {
    if (!ss.digitalRead(SS_SWITCH)) {
        Serial.println("Button pressed!");
        encoder_position = 0;
    }
}

void handleEncoderChange() {
    int32_t new_position = ss.getEncoderPosition();
    if (encoder_position != new_position) {
        updateEncoderDisplay(new_position);
        updateNeopixelColor(new_position);
        encoder_position = new_position;
    }
}

void updateEncoderDisplay(int position) {
    tft.fillRect(20, 20, 220, 100, HX8357_BLACK);
    tft.drawRect(20, 20, 220, 100, HX8357_WHITE);
    tft.setCursor(40, 60);
    tft.setTextColor(HX8357_WHITE);
    tft.setTextSize(2);
    tft.print(con[0]);
    tft.println(position);
}

void updateNeopixelColor(int position) {
    sspixel.setPixelColor(0, Wheel(position & 0xFF));
    sspixel.show();
}
void handleTouchInput() {
    //p = ts.getPoint();
    if (isValidTouch(p)) {
        handleTouchAction(p);
    }
}

bool isValidTouch(TSPoint p) {
    return (p.z >= MINPRESSURE && p.z <= MAXPRESSURE);
}

void handleTouchAction(TSPoint p) {
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
    if (isButtonPressed(p, 20, 20, 190, 140)) {
        toggleButtonAction();
    } else if (isButtonPressed(p, 230, 20, 370, 142)) {
        offButtonAction();
    } else {
        // Handle other touch events like sliders here
    }
}

bool isButtonPressed(TSPoint p, int x1, int y1, int x2, int y2) {
    return (p.x >= x1 && p.x <= x2 && p.y >= y1 && p.y <= y2);
}

void toggleButtonAction() {
    tft.fillRect(20, 20, 220, 100, HX8357_BLACK);
    tft.drawRect(20, 20, 220, 100, HX8357_WHITE);
    if (!on) {
        tft.print("GREEN");
        digitalWrite(3, HIGH);
        digitalWrite(4, LOW);
        on = true;
    } else {
        tft.print("RED");
        digitalWrite(3, LOW);
        digitalWrite(4, HIGH);
        on = false;
    }
    delay(150);
}

void offButtonAction() {
    tft.fillRect(20, 20, 220, 100, HX8357_BLACK);
    tft.drawRect(20, 20, 220, 100, HX8357_WHITE);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    tft.print("OFF");
}

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
