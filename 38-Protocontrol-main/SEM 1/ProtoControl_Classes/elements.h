//Classes for digital components
#define sx 320
#define sy 240
//Button class:
class PushButton {
public:
    PushButton(int x, int y, int w, int h, int color, int textColor, const char text[]);
    void drawButton(Adafruit_ILI9341 &tft);
    void handleTouch(int x, int y);
    void updateDisplay(const char c[3][16], Adafruit_ILI9341 &tft);
    bool pressed;

private:
    //Adafruit_ILI9341 &tft;
    int x;
    int y;
    int w;
    int h;
    int color;
    int textColor;
    char text[16];
    
    char previousDisplay[3][16];
    static const char BOX = 218;
};

PushButton::PushButton(int x, int y, int w, int h, int color, int textColor, const char text[]) {
    //this->tft = tft;
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->color = color;
    this->textColor = textColor;
    strncpy(this->text, text, sizeof(this->text));
    this->pressed = false;
}

void PushButton::drawButton(Adafruit_ILI9341 &tft) {
    // Implement the drawing logic.
  tft.fillRect(this->x,this->y,this->w,this->h,this->color);
  tft.drawRect(this->x,this->y,this->w,this->h,ILI9341_BLACK);
  tft.setCursor(this->x+15,this->y+(h/3));
  tft.setTextColor(this->textColor);
  tft.setTextSize(3);
  tft.println(this->text);
}

void PushButton::handleTouch(int px, int py) {
    // Implement the touch handling logic.
    if((py < (sx - (this->w + this->x))) && py > (this->x) ){
      if((px > 5) && (px < 50)){
        if(pressed){
          pressed = false;
        }else{
          pressed = true;
        }

      }
    }
}

void PushButton::updateDisplay(const char c[3][16], Adafruit_ILI9341 &tft) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 16; j++) {
            if (c[i][j] != previousDisplay[i][j]) {
                // Update the display for that character
                tft.setCursor(25 + j * 12, 25 + i * 10);
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



//class for toggle:


