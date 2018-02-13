#ifndef Display_h
#define Display_h

#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include "Volley.h"

#define BYTES_10    B01011101
#define BYTES_ERROR B01000000

class Display {
  public: 
    Display(uint8_t addr);
    void resetDisplay(boolean update);
    void displayError(boolean update);
    void writeDisplay();
    void displayVolley(Volley *volley, boolean update);
  private: 
    Adafruit_7segment ledDisplay;

};

Display::Display(uint8_t addr)
{
  ledDisplay = Adafruit_7segment();
  ledDisplay.begin(addr);
}

void Display::resetDisplay(boolean update)
{
  for(int i=0; i<5; i++){
    ledDisplay.writeDigitRaw(i, 0);
  }
  if(update) ledDisplay.writeDisplay();
}

void Display::displayError(boolean update)
{
  for(uint8_t i=0; i<5; i++){
    if(i == 2) continue;
    ledDisplay.writeDigitRaw(i, BYTES_ERROR);
  }
  if(update) ledDisplay.writeDisplay();
}

void Display::writeDisplay()
{
  ledDisplay.writeDisplay();
}

void Display::displayVolley(Volley *volley, boolean update)
{
  for(uint8_t i=0; i<3; i++){
    uint8_t index = i + 1;
    if(index >= 2) index++;
    if(volley->getScore(i) == 10){
      ledDisplay.writeDigitRaw(index, BYTES_10);
    }else{
      ledDisplay.writeDigitNum(index, volley->getScore(i));
    }
  }
  if(update) ledDisplay.writeDisplay();
}


#endif
