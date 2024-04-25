//Classes for digital components
#define sx 320
#define sy 240
#define PushButtonW 120
#define PushButtonH 50
#define ToggleW 100
#define ToggleH 50
//Button class:
class PushButton {
public:
    PushButton(int x, int y, int color, int textColor, const char text[]);
    PushButton();
    void drawButton(Adafruit_ILI9341 &tft);
    void handleTouch(int x, int y,Adafruit_ILI9341 &tft);
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

PushButton::PushButton(int x, int y, int color, int textColor, const char text[]) {
    //this->tft = tft;
    this->x = x;
    this->y = y;
    this->color = color;
    this->textColor = textColor;
    strncpy(this->text, text, sizeof(this->text));
    this->pressed = false;
}

PushButton::PushButton() {
    //this->tft = tft;
    this->x = 20;
    this->y = 20;
    this->color = ILI9341_RED;
    this->textColor = ILI9341_BLACK;
    char text[6] = "PRESS";
    strncpy(this->text, text, sizeof(this->text));
    this->pressed = false;
}

void PushButton::drawButton(Adafruit_ILI9341 &tft) {
    // Implement the drawing logic.
  tft.fillRect(this->x,this->y,PushButtonW,PushButtonH,this->color);

  tft.drawRect(this->x,this->y,PushButtonW,PushButtonH,ILI9341_BLACK);

  tft.setCursor(this->x+15,this->y+(PushButtonH/3));
  tft.setTextColor(this->textColor);
  tft.setTextSize(3);
  tft.println(this->text);
}

void PushButton::handleTouch(int px, int py, Adafruit_ILI9341 &tft) {
    // Implement the touch handling logic.
    if(px < (PushButtonW + this->x) && px > (this->x) ){
      if((py > (sy - (this->y + PushButtonH))) && py < (sy - this->y)){
        
        if(pressed){
          tft.drawRect(this->x,this->y,PushButtonW,PushButtonH,ILI9341_WHITE);
          this->pressed = false;
          delay(50);
        }else{
          tft.drawRect(this->x,this->y,PushButtonW,PushButtonH,ILI9341_BLACK);
          this->pressed = true;
          delay(50);
        }

      }
    }
}

void PushButton::updateDisplay(const char c[3][16], Adafruit_ILI9341 &tft) {
    for (int i = 0; i <= 3; i++) {
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

class Toggle {
public:
    Toggle(int x, int y, int textColor);
    Toggle();
    void drawToggle(Adafruit_ILI9341 &tft);
    void handleTouch(int x, int y, Adafruit_ILI9341 &tft);
    void updateDisplay(const char c[3][16], Adafruit_ILI9341 &tft);
    bool pressed;
    bool state;
private:
    int FRAME_X; //210
    int FRAME_Y; //180
    int FRAME_W; //100
    int FRAME_H; //50
    int textColor;
    int REDBUTTON_X; //FRAME_X
    int REDBUTTON_Y; //FRAME_Y
    int REDBUTTON_W; //(FRAME_W/2)
    int REDBUTTON_H; //FRAME_H
    int GREENBUTTON_X;// (REDBUTTON_X + REDBUTTON_W)
    int GREENBUTTON_Y;// FRAME_Y
    int GREENBUTTON_W; //(FRAME_W/2)
    int GREENBUTTON_H; //FRAME_H
};

Toggle::Toggle(int x, int y, int textColor){
  this->FRAME_X = x;
  this->FRAME_Y = y;
  this->FRAME_W = ToggleW;
  this->FRAME_H = ToggleH;
  this->textColor = textColor;
  this->REDBUTTON_X = x;
  this->REDBUTTON_Y = y;
  this->REDBUTTON_W = (ToggleW/2);
  this->REDBUTTON_H = ToggleH;
  this->GREENBUTTON_X = (x + (ToggleW/2));
  this->GREENBUTTON_Y = y;
  this->GREENBUTTON_W = (ToggleW/2);
  this->GREENBUTTON_H = ToggleH;
  this->state = false;
}

Toggle::Toggle(){
  this->FRAME_X = 100;
  this->FRAME_Y = 200;
  this->FRAME_W = ToggleW;
  this->FRAME_H = ToggleH;
  this->textColor = ILI9341_BLACK;
  this->REDBUTTON_X = 100;
  this->REDBUTTON_Y = 200;
  this->REDBUTTON_W = (ToggleW/2);
  this->REDBUTTON_H = ToggleH;
  this->GREENBUTTON_X = (100 + (ToggleW/2));
  this->GREENBUTTON_Y = 200;
  this->GREENBUTTON_W = (ToggleW/2);
  this->GREENBUTTON_H = ToggleH;
  this->state = false;
}

void Toggle::drawToggle(Adafruit_ILI9341 &tft){
  if(this->state){
    tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, ILI9341_RED);
    tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, ILI9341_BLUE);
    tft.drawRect(FRAME_X, FRAME_Y, FRAME_W, FRAME_H, ILI9341_BLACK);
    tft.setCursor(GREENBUTTON_X + 6 , GREENBUTTON_Y + (GREENBUTTON_H/2));
    tft.setTextColor(this->textColor);
    tft.setTextSize(2);
    tft.println("ON");
    this->state = false;
  }else{
    tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, ILI9341_GREEN);
    tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, ILI9341_BLUE);
    tft.drawRect(FRAME_X, FRAME_Y, FRAME_W, FRAME_H, ILI9341_BLACK);
    tft.setCursor(REDBUTTON_X + 6 , REDBUTTON_Y + (REDBUTTON_H/2));
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.println("OFF");
    this->state = true;
  }
}

