#ifndef DisplayManager_h
#define DisplayManager_h

#include "Adafruit_FRAM_I2C.h"


#define THUMB   0
#define INDEX   1
#define MIDDLE  2
#define RING    3

#define LONG_PRESS_MIN_MS 500
#define LONG_PRESS_MAX_MS 2000



class InputManager{
  public:
    InputManager();
    void begin(uint8_t thumb, uint8_t index, uint8_t middle, uint8_t ring, uint8_t addr);
    boolean clicked(uint8_t button);
    boolean longClicked(uint8_t button);
    void update();
  private:
    uint8_t findButton(uint8_t pin);
    boolean isOnlyPressed(uint8_t index);
    uint8_t _pins[4];
    bool _buttonsPressed[4];
    unsigned long _buttonsPressedStart[4];
    bool _buttonsClicked[4];
    bool _buttonsLongClicked[4];
    unsigned long _timeIdle;
    Adafruit_FRAM_I2C _fram;
};

InputManager::InputManager()
{
  
}

void InputManager::begin(uint8_t thumb, uint8_t index, uint8_t middle, uint8_t ring, uint8_t addr = MB85RC_DEFAULT_ADDRESS)
{
  _fram = Adafruit_FRAM_I2C();
  _fram.begin(addr);
  
  _pins[0] = thumb;
  _pins[1] = index;
  _pins[2] = middle;
  _pins[3] = ring;
  for(uint8_t i=0; i<4; i++){
    _buttonsPressed[i] = false;
    _buttonsPressedStart[i] = 0;
    _buttonsClicked[i] = false;
    _buttonsLongClicked[i] = false;
    pinMode(_pins[i], INPUT);
  }
  _timeIdle = 0;
}

boolean InputManager::clicked(uint8_t button)
{
  return _buttonsClicked[findButton(button)];
}

boolean InputManager::longClicked(uint8_t button)
{
  return _buttonsLongClicked[findButton(button)];
}

void InputManager::update()
{
  unsigned long ms = millis();
  boolean pressed;
  for(uint8_t i=0; i<4; i++){
    _buttonsClicked[i] = false;
    _buttonsLongClicked[i] = false;
    pressed = digitalRead(_pins[i]);
    if(pressed){
      _timeIdle = ms;
      if(!_buttonsPressed[i] && isOnlyPressed(i)){
        _buttonsPressedStart[i] = ms;
      }
    }else{
      if(_buttonsPressed[i] && _buttonsPressedStart[i] > 0){
        if(ms - _buttonsPressedStart[i] < LONG_PRESS_MIN_MS){
          _buttonsClicked[i] = true;
        }else if(ms - _buttonsPressedStart[i] < LONG_PRESS_MAX_MS){
          _buttonsLongClicked[i] = true;
        }
        _buttonsPressedStart[i] = 0;
      }
    }
    _buttonsPressed[i] = pressed;
  }
}

uint8_t InputManager::findButton(uint8_t pin)
{
  for(uint8_t i=0; i<4; i++){
    if(_pins[i] == pin) return i;
  }
}

boolean InputManager::isOnlyPressed(uint8_t index)
{
  for(uint8_t i=0; i<4; i++){
    if(i == index) continue;
    if(_buttonsPressed[i]) return false;
  }
  return true;
}

#endif
