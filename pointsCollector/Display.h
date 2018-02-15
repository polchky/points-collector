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
    void begin(uint8_t addr, uint8_t brightness);
    void resetDisplay(boolean update);
    void displayError(boolean update);
    void writeDisplay();
    void displayVolley(Volley *volley, boolean update);
    void setBrightness(uint8_t brightness);
    void displayBrightness(uint8_t brightness, boolean update);
  private: 
    Adafruit_7segment _ledDisplay;
};

Display::Display(){}

void Display::begin(uint8_t addr, uint8_t brightness)
{
  _ledDisplay = Adafruit_7segment();
  _ledDisplay.begin(addr);
  _ledDisplay.setBrightness(brightness);
}

void Display::resetDisplay(boolean update)
{
  _ledDisplay.clear();
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
  _ledDisplay.clear();
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

void Display::setBrightness(uint8_t brightness)
{
  _ledDisplay.setBrightness(brightness);
}

void Display::displayBrightness(uint8_t brightness, boolean update)
{
  _ledDisplay.clear();
  if(brightness < 10){
    _ledDisplay.writeDigitNum(1, 0);
    _ledDisplay.writeDigitNum(3, brightness);
  }else{
    _ledDisplay.writeDigitNum(1, 1);
    _ledDisplay.writeDigitNum(3, brightness - 10);
  }
  if(update) _ledDisplay.writeDisplay();
}

#endif
