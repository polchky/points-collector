#ifndef Display_h
#define Display_h

#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include "Volley.h"

#define BYTES_10    B01011101
#define BYTES_ERROR B01000000
const uint8_t DIGITS[4] = {0, 1, 3, 4};

class Display {
  public: 
    Display();
    void begin(uint8_t addr);
    void resetDisplay(boolean update);
    void displayError(boolean update);
    void writeDisplay();
    void displayVolley(Volley *volley, boolean update);
    boolean increaseBrightness();
    boolean decreaseBrightness();
  private: 
    Adafruit_7segment _ledDisplay;
    uint8_t _brightness;
};

Display::Display()
{
  
}

void Display::begin(uint8_t addr)
{
  _ledDisplay = Adafruit_7segment();
  _ledDisplay.begin(addr);
  _brightness = 10;
}

void Display::resetDisplay(boolean update)
{
  for(int i=0; i<5; i++){
    _ledDisplay.writeDigitRaw(i, 0);
  }
  if(update) _ledDisplay.writeDisplay();
}

void Display::displayError(boolean update)
{
  for(uint8_t i=0; i<4; i++){
    _ledDisplay.writeDigitRaw(DIGITS[i], BYTES_ERROR);
  }
  if(update) _ledDisplay.writeDisplay();
}

void Display::writeDisplay()
{
  _ledDisplay.writeDisplay();
}

void Display::displayVolley(Volley *volley, boolean update)
{
  for(uint8_t i=0; i<3; i++){
    uint8_t index = DIGITS[i + 1];
    if(volley->getScore(i) == 10){
      _ledDisplay.writeDigitRaw(index, BYTES_10);
    }else{
      _ledDisplay.writeDigitNum(index, volley->getScore(i));
    }
  }
  if(update) _ledDisplay.writeDisplay();
}

boolean Display::increaseBrightness()
{
  if(_brightness < 15){
    _brightness++;
    _ledDisplay.setBrightness(_brightness);
    return true;
  }
  return false;
}

boolean Display::decreaseBrightness()
{
  if(_brightness > 0){
    _brightness--;
    _ledDisplay.setBrightness(_brightness);
    return true;
  }
  return false;
}

#endif
