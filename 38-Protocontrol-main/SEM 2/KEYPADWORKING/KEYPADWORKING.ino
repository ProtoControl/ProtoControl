#include "Wire.h"
#include "I2CKeyPad.h"

const uint8_t KEYPAD_ADDRESS = 0x20;

I2CKeyPad keyPad(KEYPAD_ADDRESS);

uint32_t start, stop;
uint32_t lastKeyPressed = 0;

const char keys[17] = "0147*N2580N369#";
void setup()
{
  Serial.begin(115200);
  Serial.println(__FILE__);
  
  Wire.begin();
  Wire.setClock(400000);
  if (keyPad.begin() == false)
  {
    Serial.println("\nERROR: cannot communicate to keypad.\nPlease reboot.\n");
    while(1);
  }

  keyPad.setKeyPadMode(I2C_KEYPAD_5x3);
  
}


void loop()
{
  uint8_t index = keyPad.getKey();
  if (index < 16){
    lastKeyPressed = index;
    Serial.println(keys[index]);
    delay(100);
    //Serial.print("\n");
  }
}

