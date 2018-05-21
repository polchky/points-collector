#ifndef DateManager_h
#define DateManager_h

#include <Wire.h>
#include "Adafruit_FRAM_I2C.h"


class DateManager
{
  public:
    DateManager();
    void begin(uint8_t addr);
  private:
    Adafruit_FRAM_I2C _fram;
    uint8_t day;
    uint8_t month;
    uint8_t year;
};

DateManager::DateManager(){}

void DateManager::begin(uint8_t addr)
{
  _fram = Adafruit_FRAM_I2C();
  _fram.begin(addr);
}