void Toggle::handleTouch(int x, int y, Adafruit_ILI9341 &tft){
  y = (sy - y);
  if(this->state){
    if((x > REDBUTTON_X) && (x < (REDBUTTON_X + REDBUTTON_W))) {
        if ((y > REDBUTTON_Y) && (y <= (REDBUTTON_Y + REDBUTTON_H))) {
          tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, ILI9341_RED);
          tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, ILI9341_BLUE);
          tft.drawRect(FRAME_X, FRAME_Y, FRAME_W, FRAME_H, ILI9341_BLACK);
          tft.setCursor(GREENBUTTON_X + 6 , GREENBUTTON_Y + (GREENBUTTON_H/2));
          tft.setTextColor(ILI9341_WHITE);
          tft.setTextSize(2);
          tft.println("ON");
          this->state = false;
        }
      }
  }else{
    if((x > GREENBUTTON_X) && (x < (GREENBUTTON_X + GREENBUTTON_W))) {
        if ((y > GREENBUTTON_Y) && (y <= (GREENBUTTON_Y + GREENBUTTON_H))) {
          tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, ILI9341_GREEN);
          tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, ILI9341_BLUE);
          tft.drawRect(FRAME_X, FRAME_Y, FRAME_W, FRAME_H, ILI9341_BLACK);
          tft.setCursor(REDBUTTON_X + 6 , REDBUTTON_Y + (REDBUTTON_H/2));
          tft.setTextColor(ILI9341_WHITE);
          tft.setTextSize(2);
          tft.println("OFF");
          this->state = true;
        }
    }
  }
}


class increment{
  public:
    increment(int x, int y, int color);
    increment();
    void drawIncrement(Adafruit_ILI9341 &tft);
    void handleTouch(int x, int y, Adafruit_ILI9341 &tft);
    int value = 0;
    bool plusPressed = false;
    bool minusPressed = false;
  private:
    int FRAME_X; //210
    int FRAME_Y; //180
    int FRAME_W; //100
    int FRAME_H; //50
    int textColor;
    int PLUS_X; //FRAME_X
    int PLUS_Y; //FRAME_Y
    int PLUS_W; //(FRAME_W/2)
    int PLUS_H; //FRAME_H
    int MINUS_X;// (REDBUTTON_X + REDBUTTON_W)
    int MINUS_Y;// FRAME_Y
    int MINUS_W; //(FRAME_W/2)
    int MINUS_H; //FRAME_H
};

