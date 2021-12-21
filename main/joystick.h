#include "system_constants.h"

class Joystick {
    bool joyMovedX;
    bool joyMovedY;

    // marking the state in which the system is:
    volatile static int systemState;

    volatile static unsigned long lastDebounceTime;


    // interruption function
    static void toggleButton() {
      if (millis() - lastDebounceTime > DEBOUNCE_INTERVAL) {
        if (systemState == INTRO_STATE) {
          // button was pressed during the intro 
          // => skipping intro and going to the principal menu
          setSystemState(TRANSITION_FROM_INTRO_STATE);
        }
        if (systemState == PRINCIPAL_MENU_STATE) {
          // button was pressed from within the principal menu
          // => entering one of the options of the menu
          setSystemState(TRANSITION_FROM_PRINCIPAL_MENU_STATE);
        }  else if (systemState == IN_GAME_STATE) {
          // button was pressed from within the in game state
          // => pausing the game
          setSystemState(TRANSITION_TO_PAUSE_GAME_STATE);
        } else if (systemState == PAUSE_GAME_STATE) {
          // button was pressed from within the pause game menu
          // => chosing one of the options of the menu
          setSystemState(TRANSITION_FROM_PAUSE_GAME_STATE);
        } else if (systemState == LOST_GAME_STATE) {
          // button was pressed during the lost game display
          // => moving to the next display
          setSystemState(TRANSITION_FROM_END_SECTIONS_STATE);
        } else if (systemState == WON_GAME_STATE) {
          // button was pressed during the won game display
          // => moving to the next display
          setSystemState(TRANSITION_FROM_END_SECTIONS_STATE);
        } else if (systemState == STATISTICS_STATE) {
          // button was pressed during the statistics display
          // => moving to the next display
          setSystemState(TRANSITION_FROM_END_SECTIONS_STATE);
        } else if (systemState == BEAT_HIGHSCORE_STATE) {
          // button was pressed during the beat highscore display
          // => moving to the next display
          setSystemState(TRANSITION_FROM_END_SECTIONS_STATE);
        } else if (systemState == ENTER_NAME_STATE) {
          // button was pressed from within the enter name menu
          setSystemState(TRANSITION_FROM_ENTER_NAME_STATE);
        } else if (systemState == END_GAME_STATE) {
          // button was pressed from within the end game menu
          // => chosing one of the options of the menu
          setSystemState(TRANSITION_FROM_END_GAME_STATE);
        } else if (systemState == HIGHSCORE_BOARD_STATE) {
          // button was pressed from within the highscore board
          setSystemState(TRANSITION_FROM_HIGHSCORE_BOARD_STATE);
        } else if (systemState == SETTINGS_MENU_STATE) {
          // button was pressed from within the settings menu state
          // => entering one of the settings
          setSystemState(TRANSITION_FROM_SETTINGS_MENU_STATE);
        } else if (systemState == START_LEVEL_SETTINGS_STATE) {
          // button was pressed from within editing starting level section
          // => exiting setting
          setSystemState(TRANSITION_FROM_SETTINGS_SECTION_STATE);
        } else if (systemState == CONTRAST_SETTINGS_STATE) {
          // button was pressed from within editing contrast section
          // => exiting setting
          setSystemState(TRANSITION_FROM_SETTINGS_SECTION_STATE);
        } else if (systemState == BRIGHTNESS_SETTINGS_STATE) {
          // button was pressed from within editing brightness section
          // => exiting setting
          setSystemState(TRANSITION_FROM_SETTINGS_SECTION_STATE);
        } else if (systemState == INTENSITY_SETTINGS_STATE) {
          // button was pressed from within editing intensity section
          // => exiting setting
          setSystemState(TRANSITION_FROM_SETTINGS_SECTION_STATE);
        } else if (systemState == SOUND_SETTINGS_STATE) {
          // button was pressed from within editing sound section
          // => exiting setting
          setSystemState(TRANSITION_FROM_SETTINGS_SECTION_STATE);
        } else if (systemState == RESET_HIGHSCORE_SETTINGS_STATE) {
          // button was pressed from within reset highscore section
          // => exiting setting
          setSystemState(TRANSITION_FROM_RESET_HIGHSCORE_MENU_STATE);
        } else if (systemState == ABOUT_STATE) {
          // button was pressed from within the about section
          setSystemState(TRANSITION_FROM_ABOUT_STATE);
        } 
      }

      lastDebounceTime = millis();
    }

