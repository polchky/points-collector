#include <bluefruit.h>

#include "Adafruit_FRAM_I2C.h"
#include "BleManager.h"
#include "DisplayManager.h"
#include "InputManager.h"
#include "Volley.h"
#include "VolleyManager.h"
#include "StateFunctions.h"
#include <FiniteStateMachine.h>

#define THUMB   2
#define INDEX   16
#define MIDDLE  4
#define RING    5

#define BRIGHTNESS_FRAM_INDEX   32765
#define DISPLAY_SHORT_MS        500

uint8_t brightnessValue;
unsigned long idleStart;
unsigned long misc;
boolean volleyDisplayed;
uint16_t volleyIndex;
uint8_t fingers[3] = {INDEX, MIDDLE, RING};

DisplayManager displayManager = DisplayManager();
VolleyManager volleyManager = VolleyManager();
InputManager inputManager = InputManager();
Adafruit_FRAM_I2C fram = Adafruit_FRAM_I2C();
Volley volley;

State idle = State(enterIdle, doIdle, NULL);
State recording = State(enterRecording, doRecording, NULL);
State removing = State(enterRemoving, doRemoving, NULL);
State sending = State(enterSending, doSending, NULL);
State clearing = State(enterClearing, doClearing, NULL);
State date = State(enterDate, doDate, NULL);
State voltage = State(enterVoltage, doVoltage, NULL);
State brightness = State(enterBrightness, doBrightness, NULL);
State stats = State(enterStats, doStats, NULL);
State history = State(enterHistory, doHistory, NULL);

FSM stateMachine = FSM(idle);


void setup()
{
  volleyManager.begin();
  inputManager.begin(THUMB, INDEX, MIDDLE, RING);
  volley = Volley(); 
  fram.begin(MB85RC_DEFAULT_ADDRESS);
  brightnessValue = readBrightness();
  displayManager.begin(0x70);
  displayManager.setBrightness(brightnessValue);

  // Battery reading settings
  analogReference(AR_INTERNAL_3_0);
  analogReadResolution(12);
  delay(1);
  readVoltage();

  
}
 
void loop()
{
  inputManager.update();
  stateMachine.update();
}

/**********************************/
/** IDLE **/
/**********************************/
void enterIdle()
{
  idleStart = millis();
}

void doIdle(){
  // Recording
  if(inputManager.clicked(THUMB)){
    stateMachine.transitionTo(recording);
  // Deleting
  }else if(inputManager.longClicked(THUMB)){
    stateMachine.transitionTo(removing);
  // Stats
  }else if(inputManager.clicked(MIDDLE)){
    stateMachine.transitionTo(stats);
  // Date
  }else if(inputManager.longClicked(MIDDLE)){
    stateMachine.transitionTo(date);
  }
}

/**********************************/
/** RECORDING **/
/**********************************/
void enterRecording()
{
  volley.setScores(10, 10, 10);
}

void doRecording()
{
  if(inputManager.clicked(THUMB)){
    volleyManager.add(&volley);
    displayManager.displaySuccess();
    stateMachine.transitionTo(idle);
  }else if (inputManager.longClicked(THUMB)){
    stateMachine.transitionTo(idle);
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

/**********************************/
/** REMOVING **/
/**********************************/
void enterRemoving()
{
  uint16_t size = volleyManager.getSize();
  if(size > 0){
    volleyIndex = size - 1;
    volleyManager.get(volleyIndex, &volley);
    displayManager.displayVolley(&volley, true);
  }else{
    displayManager.displayError();
    stateMachine.transitionTo(idle);
  }
}

void doRemoving()
{
  if(inputManager.clicked(THUMB)){
    volleyManager.remove(volleyIndex);
    displayManager.displaySuccess();
    stateMachine.transitionTo(idle);
  }else if(inputManager.longClicked(THUMB)){
    stateMachine.transitionTo(idle);
  }
}

/**********************************/
/** SENDING **/
/**********************************/
void enterSending()
{
  
}

void doSending()
{
  
}

/**********************************/
/** CLEARING **/
/**********************************/
void enterClearing()
{
  
}

void doClearing()
{
  
}

/**********************************/
/** DATE **/
/**********************************/
void enterDate()
{
  
}

void doDate()
{
  
}


/**********************************/
/** VOLTAGE **/
/**********************************/
void enterVoltage()
{
  displayManager.displayVoltage(readVoltage(), true);
}

void doVoltage()
{
  if(inputManager.clicked(MIDDLE)){
    stateMachine.transitionTo(brightness);
  }else if(inputManager.longClicked(MIDDLE)){
    stateMachine.transitionTo(idle);
  }
}

/**********************************/
/** BRIGHTNESS **/
/**********************************/
void enterBrightness()
{
  displayManager.displayBrightness(brightnessValue, true);
}

void doBrightness()
{
  if(inputManager.clicked(THUMB)){
    writeBrightness(brightnessValue);
    displayManager.displaySuccess();
    stateMachine.transitionTo(idle);
  }else if(inputManager.longClicked(THUMB)){
    brightnessValue = readBrightness();
    displayManager.setBrightness(brightnessValue);
    stateMachine.transitionTo(idle);
  }else if(inputManager.clicked(MIDDLE)){
    brightnessValue = readBrightness();
    displayManager.setBrightness(brightnessValue);
    stateMachine.transitionTo(date);
  }else if(inputManager.clicked(INDEX)){
    if(brightnessValue < 15){
      brightnessValue++;
      displayManager.setBrightness(brightnessValue);
      displayManager.displayBrightness(brightnessValue, true);
    }
  }else if(inputManager.clicked(RING)){
    if(brightnessValue > 0){
      brightnessValue--;
      displayManager.setBrightness(brightnessValue);
      displayManager.displayBrightness(brightnessValue, true);
    }
  }
}

/**********************************/
/** STATS **/
/**********************************/
void enterStats()
{
  
}

void doStats()
{
  
}

/**********************************/
/** HISTORY **/
/**********************************/
void enterHistory()
{
  misc = millis();
  volleyDisplayed = false;
  uint16_t size = volleyManager.getSize();
  if(size > 0){
    volleyIndex = size - 1;
    volleyManager.get(volleyIndex, &volley);
    displayManager.println(volleyIndex);
  }else{
    displayManager.displayError();
    stateMachine.transitionTo(idle);
  }
}

void doHistory()
{
  uint16_t size = volleyManager.getSize();
  if(inputManager.longClicked(THUMB)){
    stateMachine.transitionTo(idle);
  }else if(inputManager.clicked(MIDDLE)){
    stateMachine.transitionTo(stats);
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
    volleyDisplayed = true;
  }
}








uint8_t readBrightness()
{
  return fram.read8(BRIGHTNESS_FRAM_INDEX);
}

void writeBrightness(uint8_t brightnessValue)
{
  fram.write8(BRIGHTNESS_FRAM_INDEX, brightnessValue);
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





