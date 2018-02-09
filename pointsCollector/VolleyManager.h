#ifndef VolleyManager_h
#define VolleyManager_h

#include <Wire.h>
#include "Volley.h"
#include "Adafruit_FRAM_I2C.h"


class VolleyManager
{
  public:
    VolleyManager(uint8_t addr);
    void get(uint16_t index, Volley *volley);
    void add(Volley *volley);
    void replace(uint16_t index, Volley *volley);
    void remove(uint16_t index);
    void clear();
    uint16_t getSize();
  private:
  Adafruit_FRAM_I2C _fram;
    uint16_t _size;
    uint16_t readSize();
    void writeSize();
    boolean checkExists(uint16_t index);
};


VolleyManager::VolleyManager(uint8_t addr = MB85RC_DEFAULT_ADDRESS)
{
  Adafruit_FRAM_I2C _fram = Adafruit_FRAM_I2C();
  _fram.begin(addr);
  _size = readSize();
  
}


void VolleyManager::get(uint16_t index, Volley *volley)
{
  if(!checkExists(index) return;
}

void VolleyManager::add(Volley *volley)
{
  if(_size >= 32768 - 2) return;
}

void VolleyManager::replace(uint16_t index, Volley *volley)
{
  if(!checkExists(index) return;
}

void VolleyManager::remove(uint16_t index)
{
  if(!checkExists(index) return;
  
}

void VolleyManager::clear()
{
  _size = 0;
  writeSize();
}

uint16_t VolleyManager::getSize()
{
  return _size;
}

uint16_t VolleyManager::readSize()
{
  return (uint16_t)(_fram.read8(0)) << 8 | (uint16_t)(_fram.read8(1));
}

void VolleyManager::writeSize()
{
  _fram.write8(0, (uint8_t)(_size >> 8));
  _fram.write8(1, (uint8_t)(_size));
}

boolean VolleyManager::checkExists(uint16_t index)
{
  return index < _size;
}




#endif
