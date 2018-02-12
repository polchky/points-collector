#ifndef Display_h
#define Display_h

#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include "Volley.h"

class Display {
  public: 
    Display(uint8_t addr);
    void resetDisplay();
    void writeDisplay();
    void displayVolley(Volley *volley);
    void displayScore(uint8_t index, uint8_t score);
  private: 
    Adafruit_7segment ledDisplay;

};

Display::Display(uint8_t addr)
{
  ledDisplay = Adafruit_7segment();
  ledDisplay.begin(addr);
}

void Display::resetDisplay()
{
  for(int i=0; i<5; i++){
    ledDisplay.writeDigitRaw(i, 0);
  }
}

void Display::writeDisplay()
{
  ledDisplay.writeDisplay();
}

void Display::displayVolley(Volley *volley)
{
  for(uint8_t i=0; i<3; i++){
    uint8_t index = i + 1;
    if(index >= 2) index++;
    if(volley->getScore(i) == 10){
      ledDisplay.writeDigitRaw(index, 1 << 6);
    }else{
      ledDisplay.writeDigitNum(index, volley->getScore(i));
    }
  }
}

void Display::displayScore(uint8_t index, uint8_t score)
{
  
}



#endif
