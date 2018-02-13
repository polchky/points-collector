#ifndef DisplayManager_h
#define DisplayManager_h

#define THUMB   0
#define INDEX   1
#define MIDDLE  2
#define RING    3

#define LONG_PRESS_MS 500



class InputManager{
  public:
    InputManager();
    void begin(uint8_t thumb, uint8_t index, uint8_t middle, uint8_t ring);
    boolean clicked(uint8_t button);
    boolean longClicked(uint8_t button);
    void update();
    void resetButtons();
  private:
    uint8_t findButton(uint8_t pin);
    uint8_t _pins[4];
    bool _buttonsPressed[4];
    unsigned long _buttonsPressedStart[4];
    bool _buttonsClicked[4];
    bool _buttonsLongClicked[4];
    unsigned long _timeIdle;
};

InputManager::InputManager()
{
  
}

void InputManager::begin(uint8_t thumb, uint8_t index, uint8_t middle, uint8_t ring)
{
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
  boolean pressed;
  for(uint8_t i=0; i<4; i++){
    _buttonsClicked[i] = false;
    _buttonsLongClicked[i] = false;
    pressed = digitalRead(_pins[i]);
    if(pressed){
      _timeIdle = millis();
      if(!_buttonsPressed[i]){
        _buttonsPressedStart[i] = millis();
      }
    }else{
      if(_buttonsPressed[i]){
        if(millis() - _buttonsPressedStart[i] < LONG_PRESS_MS){
          _buttonsClicked[i] = true;
        }else{
          _buttonsLongClicked[i] = true;
        }
      }
    }
    _buttonsPressed[i] = pressed;
  }
}

 

void InputManager::resetButtons()
{
  for(uint8_t i=0; i<4; i++){
    _buttonsClicked[i] = false;
    _buttonsLongClicked[i] = false;
  }
}

uint8_t InputManager::findButton(uint8_t pin)
{
  for(uint8_t i=0; i<4; i++){
    if(_pins[i] == pin) return i;
  }
}



#endif