increment::increment(int x, int y, int color){
    this->FRAME_X = x;  //210
    this->FRAME_Y = y; //180
    this->FRAME_W = 50; //100
    this->FRAME_H = 100; //50
    this->textColor = color;
    this->PLUS_X = FRAME_X; //FRAME_X
    this->PLUS_Y = FRAME_Y; //FRAME_Y
    this->PLUS_W = (FRAME_W); //(FRAME_W/2)
    this->PLUS_H = FRAME_H/2;
    this->MINUS_X = PLUS_X;
    this->MINUS_Y =  FRAME_Y + PLUS_H;
    this->MINUS_W = (FRAME_W);
    this->MINUS_H = FRAME_H/2;
}


increment::increment(){
    this->FRAME_X = 20;  //210
    this->FRAME_Y = 180; //180
    this->FRAME_W = 50; //100
    this->FRAME_H = 100; //50
    this->textColor = ILI9341_RED;
    this->PLUS_X = FRAME_X; //FRAME_X
    this->PLUS_Y = FRAME_Y; //FRAME_Y
    this->PLUS_W = (FRAME_W); //(FRAME_W/2)
    this->PLUS_H = FRAME_H/2;
    this->MINUS_X = PLUS_X;
    this->MINUS_Y =  FRAME_Y + PLUS_H;
    this->MINUS_W = (FRAME_W);
    this->MINUS_H = FRAME_H/2;
}


void increment::drawIncrement(Adafruit_ILI9341 &tft){
  
    tft.fillRect(PLUS_X, PLUS_Y, PLUS_W, PLUS_H, ILI9341_RED);
    tft.drawRect(PLUS_X, PLUS_Y, PLUS_W, PLUS_H, ILI9341_BLACK);
    tft.fillRect(MINUS_X, MINUS_Y, MINUS_W, MINUS_H, ILI9341_BLUE);
    tft.drawRect(FRAME_X, FRAME_Y, FRAME_W, FRAME_H, ILI9341_BLACK);
    tft.setCursor(PLUS_X + 5 , PLUS_Y - 2);
    tft.setTextColor(this->textColor);
    tft.setTextSize(8);
    tft.println("+");
    tft.setCursor(PLUS_X + 5 , MINUS_Y - 2);
    tft.println("-");
}


void increment::handleTouch(int x, int y, Adafruit_ILI9341 &tft){
    y = (sy-y);
    if((x > PLUS_X) && (x < (PLUS_X + PLUS_W))){
      if((y > PLUS_Y) && (y < (PLUS_Y + PLUS_H))){
        value++;
        tft.drawRect(PLUS_X, PLUS_Y, PLUS_W, PLUS_H, ILI9341_GREEN);
        plusPressed = true;
      }
    }
    if(plusPressed){
      delay(20);
      tft.drawRect(PLUS_X, PLUS_Y, PLUS_W, PLUS_H, ILI9341_BLACK);
      plusPressed = false;
    }

    
} 

//  class slider{
//   public: 
//     slider()
//  };
  //   if(p.x > 75 && p.x < 125 && p.y > 20 && p.y < 300){
  //     tft.fillCircle(ymap+20,280,27,ILI9341_CYAN);
  //     slide = p.y;
  //     if(slide > prevSlide){
  //       tft.fillRect(prevSlide,125,slide-prevSlide,30,ILI9341_RED);
  //       }
  //     else{
  //       tft.fillRect(slide,125,prevSlide-slide,30,ILI9341_CYAN);
  //     }
  //     prevSlide = slide;
  //     ymap = slide;
  //     char s[5] = {'\0'};
  //     strcpy(con[2],"Slider: ");
  //     itoa(ymap,s,10);
  //     strcat(con[2], s);
  //     //display(con);
  // }

