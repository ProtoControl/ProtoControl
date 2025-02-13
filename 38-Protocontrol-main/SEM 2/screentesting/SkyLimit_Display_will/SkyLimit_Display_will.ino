// Base Libraries
#include <Wire.h>
#include <SPI.h>
// LCD LibrariesA
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
// Sensor Libraries
#include <Adafruit_FT6206.h>

// Misc. Libraries

//----Pinouts----------------------------------------------------------------------------------------------
// LCD Pinouts - Follow these EXACTLY

//I have to redefine these for my ESP - change back to yor shit, which is CS - 15 and DC - 2
#define TFT_CS 10
#define TFT_DC 9
// Connect the rest of the pins as follows:
// MOSI 11
// CLK 12
// RST 18 (not particularly important, experiment without using this)
// MISO 13
// CD connect to GND

// Sensor Pinouts

//--------------------------------------------------------------------------------------------------------- 

// Declare LCD object 
// We use hardware SPI for this display
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Buttons 
struct Button {
  uint8_t PIN;
  uint32_t numKeyPress;
  bool pressed;
};
#define button1Pin 9
Button button1 = {button1Pin, 0, false};
void IRAM_ATTR isr() {
  // Tracks button1 pressing
  button1.numKeyPress += 1;
  button1.pressed = true; // Make sure to update this after the function returns
}
int start = 0; // Flag to allow us to use the same button for start up and polling sensors

// LCD Constants
#define SENSOR_VALUE_X 10
#define SENSOR_VALUE_Y 45
#define SENSOR_LABEL_X 10
#define SENSOR_LABEL_Y 30 
#define SENSOR_SPACING 30

