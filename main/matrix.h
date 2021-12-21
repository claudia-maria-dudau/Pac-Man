#include <LedControl.h>
#include <EEPROM.h>
#include "system_constants.h"

class Matrix {
    // matrix
    LedControl lc = LedControl(DIN_PIN, CLOCK_PIN, LOAD_PIN, 1);

    // matrix settings
    int intensity;

    // blinking objects states
    bool playerState;
    bool enemiesState;

    // done level animation
    int animationStep;
    unsigned long moveAnimation;

  public:
    // matrix initialization
    Matrix() {
      // loading values from memory
      intensity = EEPROM.read(INTENSITY_ADDRESS);

      lc.shutdown(0, false);
      lc.clearDisplay(0);
      showIntensity();

      initialize();
    }

    // initializing the matrix
    void initialize() {
      // reinitializing the matrix (0 on al positions)
      clearMatrix();

      //resetting blinking objects states
      playerState = false;
      enemiesState = false;
    }

    // clearMatrixing the matrix
    void clearMatrix() {
      for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
          lc.setLed(0, i, j, 0);
        }
      }
    }


    // --- INTRO ---
    // showing animation at the start of the game
    void showPacManAnimation() {
      bool pacMan[MATRIX_SIZE][MATRIX_SIZE] = {
        { 0, 0, 1, 1, 1, 0, 0, 0 },
        { 0, 1, 0, 1, 1, 1, 0, 0 },
        { 1, 1, 1, 1, 1, 0, 0, 0 },
        { 1, 1, 1, 1, 0, 0, 0, 1 },
        { 1, 1, 1, 1, 1, 0, 0, 0 },
        { 0, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 1, 1, 1, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0 }
      };

      for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
          lc.setLed(0, i, j, pacMan[i][j]);
        }
      }
    }


    // --- GAME ---
    // setting the position of an element
    void setPosition(int positionX, int positionY) {
      lc.setLed(0, positionX, positionY, 1);
    }

    // resetting the position of an element
    void resetPosition(int positionX, int positionY) {
      lc.setLed(0, positionX, positionY, 0);
    }

    // changing the positon of an element on the matrix
    void changePosition(int lastPositionX, int lastPositionY, int currentPositionX, int currentPositionY) {
      resetPosition(lastPositionX, lastPositionY);
      setPosition(currentPositionX, currentPositionY);
    }

    // blinking the player's position
    void blinkPlayer(int positionX, int positionY) {
      playerState = !playerState;
      lc.setLed(0, positionX, positionY, playerState);
    }

    // blinking the enemy's position
    void blinkEnemy(int positionX, int positionY) {
      lc.setLed(0, positionX, positionY, enemiesState);
    }

    // changing the enemies' state only once for all enemies
    void doneBlinkEnemies() {
      enemiesState = !enemiesState;
    }

    // intializing animation variables
    void startDoneLevelAnimation() {
      moveAnimation = millis();
      animationStep = 0;
    }

    // showing done level animation
    void showDoneLevelAnimation() {
      if (millis() - moveAnimation > ANIMATION_INTERVAL) {
        clearMatrix();
        animationStep++;
        moveAnimation = millis();
      }

      for (int i = 0; i < MATRIX_SIZE - animationStep; i++) {
        int currentStep = animationStep + i;

        if (HALF_MATRIX_SIZE - currentStep > 0) {
          // point of the arrow
          for (int j = HALF_MATRIX_SIZE - currentStep; j <= HALF_MATRIX_SIZE + currentStep; j++) {
            lc.setLed(0, i, j, 1);
          }
        } else {
          // body of the arrow
          for (int j = HALF_MATRIX_SIZE - 1; j <= HALF_MATRIX_SIZE + 1; j++) {
            lc.setLed(0, i, j, 1);
          }
        }
      }
    }


    // --- INTENSITY SETTINGS ---
    // getting the intensity value
    int getIntensity() {
      return intensity;
    }

    // setting intensity value
    void showIntensity() {
      lc.setIntensity(0, intensity);
    }

    // changing intensity value
    void setIntensity(int value) {
      intensity = value;
      EEPROM.update(INTENSITY_ADDRESS, value);
      showIntensity();
    }
};
