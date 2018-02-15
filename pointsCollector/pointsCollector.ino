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
#define DISPLAY_SHORT_MS     500

Display display = Display();
VolleyManager volleyManager = VolleyManager();
InputManager inputManager = InputManager();
Adafruit_FRAM_I2C fram = Adafruit_FRAM_I2C();
Volley volley;

uint8_t state = STATE_IDLE;
uint8_t brightness;
unsigned long idleStart;
uint16_t volleyIndex;
uint8_t fingers[3] = {INDEX, MIDDLE, RING};

void setup()
{
  // DEBUG
  Serial.begin(115200);

  
  
  display.begin(0x70);
  volleyManager.begin();
  inputManager.begin(THUMB, INDEX, MIDDLE, RING);
  volley = Volley(); 
  fram.begin(MB85RC_DEFAULT_ADDRESS);
  
  brightness = readBrightness();
  display.setBrightness(brightness);

  enterIdle();
  
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
    doRemoving();
    break;

    default:
    break;
  }

}

void enterIdle()
{
  Serial.println("entering idle state");
  state = STATE_IDLE;
  idleStart = millis();
}

void doIdle(){
  if(inputManager.clicked(THUMB)){
    enterRecording();
  }else if(inputManager.longClicked(THUMB)){
    state = STATE_SENDING;
  }else if(inputManager.longClicked(INDEX)){
    enterHistory();
  }else if(inputManager.longClicked(MIDDLE)){
    enterSettings();    
  }else if(inputManager.longClicked(RING)){
    enterRemoving();
  }else{
    display.displayIdle(idleStart);
  }
}

void enterRecording()
{
  Serial.println("entering recording state");
  state = STATE_RECORDING;
  volley.setScores(10, 10, 10);
}

void doRecording()
{
  if(inputManager.clicked(THUMB)){
    volleyManager.add(&volley);
    display.displaySuccess();
    enterIdle();
  }else if (inputManager.longClicked(THUMB)){
    enterIdle();
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

void enterHistory()
{
  Serial.println("entering history state");
  state = STATE_HISTORY;
  uint16_t size = volleyManager.getSize();
  if(size > 0){
    volleyIndex = size - 1;
    volleyManager.get(volleyIndex, &volley);
    display.println(volleyIndex);
    delay(DISPLAY_SHORT_MS);
  }else{
    display.displayError();
    enterIdle();
  }
}

void doHistory()
{
  uint16_t size = volleyManager.getSize();
  if(inputManager.longClicked(THUMB)){
    enterIdle();
  }else{
    if(inputManager.clicked(INDEX)){
      if(volleyIndex < size - 1){
        volleyIndex++;
        volleyManager.get(volleyIndex, &volley);
        display.println(volleyIndex);
        delay(DISPLAY_SHORT_MS);
      }else{
        display.displayError();
      }
    }else if(inputManager.clicked(RING)){
      if(volleyIndex > 0){
        volleyIndex--;
        volleyManager.get(volleyIndex, &volley);
        display.println(volleyIndex);
        delay(DISPLAY_SHORT_MS);
      }else {
        display.displayError();
      }
    }
    display.displayVolley(&volley, true);
  }
}

void enterRemoving()
{
  Serial.println("entering removing state");
  state = STATE_REMOVING;
  uint16_t size = volleyManager.getSize();
  if(size > 0){
    volleyIndex = size - 1;
    volleyManager.get(volleyIndex, &volley);
    display.displayVolley(&volley, true);
  }else{
    display.displayError();
    enterIdle();
  }
}

void doRemoving()
{
  if(inputManager.clicked(THUMB)){
    volleyManager.remove(volleyIndex);
    display.displaySuccess();
    enterIdle();
  }else if(inputManager.longClicked(THUMB)){
    enterIdle();
  }
}

void enterSettings()
{
  Serial.println("entering settings state");
  state = STATE_SETTINGS;
  display.displayBrightness(brightness, true);
}

void doSettings()
{
  if(inputManager.clicked(THUMB)){
    writeBrightness(brightness);
    display.displaySuccess();
    enterIdle();
  }else if(inputManager.longClicked(THUMB)){
    brightness = readBrightness();
    display.setBrightness(brightness);
    enterIdle();
  }else if(inputManager.clicked(INDEX)){
    if(brightness < 15){
      brightness++;
      display.setBrightness(brightness);
      display.displayBrightness(brightness, true);
    }
  }else if(inputManager.clicked(RING)){
    if(brightness > 0){
      brightness--;
      display.setBrightness(brightness);
      display.displayBrightness(brightness, true);
    }
  }
}

uint8_t readBrightness()
{
  return fram.read8(BRIGHTNESS_FRAM_INDEX);
}

void writeBrightness(uint8_t brightness)
{
  fram.write8(BRIGHTNESS_FRAM_INDEX, brightness);
}




