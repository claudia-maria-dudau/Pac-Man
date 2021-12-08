#include <LedControl.h>
#include <EEPROM.h>
#include "system_constants.h"

class Matrix {
    // matrix
    LedControl lc = LedControl(DIN_PIN, CLOCK_PIN, LOAD_PIN, 1);

    // matrix settings
    short int intensity;

    // map of the game
    short int matrixSize;
    char matrix[MAX_MAP_SIZE][MAX_MAP_SIZE];

    // blinking objects states
    bool playerState;
    bool enemiesState;

  public:
    // matrix initialization
    Matrix() {
      // loading values from memory
      intensity = EEPROM.read(INTENSITY_ADDRESS);

      lc.shutdown(0, false);
      lc.clearDisplay(0);
      showIntensity();

      initialize(MIN_MAP_SIZE);
    }

    // initializing the matrix
    void initialize(short int size) {
      matrixSize = size;

      // reinitializing the matrix (0 on al positions)
      clear();

      //resetting blinking objects states
      playerState = false;
      enemiesState = false;
    }

    // clearing the matrix
    void clear() {
      for (short int i = 0; i < matrixSize; i++) {
        for (short int j = 0; j < matrixSize; j++) {
          matrix[i][j] = EMPTY_SYMBOL;
          lc.setLed(0, i, j, 0);
        }
      }
    }

    // getting the symbol at a given position on the matrix
    char getSymbol(short int row, short int col) {
      return matrix[row][col];
    }


    // --- INTRO ---
    // showing animation at the start of the game
    void startAnimation() {
      short int i;
      
      for (i = 2; i <= 4; i++) {
        lc.setLed(0, 0, i, 1);
      }
      for (i = 1; i <= 5; i++) {
        if (i != 2)
          lc.setLed(0, 1, i, 1);
      }
      for (i = 0; i <= 4; i++) {
        lc.setLed(0, 2, i, 1);
      }
      for (i = 0; i <= 3; i++) {
        lc.setLed(0, 3, i, 1);
      }
      lc.setLed(0, 3, 7, 1);
      for (i = 0; i <= 4; i++) {
        lc.setLed(0, 4, i, 1);
      }
      for (i = 1; i <= 5; i++) {
        lc.setLed(0, 5, i, 1);
      }
      for (i = 2; i <= 4; i++) {
        lc.setLed(0, 6, i, 1);
      }
    }


    // --- GAME ---
    // setting the position of an object
    void setPosition(short int* position, char symbol) {
      matrix[position[0]][position[1]] = symbol;

      // showing the position on the matrix
      lc.setLed(0, position[0], position[1], 1);
    }

    // changing the positon of the player or of an enemy
    void changePosition(short int* lastPosition, short int* currentPosition, char symbol) {
      matrix[lastPosition[0]][lastPosition[1]] = EMPTY_SYMBOL;
      matrix[currentPosition[0]][currentPosition[1]] = symbol;

      // updating the matrix to show the updated position
      lc.setLed(0, lastPosition[0], lastPosition[1], 0);
      lc.setLed(0, currentPosition[0], currentPosition[1], 1);
    }

    // blinking the player's position
    void blinkPlayer(short int* position) {
      playerState = !playerState;
      lc.setLed(0, position[0], position[1], playerState);
    }

    // blinking the enemy's position
    void blinkEnemy(short int* position, bool first) {
      if (first) {
        // changing the enemies' state only once for all enemies
        enemiesState = !enemiesState;
      }

      lc.setLed(0, position[0], position[1], enemiesState);
    }

    // removing a food item from the map
    void removeFoodItem(short int* position) {
      matrix[position[0]][position[1]] = EMPTY_SYMBOL;

      // not showing the position anymore on the matrix
      lc.setLed(0, position[0], position[1], 0);
    }


    // --- INTENSITY SETTINGS ---
    // getting the intensity value
    short int getIntensity() {
      return intensity;
    }

    // setting intensity value
    void showIntensity() {
      lc.setIntensity(0, intensity);
    }

    // changing intensity value
    void setIntensity(short int value) {
      intensity = value;
      EEPROM.update(INTENSITY_ADDRESS, value);
      showIntensity();
    }
};