// 'compass-icon-1', 32x32px
const unsigned char compassBitmap[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 
    0x00, 0x0F, 0xF0, 0x00, 0x00, 0x7F, 0xFC, 0x00, 0x00, 0xF0, 0x1F, 0x00, 0x03, 0xC3, 0xC7, 0x80, 
    0x07, 0x1F, 0xF9, 0xC0, 0x06, 0x7F, 0xFC, 0xE0, 0x0E, 0xFF, 0xFE, 0x70, 0x1D, 0xFF, 0xF7, 0x30, 
    0x18, 0xFD, 0xEF, 0xB8, 0x1B, 0xFC, 0xCF, 0x98, 0x33, 0xFC, 0x9F, 0xD8, 0x33, 0xF3, 0x3F, 0x98, 
    0x37, 0xFE, 0x23, 0xDC, 0x37, 0xF5, 0x5F, 0xDC, 0x37, 0xE4, 0x7F, 0xDC, 0x33, 0xF8, 0xDF, 0xD8, 
    0x33, 0xF3, 0x7F, 0xD8, 0x3B, 0xF7, 0x7F, 0x98, 0x19, 0xEF, 0x7F, 0xB8, 0x1D, 0xDF, 0xFF, 0x30, 
    0x0C, 0xFF, 0xFE, 0x70, 0x0E, 0x7F, 0xFC, 0xE0, 0x07, 0x1F, 0xF9, 0xC0, 0x03, 0xC7, 0xE3, 0x80, 
    0x01, 0xF0, 0x0F, 0x00, 0x00, 0x7F, 0xFE, 0x00, 0x00, 0x1F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char thermoBitmap [] PROGMEM = {
	0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x00, 0x03, 0xff, 0x80, 0x00, 0x00, 
	0x03, 0x83, 0x80, 0x00, 0x00, 0x07, 0x01, 0xc0, 0x00, 0x00, 0x07, 0x01, 0xc0, 0x00, 0x00, 0x06, 
	0x00, 0xc0, 0x00, 0x00, 0x06, 0x00, 0xc0, 0x00, 0x00, 0x07, 0xe0, 0xc0, 0x00, 0x00, 0x07, 0xe0, 
	0xc0, 0x00, 0x00, 0x06, 0x00, 0xc0, 0x00, 0x00, 0x06, 0x00, 0xc0, 0x00, 0x00, 0x07, 0xe0, 0xc0, 
	0x00, 0x00, 0x07, 0xe0, 0xc0, 0x00, 0x00, 0x06, 0x00, 0xc0, 0x00, 0x00, 0x06, 0x00, 0xc0, 0x00, 
	0x00, 0x06, 0x7c, 0xc0, 0x00, 0x00, 0x06, 0x7c, 0xc0, 0x00, 0x00, 0x06, 0x7c, 0xc0, 0x00, 0x00, 
	0x06, 0x7c, 0xc0, 0x00, 0x00, 0x06, 0x7c, 0xc0, 0x00, 0x00, 0x06, 0x7c, 0xc0, 0x00, 0x00, 0x06, 
	0x7c, 0xc0, 0x00, 0x00, 0x06, 0x7c, 0xc0, 0x00, 0x00, 0x0e, 0x7c, 0xe0, 0x00, 0x00, 0x1c, 0x7c, 
	0x70, 0x00, 0x00, 0x1c, 0x7c, 0x70, 0x00, 0x00, 0x18, 0xfe, 0x30, 0x00, 0x00, 0x39, 0xff, 0x38, 
	0x00, 0x00, 0x39, 0xff, 0x38, 0x00, 0x00, 0x39, 0xff, 0x38, 0x00, 0x00, 0x38, 0xfe, 0x38, 0x00, 
	0x00, 0x18, 0xfe, 0x30, 0x00, 0x00, 0x1c, 0x7c, 0x70, 0x00, 0x00, 0x0e, 0x00, 0xe0, 0x00, 0x00, 
	0x0f, 0x01, 0xe0, 0x00, 0x00, 0x07, 0xff, 0xc0, 0x00, 0x00, 0x03, 0xff, 0x80, 0x00, 0x00, 0x00, 
	0x7c, 0x00, 0x00
};
const unsigned char humidityBitmap [] PROGMEM = {
	0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 
	0x01, 0xf0, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x07, 
	0xbc, 0x00, 0x00, 0x00, 0x07, 0x1c, 0x00, 0x00, 0x00, 0x0f, 0x1e, 0x00, 0x00, 0x00, 0x1e, 0x0f, 
	0x00, 0x00, 0x00, 0x1c, 0x07, 0x00, 0x00, 0x00, 0x3c, 0x07, 0x80, 0x00, 0x00, 0x78, 0x03, 0x80, 
	0x00, 0x00, 0x78, 0x03, 0xc0, 0x00, 0x00, 0xf0, 0x01, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0xe0, 0x00, 
	0x01, 0xe0, 0x00, 0xf0, 0x00, 0x01, 0xc0, 0x00, 0x70, 0x00, 0x03, 0xc0, 0x00, 0x78, 0x00, 0x03, 
	0x80, 0x00, 0x38, 0x00, 0x07, 0x9c, 0x02, 0x3c, 0x00, 0x0f, 0x3e, 0x06, 0x1c, 0x00, 0x0f, 0x23, 
	0x0e, 0x1e, 0x00, 0x0e, 0x23, 0x0c, 0x0e, 0x00, 0x1e, 0x23, 0x18, 0x0f, 0x00, 0x1c, 0x33, 0x18, 
	0x07, 0x00, 0x3c, 0x3e, 0x30, 0x07, 0x80, 0x38, 0x0c, 0x70, 0x03, 0x80, 0x78, 0x00, 0x60, 0x03, 
	0xc0, 0x70, 0x00, 0xc0, 0x03, 0xc0, 0x70, 0x01, 0xc0, 0x01, 0xc0, 0x70, 0x01, 0x8f, 0x01, 0xc0, 
	0xf0, 0x03, 0x99, 0x81, 0xe0, 0xe0, 0x03, 0x18, 0x80, 0xe0, 0xe0, 0x06, 0x18, 0x80, 0xe0, 0xe0, 
	0x0e, 0x18, 0x80, 0xe0, 0xe0, 0x0c, 0x19, 0x80, 0xe0, 0xf0, 0x08, 0x0f, 0x01, 0xe0, 0x70, 0x00, 
	0x00, 0x01, 0xc0, 0x73, 0x80, 0x00, 0x39, 0xc0, 0x7f, 0xf0, 0x01, 0xff, 0xc0, 0x7f, 0xfe, 0x0f, 
	0xff, 0xc0, 0x3f, 0xff, 0xff, 0xff, 0x80, 0x3f, 0xff, 0xff, 0xff, 0x80, 0x1f, 0xff, 0xff, 0xff, 
	0x00, 0x0f, 0xff, 0xff, 0xfe, 0x00, 0x07, 0xff, 0xff, 0xfc, 0x00, 0x03, 0xff, 0xff, 0xf8, 0x00, 
	0x01, 0xff, 0xff, 0xf0, 0x00, 0x00, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x3f, 0xff, 0x80, 0x00, 0x00, 
	0x03, 0xf8, 0x00, 0x00
};
const unsigned char airPressureBitmap [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 
	0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 
	0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 
	0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 
	0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 
	0x00, 0x00, 0x00, 0x00, 0x3f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf8, 0x00, 0x00, 0x00, 0x00, 
	0x3f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf0, 0x04, 0x00, 
	0x00, 0x00, 0x0f, 0xe0, 0x1f, 0x80, 0x00, 0x00, 0x07, 0xc0, 0x7f, 0xc0, 0x00, 0x00, 0x07, 0xc0, 
	0x7b, 0xe0, 0x00, 0x00, 0x03, 0x80, 0xf0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x70, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 
	0x0f, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xff, 0xff, 0xff, 
	0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 
	0xff, 0xff, 0xff, 0xf0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xe0, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x1f, 0xff, 0xff, 0xff, 
	0xff, 0xf0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char uvIndexBitmap [] PROGMEM = {
	0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 
	0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x01, 
	0xf8, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x01, 0xf8, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0xf8, 0x00, 
	0x00, 0x07, 0xc0, 0x00, 0x00, 0x78, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x0f, 0xfc, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0x80, 0x00, 0x00, 
	0x00, 0x00, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x03, 
	0xf8, 0x07, 0xf0, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x01, 
	0xf8, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0xf8, 0x00, 
	0x00, 0x7e, 0x07, 0xc0, 0x00, 0xf8, 0x1f, 0x80, 0xfe, 0x07, 0xc0, 0x00, 0xfc, 0x1f, 0xc0, 0xfe, 
	0x07, 0x80, 0x00, 0x78, 0x1f, 0xc0, 0x7e, 0x07, 0x80, 0x00, 0x78, 0x1f, 0x80, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x20, 0x10, 0x10, 0x01, 0x00, 0x00, 0x00, 0xf0, 0x3c, 0x7c, 0x03, 0xc0, 0x00, 0x00, 0xf0, 
	0x3e, 0x7c, 0x07, 0xc0, 0x00, 0x01, 0xf8, 0x3e, 0x7c, 0x07, 0xc0, 0x00, 0x00, 0xf8, 0x3e, 0x7e, 
	0x07, 0xc0, 0x00, 0x00, 0xf8, 0x3e, 0x3e, 0x0f, 0xc0, 0x00, 0x00, 0xf8, 0x3e, 0x3f, 0x0f, 0x80, 
	0x00, 0x00, 0xf8, 0x3e, 0x1f, 0x1f, 0x80, 0x00, 0x00, 0xf8, 0x3e, 0x1f, 0x1f, 0x00, 0x00, 0x00, 
	0xf8, 0x3e, 0x1f, 0x9f, 0x00, 0x00, 0x00, 0xf0, 0x3e, 0x0f, 0xbf, 0x00, 0x00, 0x00, 0xf8, 0x7e, 
	0x0f, 0xfe, 0x00, 0x00, 0x00, 0xfc, 0xfc, 0x07, 0xfe, 0x00, 0x00, 0x00, 0xff, 0xfc, 0x07, 0xfc, 
	0x00, 0x00, 0x00, 0x7f, 0xf8, 0x07, 0xfc, 0x00, 0x00, 0x00, 0x7f, 0xf0, 0x03, 0xfc, 0x00, 0x00, 
	0x00, 0x1f, 0xe0, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x07, 0x80, 0x01, 0xf0, 0x00, 0x00
};
const unsigned char altitudeBitmap [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x03, 0x80, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0xf3, 0xc0, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x07, 0xe1, 0xf3, 0xe0, 0x70, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf3, 0xe1, 
	0xe0, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf3, 0xc1, 0xe1, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x1f, 
	0xff, 0xc0, 0xc3, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x7f, 0x80, 0x07, 0xff, 0x00, 0x00, 0x00, 
	0x00, 0x3c, 0x3f, 0x00, 0x0f, 0xff, 0x80, 0x00, 0x00, 0x00, 0x7c, 0x1f, 0x00, 0x1f, 0xff, 0xc0, 
	0x00, 0x00, 0x00, 0x78, 0x1e, 0x00, 0x1f, 0x7b, 0xe0, 0x00, 0x00, 0x00, 0xf8, 0x04, 0x00, 0x1e, 
	0x79, 0xe0, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x0c, 0x78, 0xc0, 0x00, 0x00, 0x01, 0xe0, 0x00, 
	0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x03, 
	0xc0, 0x78, 0x3e, 0x00, 0x78, 0x00, 0x00, 0x00, 0x07, 0xf1, 0xfc, 0xff, 0x84, 0x78, 0x00, 0x00, 
	0x00, 0x07, 0xff, 0xff, 0xff, 0xfc, 0x78, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xfc, 0x78, 
	0x00, 0x00, 0x00, 0x0f, 0x7f, 0xcf, 0xe1, 0xfc, 0x78, 0x00, 0x00, 0x00, 0x1e, 0x1f, 0x07, 0x80, 
	0x78, 0x78, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x3c, 0x00, 
	0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 
	0xf8, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x78, 0x80, 
	0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x30, 0x80, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0xc0, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x03, 0xc0, 0x00, 0x00, 
	0x00, 0x00, 0x01, 0xe0, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x0f, 0x80, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 
	0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x3c, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xfe, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x1f, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Constants for icon positions and sizes
const int ICON_START_X = 50;
const int ICON_START_Y_COMPASS = 170;
const int ICON_START_Y_THERMO = 50;
const int ICON_START_Y_HUMIDITY = 100;
const int ICON_COLUMN_OFFSET = 175; // Adjusted for second column alignment
const int ICON_SPACING_Y = 50; // Vertical spacing between icons

// Icon dimensions
const int COMPASS_ICON_SIZE = 32;
const int THERMO_ICON_WIDTH = 39;
const int THERMO_ICON_HEIGHT = 39;
const int HUMIDITY_ICON_WIDTH = 35;
const int HUMIDITY_ICON_HEIGHT = 52;
const int AIR_PRESSURE_ICON_SIZE = 47;
const int UV_INDEX_ICON_SIZE = 50;
const int ALTITUDE_ICON_SIZE = 65;

void displayIcons() {
  // Displaying icons in the first column
  tft.drawBitmap(ICON_START_X, ICON_START_Y_COMPASS, compassBitmap, COMPASS_ICON_SIZE, COMPASS_ICON_SIZE, ILI9341_WHITE);
  tft.drawBitmap(ICON_START_X, ICON_START_Y_THERMO, thermoBitmap, THERMO_ICON_WIDTH, THERMO_ICON_HEIGHT, ILI9341_WHITE);
  tft.drawBitmap(ICON_START_X, ICON_START_Y_HUMIDITY, humidityBitmap, HUMIDITY_ICON_WIDTH, HUMIDITY_ICON_HEIGHT, ILI9341_WHITE);
  
  // Displaying icons in the second column with offset
  int secondColumnX = ICON_START_X + ICON_COLUMN_OFFSET;
  tft.drawBitmap(secondColumnX, ICON_START_Y_THERMO, airPressureBitmap, AIR_PRESSURE_ICON_SIZE, AIR_PRESSURE_ICON_SIZE, ILI9341_WHITE);
  tft.drawBitmap(secondColumnX, ICON_START_Y_HUMIDITY, uvIndexBitmap, UV_INDEX_ICON_SIZE, UV_INDEX_ICON_SIZE, ILI9341_WHITE);
  tft.drawBitmap(secondColumnX, ICON_START_Y_COMPASS, altitudeBitmap, ALTITUDE_ICON_SIZE, ALTITUDE_ICON_SIZE, ILI9341_WHITE);
}


void setup() {
  Serial.begin(115200);
  //while (!Serial) delay(10);

  // Initialize buttons
 // pinMode(button1.PIN, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(button1.PIN), isr, FALLING);

  // Initialize display
  tft.begin();
  tft.setRotation(3); // Set rotation of display

  // Create start up GUI
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.print("Welcome to SkyLimit!");
  tft.setCursor(10, 50);
  tft.setTextSize(1);
  tft.print("Press Button to begin");
}

void loop() {
  //if (button1.pressed && (start == 0)) {
    // Change display to main screen
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.setCursor(0, 0);
    tft.print("SkyLimit");

    tft.setTextSize(1);
    // Print labels
    tft.setCursor(SENSOR_LABEL_X, SENSOR_LABEL_Y);
    tft.print("Temperature: ");

    tft.setCursor(SENSOR_LABEL_X, SENSOR_LABEL_Y + SENSOR_SPACING);
    tft.print("Humidity: ");

    tft.setCursor(SENSOR_LABEL_X, SENSOR_LABEL_Y + 2 * SENSOR_SPACING);
    tft.print("Pressure: ");

    tft.setCursor(SENSOR_LABEL_X, SENSOR_LABEL_Y + 3 * SENSOR_SPACING);
    tft.print("Altitude: ");

    tft.setCursor(SENSOR_LABEL_X, SENSOR_LABEL_Y + 4 * SENSOR_SPACING);
    tft.print("UV Index: ");

    tft.setCursor(SENSOR_LABEL_X, SENSOR_LABEL_Y + 5 * SENSOR_SPACING);
    tft.print("Compass: ");
    displayIcons(); // Display the compass icon

    start = 1;
    button1.pressed = false; // Update button press
  //}

  if (button1.pressed && (start == 1)) {
    // Function to update sensor values would go here
    button1.pressed = false;
  }
}
