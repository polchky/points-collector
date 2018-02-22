#include <bluefruit.h>

#include "Adafruit_FRAM_I2C.h"
#include "BleManager.h"
#include "DisplayManager.h"
#include "InputManager.h"
#include "Volley.h"
#include "VolleyManager.h"

#define THUMB   2
#define INDEX   16
#define MIDDLE  4
#define RING    5

#define STATE_SENDING     0
#define STATE_IDLE        1
#define STATE_BRIGHTNESS  2
#define STATE_RECORDING   3
#define STATE_HISTORY     4
#define STATE_REMOVING    5
#define STATE_VOLTAGE     6

#define BRIGHTNESS_FRAM_INDEX   32765
#define DISPLAY_SHORT_MS        500

DisplayManager displayManager = DisplayManager();
VolleyManager volleyManager = VolleyManager();
InputManager inputManager = InputManager();
Adafruit_FRAM_I2C fram = Adafruit_FRAM_I2C();
Volley volley;

uint8_t state = STATE_IDLE;
uint8_t brightness;
unsigned long idleStart;
unsigned long misc;
boolean volleyDisplayed;
uint16_t volleyIndex;
uint8_t fingers[3] = {INDEX, MIDDLE, RING};

void setup()
{
  volleyManager.begin();
  inputManager.begin(THUMB, INDEX, MIDDLE, RING);
  volley = Volley(); 
  fram.begin(MB85RC_DEFAULT_ADDRESS);
  brightness = readBrightness();
  displayManager.begin(0x70);
  displayManager.setBrightness(brightness);

  // Battery reading settings
  analogReference(AR_INTERNAL_3_0);
  analogReadResolution(12);
  delay(1);
  readVoltage();

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

    case STATE_BRIGHTNESS:
    doBrightness();
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

    case STATE_VOLTAGE:
    doVoltage();
    break;
    
    default:
    break;
  }

}

void enterIdle()
{
  state = STATE_IDLE;
  idleStart = millis();
}

void doIdle(){
  if(inputManager.clicked(THUMB)){
    enterRecording();
  }else if(inputManager.longClicked(THUMB)){
    enterSending();
  }else if(inputManager.longClicked(INDEX)){
    enterHistory();
  }else if(inputManager.longClicked(MIDDLE)){
    enterBrightness();    
  }else if(inputManager.longClicked(RING)){
    enterRemoving();
  }else{
    displayManager.displayIdle(idleStart);
  }
}

void enterRecording()
{
  state = STATE_RECORDING;
  volley.setScores(10, 10, 10);
}

void doRecording()
{
  if(inputManager.clicked(THUMB)){
    volleyManager.add(&volley);
    displayManager.displaySuccess();
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
    displayManager.displayVolley(&volley, true);
  }
}

void enterSending()
{
  state = STATE_SENDING;
}

void doSending()
{
  
}

void enterHistory()
{
  state = STATE_HISTORY;
  misc = millis();
  volleyDisplayed = false;
  uint16_t size = volleyManager.getSize();
  if(size > 0){
    volleyIndex = size - 1;
    volleyManager.get(volleyIndex, &volley);
    displayManager.println(volleyIndex);
  }else{
    displayManager.displayError();
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
      }
      displayManager.println(volleyIndex);
      misc = millis();
      volleyDisplayed = false;

    }else if(inputManager.clicked(RING)){
      if(volleyIndex > 0){
        volleyIndex--;
        volleyManager.get(volleyIndex, &volley);
      }
      displayManager.println(volleyIndex);
      misc = millis();
      volleyDisplayed = false;

    }
  }
  if(misc + DISPLAY_SHORT_MS <= millis() && !volleyDisplayed){
    displayManager.displayVolley(&volley, true);
  }
}

void enterRemoving()
{
  state = STATE_REMOVING;
  uint16_t size = volleyManager.getSize();
  if(size > 0){
    volleyIndex = size - 1;
    volleyManager.get(volleyIndex, &volley);
    displayManager.displayVolley(&volley, true);
  }else{
    displayManager.displayError();
    enterIdle();
  }
}

void doRemoving()
{
  if(inputManager.clicked(THUMB)){
    volleyManager.remove(volleyIndex);
    displayManager.displaySuccess();
    enterIdle();
  }else if(inputManager.longClicked(THUMB)){
    enterIdle();
  }
}

void enterBrightness()
{
  state = STATE_BRIGHTNESS;
  displayManager.displayBrightness(brightness, true);
}

void doBrightness()
{
  if(inputManager.clicked(THUMB)){
    writeBrightness(brightness);
    displayManager.displaySuccess();
    enterIdle();
  }else if(inputManager.longClicked(THUMB)){
    brightness = readBrightness();
    displayManager.setBrightness(brightness);
    enterIdle();
  }else if(inputManager.clicked(MIDDLE)){
    brightness = readBrightness();
    displayManager.setBrightness(brightness);
    enterVoltage();
  }else if(inputManager.clicked(INDEX)){
    if(brightness < 15){
      brightness++;
      displayManager.setBrightness(brightness);
      displayManager.displayBrightness(brightness, true);
    }
  }else if(inputManager.clicked(RING)){
    if(brightness > 0){
      brightness--;
      displayManager.setBrightness(brightness);
      displayManager.displayBrightness(brightness, true);
    }
  }
}

void enterVoltage()
{
  state = STATE_VOLTAGE;
  displayManager.displayVoltage(readVoltage(), true);
}

void doVoltage()
{
  if(inputManager.clicked(MIDDLE)){
    enterBrightness();
  }else if(inputManager.longClicked(MIDDLE)){
    enterIdle();
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

uint8_t readVoltage()
{
  uint8_t batteryLevel;
  float mvolts = analogRead(A7) * 0.73242188F;
  if (mvolts >= 3000){
    batteryLevel = 100;
  }else if(mvolts > 2900){
    batteryLevel = 100 - ((3000 - mvolts) * 58) / 100;
  }else if(mvolts > 2740){
    batteryLevel = 42 - ((2900 - mvolts) * 24) / 160;
  }else if(mvolts > 2440){
    batteryLevel = 18 - ((2740 - mvolts) * 12) / 300;
  }else if(mvolts > 2100){
    batteryLevel = 6 - ((2440 - mvolts) * 6) / 340;
  }else{
      batteryLevel = 0;
  }
  return batteryLevel;
}





