#include "Volley.h"
#include "VolleyManager.h"
#include "Display.h"
#include "InputManager.h"
#include "Adafruit_FRAM_I2C.h"

#define THUMB   2
#define INDEX   16
#define MIDDLE  4
#define RING    5

#define STATE_SENDING   0
#define STATE_IDLE      1
#define STATE_SETTINGS  2
#define STATE_RECORDING 3
#define STATE_HISTORY   4
#define STATE_REMOVING  5

#define BRIGHTNESS_FRAM_INDEX   32767
#define DELAY_DISPLAY_SHORT     300

Display display = Display();
VolleyManager volleyManager = VolleyManager();
InputManager inputManager = InputManager();
Adafruit_FRAM_I2C fram = Adafruit_FRAM_I2C();
Volley volley;
uint8_t state = STATE_IDLE;

uint8_t brightness;
uint16_t index;
uint8_t fingers[3] = {INDEX, MIDDLE, RING};

void setup()
{
  // DEBUG
  Serial.begin(115200);
  delay(1000);


  
  display.begin(0x70, 15);
  volleyManager.begin();
  inputManager.begin(THUMB, INDEX, MIDDLE, RING);
  volley = Volley(); 
  fram.begin(MB85RC_DEFAULT_ADDRESS);

  brightness = readBrightness();
  display.setBrightness(brightness);
  delay(10);
  display.resetDisplay(true);

  
  
  /**volley.setScores(10, 9, 6);
  volleyManager.add(&volley);
  volleyManager.get(0, &volley);
  display.displayVolley(&volley, true);
  delay(3000);**/
  
  
}
 
void loop()
{
  inputManager.update();
  
  switch(state){
    case STATE_IDLE:
    doIdle();
    break;

    case STATE_SENDING:
    doSending();
    break;

    case STATE_SETTINGS:
    doSettings();
    break;

    case STATE_RECORDING:
    doRecording();
    break;

    case STATE_HISTORY:
    doHistory();
    break;

    case STATE_REMOVING:
    deRemoving();
    break;

    default:
    break;
  }

}

void doIdle(){
  if(inputManager.clicked(THUMB)){
    state = STATE_RECORDING;
    volley.setScores(10, 10, 10);
  }else if(inputManager.longClicked(THUMB)){
    state = STATE_SENDING;
  }else if(inputManager.longClicked(INDEX)){
    state = STATE_HISTORY;
    uint16_t size = volleyManager.getSize();
    if(size > 0){
      index = size - 1;
      volley = volleyManager.get(index, &volley);
    }else{
      display.displayError();
      goIdle();
    }
  }else if(inputManager.longClicked(MIDDLE)){
    state = STATE_SETTINGS;
  }else if(inputManager.longClicked(RING){
    state = STATE_REMOVING;
  }
}

void doRecording()
{
  if(inputManager.clicked(THUMB)){
    volleyManager.add(&volley);
    display.displaySuccess();
    goIdle();
  }else if (inputManager.longClicked(THUMB)){
    display.displayError();
    goIdle();
  }else{
    for(uint8_t i=0; i<3; i++){
      if(inputManager.clicked(fingers[i])){
        volley.decreaseScore(i);
      }else if(inputManager.longClicked(fingers[i])){
        volley.setScore(i, 10);
      }
    }
    display.displayVolley(&volley, true);
  }
}

void doSending()
{
  
}

void doHistory()
{
  uint16_t size = volleyManager.getSize();
  if(inputManager.longClicked(THUMB)){
    goIdle();
  }else{
    if(inputManager.clicked(INDEX)){
      if(index < size - 1){
        index++;
        volleyManager.get(index, &volley);
        display.resetDisplay(true);
        delay(DELAY_DISPLAY_SHORT);
      }else{
        display.displayError();
      }
    }else if(inputManager.clicked(RING)){
      if(index > 0){
        index--;
        volleyManager.get(index, &volley);
        display.resetDisplay(true);
        delay(DISPLAY_INTERVAL_SHORT);
      }else {
        display.displayError();
      }
    }
    display.displayVolley(&volley, true);
  }
}

void doRemoving()
{
  
}

void doSettings()
{
  
}

void goIdle()
{
  state = STATE_IDLE;
  display.resetDisplay(true);
}

uint8_t readBrightness()
{
  return fram.read8(BRIGHTNESS_FRAM_INDEX);
}

void writeBrightness(uint8_t brightness)
{
  fram.write8(BRIGHTNESS_FRAM_INDEX, brightness);
}




