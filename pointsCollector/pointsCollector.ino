#include "Volley.h"
#include "VolleyManager.h"

void setup()
{
  Serial.begin(115200);
  delay(1000);
  VolleyManager vm = VolleyManager();
  Volley volley = Volley();
  volley.setScores(10, 9, 6);
  vm.add(&volley);
  vm.get(0, &volley);
  Serial.println(volley.getScore(0));
  Serial.println(volley.getScore(1));
  Serial.println(volley.getScore(2));
  Serial.println(volley.getTimestamp());
}

void loop()
{
  
}





/*

#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_FRAM_I2C.h"
#include <LinkedList.h>


#define STATE_SENDING   0
#define STATE_IDLE      1
#define STATE_SETTINGS  2
#define STATE_RECORDING 3
#define STATE_HISTORY   4

#define LONG_PRESS_MS 	1000
#define SLEEP_MS      	50
#define TIMEOUT_MS    	5000

#define DISPLAY_INF_MS    0
#define DISPLAY_SHORT_MS  500
#define DISPLAY_LONG_MS   2000

#define COLUMN_INDEX 2
#define DEFAULT_DISPLAY_INDEX 3

#define DASH 1 << 6

const uint8_t buttonsPins[4] = {0,1,2,3};

bool buttonsPressed[4];
unsigned long buttonsPressedStart[4];
bool buttonsClicked[4];
bool buttonsLongClicked[4];
bool buttonPressed;


unsigned long timeIdle = millis();
unsigned long displayEnd = 0;
unsigned long now;

uint8_t state = STATE_IDLE;
uint8_t brightness;


Adafruit_7segment ledDisplay;

class Volley {
	public:
		unsigned long time;
    uint8_t scores[3];
};

LinkedList<Volley*> volleyList = LinkedList<Volley*>();

void updateDisplay(){
  if(displayEnd != 0 && displayEnd < now){
    ledDisplay.clear();
  }
}

/**
* Update buttons and check for clicked events
**/

/**
void update_buttons(){
  for(uint8_t i=0; i<4; i++){
    buttonPressed = digitalRead(buttonsPins[i]);
    // Button is pressed
    if(buttonPressed){
      // Reset idle counter
      timeIdle = millis();
      // Button was just pressed
      if(!buttonsPressed[i]){
        buttonsPressedStart[i] = millis();
      }
    }
    // Button is not pressed
    else{
      // Button was just released
      if(buttonsPressed[i] && buttonsPressedStart > 0){
        if(millis() - buttonsPressedStart[i] < LONG_PRESS_MS){
          buttonsClicked[i] = true;
        }else{
          buttonsLongClicked[i] = true;
        }
        buttonsPressedStart[i] = 0;
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

void displayRaw(uint8_t index, uint8_t bits, uint8_t duration, bool clear){
  if(clear){
    ledDisplay.clear();
  }
}

void displayNumber(uint8_t index, uint8_t number, uint8_t duration, bool clear){
  if(clear){
    ledDisplay.clear();
  }
}

/**
* Displays a line of symbols to signal an error
**/
/**
void displayNope(){
  uint8_t temp[8] = ledDisplay->displaybuffer;

  ledDisplay.writeDigitRaw(0, 1 << 6);
  ledDisplay.writeDigitRaw(1, 1 << 6);
  ledDisplay.writeDigitRaw(3, 1 << 6);
  ledDisplay.writeDigitRaw(4, 1 << 6);
  ledDisplay.writeDisplay();
  delay(DISPLAY_SHORT_MS);
  now = millis();
  for(int i=0; i < 5; i++){
    ledDisplay.writeDigitRaw(i, temp[i]);
  }
  if (displayEnd > 0){
    displayEnd += DISPLAY_SHORT_MS;
  }
}

void do_send(){

}


/**
* Check for changes in state
**/
/**
void do_idle(){
  if(buttonsClicked[0]){
    state = STATE_RECORDING;
  }else if(buttonsLongClicked[0]){
    state = STATE_SENDING;
  }else if(buttonsLongClicked[1] && buttonsLongClicked[3]){
    state = STATE_HISTORY;
  }else if(buttonsLongClicked[2]){
    state = STATE_SETTINGS;
    displayNumber(DEFAULT_DISPLAY_INDEX, brightness, DISPLAY_INF_MS, true);
  }
}

/**
* Settings to change the screen brightness
**/
/**
void do_settings(){
  bool changed = false;
  if(buttonsClicked[2] || timeIdle > TIMEOUT_MS){
    state = STATE_IDLE;
    ledDisplay.clear();
    return;
  }
  if(buttonsClicked[1] && brightness < 15){
    brightness++;
    changed = true;
  }else if(buttonsClicked[3] && brightness > 0){
    brightness--;
    changed = true;
  }
  if(changed){
    // TODO: save value
    ledDisplay.setBrightness(brightness);
    displayNumber(DEFAULT_DISPLAY_INDEX, brightness, DISPLAY_INF_MS, true);
  }
}

void do_record(){

}

void do_history(){

}

void setup(){
  buttonsPressed = {false, false, false, false};
  buttonsPressedStart = {0, 0, 0, 0};
  buttonsClicked = {false, false, false, false};
  buttonsLongClicked = {false, false, false, false};
  for(uint8_t i=0; i<4; i++){
    pinMode(buttonsPins[i], INPUT);
  }

  //TODO: retrieve brightness
  brightness = 15;

  ledDisplay = Adafruit_7segment();
}

void loop() {
  now = millis();
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
*/
