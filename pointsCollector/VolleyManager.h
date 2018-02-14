#ifndef VolleyManager_h
#define VolleyManager_h

#include <Wire.h>
#include "Volley.h"
#include "Adafruit_FRAM_I2C.h"

#define SIZE_FRAM_INDEX 32765


class VolleyManager
{
  public:
    VolleyManager();
    void begin(uint8_t addr);
    void get(uint16_t index, Volley *volley);
    void add(Volley *volley);
    void replace(uint16_t index, Volley *volley);
    void remove(uint16_t index);
    void clear();
    uint16_t getSize();
  private:
    Adafruit_FRAM_I2C _fram;
    uint16_t _size;
    unsigned long _timeOffset;
    uint16_t readSize();
    void writeSize();
    boolean checkExists(uint16_t index);
    unsigned long getTimestamp(uint16_t index);
    void write(uint16_t index, Volley *volley);
    uint8_t scoreToWriteable(uint8_t score);
    uint8_t writeableToScore(uint8_t score);
};

VolleyManager::VolleyManager()
{
  
}

void VolleyManager::begin(uint8_t addr = MB85RC_DEFAULT_ADDRESS)
{
  _fram = Adafruit_FRAM_I2C();
  _fram.begin(addr);
  _size = readSize();
  // set time offset
  if(_size > 0){
    _timeOffset = getTimestamp(_size - 1);
    Serial.print("time offset: ");Serial.println(_timeOffset);
  } else {
    _timeOffset = 0;
  }

}


void VolleyManager::get(uint16_t index, Volley *volley)
{
  if(!checkExists(index)) return;
  uint8_t value[3];
  for(uint8_t i=0; i<3; i++){
      value[i] = _fram.read8(3 * index + i);
  }
  volley->setScore(0, writeableToScore(value[0] >> 5));
  volley->setScore(1, writeableToScore(value[0] >> 2 & B111));
  volley->setScore(2, writeableToScore((value[0] & B11) << 1 | value[1] >> 7));
  volley->setTimestamp(((uint16_t)value[1] & 127) << 8 | value[2]);
}

void VolleyManager::add(Volley *volley)
{
  if(_size == 10921) return; // Max capacity reached
  volley->setTimestamp(_timeOffset);
  write(_size, volley);
  _size++;
  writeSize();
}

void VolleyManager::replace(uint16_t index, Volley *volley)
{
  if(!checkExists(index)) return;
  write(index, volley);
}

void VolleyManager::remove(uint16_t index)
{
  if(!checkExists(index)) return;
  for(uint8_t i = 3 * index; i <= _size * 3 - 4; i++){
    _fram.write8(i, _fram.read8(i + 3));
  }
  _size--;
  writeSize();
}

void VolleyManager::clear()
{
  _size = 0;
  writeSize();
}

void VolleyManager::write(uint16_t index, Volley *volley)
{
  if(volley->getTimestamp() == 0) return;
  uint8_t value;
  value = scoreToWriteable(volley->getScore(0)) << 5;
  value |= scoreToWriteable(volley->getScore(1)) << 2;
  value |= scoreToWriteable(volley->getScore(2)) >> 1;
  _fram.write8(3 * index, value);
  value = (uint8_t)(volley->getTimestamp() >> 8);
  // ensure the timestamp is not erasing the score value
  value &= 255;
  value |= volley->getScore(3) << 7;
  _fram.write8(3 * index + 1, value);
  value = (uint8_t)(volley->getTimestamp());
  _fram.write8(3 * index + 2, value);
}

uint16_t VolleyManager::getSize()
{
  return _size;
}

uint16_t VolleyManager::readSize()
{
  return (uint16_t)(_fram.read8(SIZE_FRAM_INDEX)) << 8 | (uint16_t)(_fram.read8(SIZE_FRAM_INDEX + 1));
}

void VolleyManager::writeSize()
{
  _fram.write8(SIZE_FRAM_INDEX, (uint8_t)(_size >> 8));
  _fram.write8(SIZE_FRAM_INDEX + 1, (uint8_t)(_size));
}

boolean VolleyManager::checkExists(uint16_t index)
{
  return index < _size;
}

unsigned long VolleyManager::getTimestamp(uint16_t index)
{
  if(!checkExists(index)) return 0;
  unsigned long value;
  value = ((unsigned long)_fram.read8(3 * index + 1) & B1111111) << 8;
  value |= (unsigned long)_fram.read8(3 * index + 2);
  return value;
}

uint8_t VolleyManager::scoreToWriteable(uint8_t score)
{
  if(score >= 6){
    score -= 5;
  }
  return score;
}

uint8_t VolleyManager::writeableToScore(uint8_t score)
{
  if(score > 0){
    score += 5;
  }
  return score;
}

#endif
