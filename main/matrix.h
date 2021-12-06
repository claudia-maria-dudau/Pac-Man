#include <LedControl.h>
#include <EEPROM.h>
#include "system_constants.h"

class Matrix {
  // shift register pins
  const int dinPin = 12;
  const int clockPin = 11;
  const int loadPin = 10;

  // matrix 
  LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);

  // matrix settings
  int intensity;
  const int intensityAddress = 2;
  const int intensityStep = 1;
  const int minIntensity = 0;
  const int maxIntensity = 15;
  
  int matrixSize;
  bool matrixChanged;

  // map of the game:
  // 1 - pac man
  // 2 - food
  // 3 - ghost
  bool matrix[MAX_MAP_SIZE][MAX_MAP_SIZE] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}  
  };

public:
  // matrix initialization
  Matrix() {
    // loading values from memory
    intensity = EEPROM.read(intensityAddress);
    
    lc.shutdown(0, false); 
    lc.clearDisplay(0);
    showIntensity();

    matrixChanged = false;
    matrixSize = MIN_MAP_SIZE;
  }


  // displaying the matrix
  void show() {
   for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrix[row][col]);
      }
    }
  }

  void clear() {
   for (int i = 0; i < matrixSize; i++) {
    for (int j = 0; j < matrixSize; j++) {
      matrix[i][j] = 0;
      }
    }
  }


  // --- INTRO ---
  // showing animation at the start of the game
  void startAnimation() {
    int i;

    for (i = 2; i <= 4; i++) {
      matrix[0][i] = 1;
    }
    for (i = 1; i <= 5; i++) {
      if (i != 2)
        matrix[1][i] = 1;
    }
    for (i = 0; i<= 4; i++) {
      matrix[2][i] = 1;
    }
    for (i = 0; i<= 3; i++) {
      matrix[3][i] = 1;
    }
    matrix[3][7] = 1;
    for (i = 0; i<= 4; i++) {
      matrix[4][i] = 1;
    }
    for (i = 1; i <= 5; i++) {
        matrix[5][i] = 1;
    }
    for (i = 2; i <= 4; i++) {
      matrix[6][i] = 1;
    }
    
    show();
  }


  // --- INTENSITY SETTINGS ---
  // getting the intensity value
  int getIntensity(){
    return intensity;
  }
  
  // setting intensity value
  void showIntensity() {
    lc.setIntensity(0, intensity); 
  }

  // changing intensity value
  void setIntensity(int value) {
    intensity = value;
    EEPROM.update(intensityAddress, value);
    showIntensity();
  }

  // getting intensity step
  int getIntensityStep() {
    return intensityStep;
  }

  // getting minimum value for intensity
  int getMinIntensity() {
    return minIntensity;
  }

  // getting maximum value for intensity
  int getMaxIntensity() {
    return maxIntensity;
  }
};