  public:
    Joystick() {
      // joystick pins are in INPUT mode
      pinMode(X_PIN, INPUT);
      pinMode(Y_PIN, INPUT);

      // activating pull-up resistor + interruptions on the push-button pin
      pinMode(SW_PIN, INPUT_PULLUP);
      attachInterrupt(digitalPinToInterrupt(SW_PIN), toggleButton, FALLING);

      joyMovedX = false;
      joyMovedY = false;
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
    // updating the position of a cursor on the X axis
    int updateMenuPositionX(int currentPosition, int noItems) {
      // reading value from joystick
      int xValue = analogRead(X_PIN);

      // cursor moving donwards
      if (xValue < MIN_THRESHOLD && !joyMovedX) {
        joyMovedX = true;

        if (currentPosition > 0) {
          return --currentPosition;
        }
      }

      // cursor moving upwards
      if (xValue > MAX_THRESHOLD && !joyMovedX) {
        joyMovedX = true;

        if (currentPosition < noItems - 1) {
          return ++currentPosition;
        }
      }

      if (xValue > MIN_THRESHOLD && xValue < MAX_THRESHOLD) {
        joyMovedX = false;
      }

      return currentPosition;
    }

    // updating the position of a cursor on the Y axis
    int updateMenuPositionY(int currentPosition, int noItems) {
      // reading value from joystick
      int yValue = analogRead(Y_PIN);

      // cursor moving right
      if (yValue < MIN_THRESHOLD && !joyMovedY) {
        joyMovedY = true;

        if (currentPosition < noItems - 1) {
          return ++currentPosition;
        }
      }

      // cursor moving left
      if (yValue > MAX_THRESHOLD && !joyMovedY) {
        joyMovedY = true;

        if (currentPosition > 0) {
          return --currentPosition;
        }
      }

      if (yValue > MIN_THRESHOLD && yValue < MAX_THRESHOLD) {
        joyMovedY = false;
      }

      return currentPosition;
    }


    // --- VALUE EDITING ---
    // updating the value of a setting
    int updateValue(int value, int minVal, int maxVal, int valueStep) {
      // reading value from joystick
      int xValue = analogRead(X_PIN);

      // increasing value
      if (xValue < MIN_THRESHOLD && !joyMovedX) {
        joyMovedX = true;

        int newVal = value + valueStep;
        if (newVal < maxVal + 1) {
          return newVal;
        }
      }

      // decreasing value
      if (xValue > MAX_THRESHOLD && !joyMovedX) {
        joyMovedX = true;

        int newVal = value - valueStep;
        if (newVal > minVal - 1) {
          return newVal;
        }
      }

      if (xValue > MIN_THRESHOLD && xValue < MAX_THRESHOLD) {
        joyMovedX = false;
      }

      return value;
    }


    // --- GAME NAVIGATION ---
    // moving the player on the rows
    int movePlayerX(int currentPositionX, int matrixSize) {
      int xValue = analogRead(X_PIN);

      // moving up
      if (xValue < MIN_THRESHOLD) {
        if (currentPositionX > 0)
          return --currentPositionX;
      }

      // moving down
      if (xValue > MAX_THRESHOLD) {
        if (currentPositionX < matrixSize - 1)
          return ++currentPositionX;
      }

      return currentPositionX;
    }

    // moving the player on the columns
    int movePlayerY(int currentPositionY, int matrixSize) {
      int yValue = analogRead(Y_PIN);

      // moving left
      if (yValue > MAX_THRESHOLD) {
        if (currentPositionY > 0)
          return --currentPositionY;
      }

      // moving right
      if (yValue < MIN_THRESHOLD) {
        if (currentPositionY < matrixSize - 1)
          return ++currentPositionY;
      }

      return currentPositionY;
    }
};

volatile int Joystick::systemState = 0;
volatile unsigned long Joystick::lastDebounceTime = 0;
