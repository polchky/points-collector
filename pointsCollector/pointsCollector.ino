#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"


#define STATE_SENDING   0
#define STATE_IDLE      1
#define STATE_SETTINGS  2
#define STATE_RECORDING 3
#define STATE_HISTORY   4

#define LONG_PRESS_MS 1000
#define SLEEP_MS 50

#define DISPLAY_TIME_SHORT  500
#define DISPLAY_TIME_LONG   2000

const uint8_t buttonsPins[4] = {0,1,2,3};
bool buttonsPressed[4] = {false, false, false, false};
unsigned long buttonsPressedStart[4] = {0, 0, 0, 0};
bool buttonsClicked[4] = {false, false, false, false};
bool buttonsLongClicked[4] = {false, false, false, false};
bool buttonPressed;

uint8_t singleScores[3];
uint8_t state = STATE_IDLE;
uint8_t brightness;
Adafruit_7segment ledDisplay;




void update_buttons(){
  for(uint8_t i=0; i<4; i++){
    buttonPressed = digitalRead(buttonsPins[i]);
    // Button is pressed
    if(buttonPressed){
      // Button was just pressed
      if(!buttonsPressed[i]){
        buttonsPressedStart[i] = millis();
      }
    }
    // Button is not pressed
    else{
      // Button was just released
      if(buttonsPressed[i]){
        if(millis() - buttonsPressedStart[i] < LONG_PRESS_MS){
          buttonsClicked[i] = true;
        }else{
          buttonsLongClicked[i] = true;
        }
      }
    }
    buttonsPressed[i] = buttonsPressed;
  }
}

void clear_buttons(){
  for(uint8_t i=0; i<4; i++){
    buttonsClicked[i] = false;
    buttonsLongClicked[i] = false;
  }
}

void setRawDisplay(uint8_t index, uint8_t bits, uint8_t duration){
  
}

void do_send(){
  
}

void do_idle(){
  if(buttonsClicked[0]){
    state = STATE_RECORDING;
  }else if(buttonsLongClicked[0]){
    state = STATE_SENDING;
  }else if(buttonsLongClicked[1] && buttonsLongClicked[3]){
    state = STATE_HISTORY;
  }else if(buttonsLongClicked[2]){
    state = STATE_SETTINGS;
  }
}

void do_settings(){
  if(buttonsClicked[2]){
    state = STATE_IDLE;
    return;
  }
  if(buttonsClicked[1] && brightness < 14){
    brightness++;
  }else if(buttonsClicked[3] && brightness > 0){
    brightness--;
  }
  ledDisplay.setBrightness(brightness);
  // TODO: save value
}

void do_record(){
  
}

void do_history(){
  
}

void setup(){
  for(uint8_t i=0; i<4; i++){
    pinMode(buttonsPins[i], INPUT);
  }

  //TODO: retrieve brightness
  brightness = 15;

  ledDisplay = Adafruit_7segment();
}

void loop() {
  update_buttons();

  switch(state){
    case STATE_IDLE:
    break;

    case STATE_SENDING:
    break;

    case STATE_SETTINGS:
    break;

    case STATE_RECORDING:
    break;

    case STATE_HISTORY:
    break;
    
    default:
    break;
  }

  clear_buttons();
  ledDisplay.writeDisplay();
}








Adafruit_7segment matrix = Adafruit_7segment();

void setup() {
#ifndef __AVR_ATtiny85__
  Serial.begin(9600);
  Serial.println("7 Segment Backpack Test");
#endif
  matrix.begin(0x70);
  brightness = 0;
  matrix.writeDigitRaw(0, 0x7F);
  matrix.writeDigitRaw(1, 0x7F);
  matrix.writeDigitRaw(3, 0x7F);
  matrix.writeDigitRaw(4, 0x7F);
  matrix.writeDisplay();
}

void loop() {
  matrix.setBrightness(brightness);
  brightness = (brightness + 1) % 15;
  matrix.writeDisplay();
  delay(100);

}
