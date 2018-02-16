#ifndef DisplayManager_h
#define DisplayManager_h

#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include "Volley.h"

#define BYTES_10            B01011101
#define BYTES_ERROR         B01000000
#define BYTES_DOT           B10000000
#define BLOCK_DELAY_MS      10
#define DEFAULT_DISPLAY_MS  500

const uint8_t DIGITS[4] = {0, 1, 3, 4};
const uint8_t PATTERN_ERROR[4] = {BYTES_ERROR, BYTES_ERROR, BYTES_ERROR, BYTES_ERROR};
const uint8_t PATTERN_SUCCESS[4] = {1, 1, 1, 1};

class DisplayManager {
  public: 
    DisplayManager();
    void begin(uint8_t addr);
    void resetDisplay(boolean update);
    void displayIdle(unsigned long idleStart);
    void println(uint16_t number);
    void displaySuccess(unsigned long ms);
    void displayError(unsigned long ms);
    void writeDisplay();
    void displayVolley(Volley *volley, boolean update);
    void setBrightness(uint8_t brightness);
    void displayBrightness(uint8_t brightness, boolean update);
  private: 
    Adafruit_7segment _ledDisplay;
};

DisplayManager::DisplayManager(){}

void DisplayManager::begin(uint8_t addr)
{
  _ledDisplay = Adafruit_7segment();
  _ledDisplay.begin(addr);
}

void DisplayManager::resetDisplay(boolean update)
{
  _ledDisplay.clear();
  if(update) _ledDisplay.writeDisplay();
}

void DisplayManager::displayIdle(unsigned long idleStart)
{
  uint8_t cycle = (uint8_t) ((millis() - idleStart) / DEFAULT_DISPLAY_MS) % 4;
  for(uint8_t i=0; i<4; i++){
    if(i == cycle){
      _ledDisplay.writeDigitRaw(DIGITS[i], BYTES_DOT);
    }else{
      _ledDisplay.writeDigitRaw(DIGITS[i], 0);
    }
  }
  _ledDisplay.writeDisplay();
}

void DisplayManager::println(uint16_t number)
{
  _ledDisplay.println(number);
  _ledDisplay.writeDisplay();
}

void DisplayManager::displaySuccess(unsigned long ms = DEFAULT_DISPLAY_MS)
{
  for(uint8_t i=0; i<4; i++){
    _ledDisplay.writeDigitRaw(DIGITS[i], PATTERN_SUCCESS[i]); 
  }
  _ledDisplay.writeDisplay();
  delay(DEFAULT_DISPLAY_MS);
  _ledDisplay.clear();
  _ledDisplay.writeDisplay();
}

void DisplayManager::displayError(unsigned long ms = DEFAULT_DISPLAY_MS)
{
  for(uint8_t i=0; i<4; i++){
    _ledDisplay.writeDigitRaw(DIGITS[i], PATTERN_ERROR[i]); 
  }
  _ledDisplay.writeDisplay();
  delay(DEFAULT_DISPLAY_MS);
  _ledDisplay.clear();
  _ledDisplay.writeDisplay();
}

void DisplayManager::writeDisplay()
{
  _ledDisplay.writeDisplay();
}

void DisplayManager::displayVolley(Volley *volley, boolean update)
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

void DisplayManager::setBrightness(uint8_t brightness)
{
  _ledDisplay.setBrightness(brightness);
}

void DisplayManager::displayBrightness(uint8_t brightness, boolean update)
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
