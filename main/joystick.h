#include "system_constants.h"

class Joystick {
  // joystick pins
  const int xPin = A0;
  const int yPin = A1;
  const int swPin = 2;

  // joystick thresholds
  const int minThreshold = 200;
  const int maxThreshold = 600;

  bool joyMoved;

  // marking the state in which the system is:
  volatile static int systemState;

  volatile static unsigned long lastDebounceTime;
  static const int debounceInterval = 200;


  // interruption function
  static void toggleButton() {
    if (millis() - lastDebounceTime > debounceInterval) {
      if (systemState == PRINCIPAL_MENU_STATE) {
        // button was pressed from within the principal menu
        setSystemState(TRANSITION_FROM_PRINCIPAL_MENU_STATE);
      } else if (systemState == HIGHSCORE_BOARD_STATE) {
        // button was pressed from within the highscore board
        setSystemState(TRANSITION_FROM_HIGHSCORE_BOARD_STATE); 
      } else if (systemState == SETTINGS_MENU_STATE) {
        // button was pressed from within the settings menu state
        setSystemState(TRANSITION_FROM_SETTINGS_MENU_STATE); 
      } else if (systemState == ABOUT_STATE) {
        // button was pressed from within the about section
        setSystemState(TRANSITION_FROM_ABOUT_STATE);
      } else if (systemState == START_LEVEL_SETTINGS_STATE) {
        // button was pressed from within editing starting level section
        setSystemState(TRANSITION_FROM_SETTINGS_SECTION_STATE);
      } else if (systemState == CONTRAST_SETTINGS_STATE) {
        // button was pressed from within editing starting level section
        setSystemState(TRANSITION_FROM_SETTINGS_SECTION_STATE);
      } else if (systemState == BRIGHTNESS_SETTINGS_STATE) {
        // button was pressed from within editing starting level section
        setSystemState(TRANSITION_FROM_SETTINGS_SECTION_STATE);
      } else if (systemState == INTENSITY_SETTINGS_STATE) {
        // button was pressed from within editing starting level section
        setSystemState(TRANSITION_FROM_SETTINGS_SECTION_STATE);
      }
    }

    lastDebounceTime = millis();
  }

public:
  Joystick() {
    // joystick pins are in INPUT mode
    pinMode(xPin, INPUT);
    pinMode(yPin, INPUT);

    // activating pull-up resistor + interruptions on the push-button pin 
    pinMode(swPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(swPin), toggleButton, FALLING);

    joyMoved = false;
  }

  // getting the curent state of the system
  static int getSystemState() {
    return systemState;
  }

  // setting the state of the system
  static void setSystemState(int state) {
    systemState = state;
  }


  // --- MENUS NAVIGATION ---
  int updateMenuPosition(int position, int noItems) {
    // reading value from joystick
    int xValue = analogRead(xPin);

    // cursor moving donwards
    if (xValue < minThreshold && !joyMoved) {
      joyMoved = true;
      
      if (position > 0){
        return --position;
      }
    }

    // cursor moving upwards
    if (xValue > maxThreshold && !joyMoved) {
      joyMoved = true;
      
      if (position < noItems - 1){
        return ++position;
      }
    }

    if (xValue > minThreshold && xValue < maxThreshold) {
      joyMoved = false;
    }

    return position;
  }

  // --- VALUE EDITING ---
  int updateValue(int value, int minVal, int maxVal, int step) {
    // reading value from joystick
    int xValue = analogRead(xPin);

    // increasing value
    if (xValue < minThreshold && !joyMoved) {
      joyMoved = true;

      int newVal = value + step;
      if (newVal < maxVal + 1){
        return newVal;
      }
    }

    // decreasing value
    if (xValue > maxThreshold && !joyMoved) {
      joyMoved = true;
      
      int newVal = value - step;
      if (newVal > minVal - 1){
        return newVal;
      }
    }

    if (xValue > minThreshold && xValue < maxThreshold) {
      joyMoved = false;
    }

    return value;
  }
};

volatile int Joystick::systemState = 0;
volatile unsigned long Joystick::lastDebounceTime = 0;
