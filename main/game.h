#include "display.h"
#include "matrix.h"
#include "joystick.h"

using namespace std;

class Game {
    Display display = Display();
    Matrix matrix = Matrix();
    Joystick joystick = Joystick();

    // in game variables
    int lives;
    int currentScore;
    int level;
    int currentPosition[2];

    // enemies
    int noEnemies;
    int enemies[MAX_ENEMIES][2];

    // food items
    int noFoodItems;
    int foodItems[MAX_FOOD_ITEMS][2];

    // map size
    int mapSize;

    // level settings
    int startLevelValue = 1;
    const int minLevelValue = 1;
    const int maxLevelValue = MAX_LEVEL;
    const int levelStep = 1;

    // highscore board
    String names[HIGHSCORE_TOP] = { "", "", "", "", "" };
    int scores[HIGHSCORE_TOP] = { 0, 0, 0, 0, 0 };
    int namesAddresses[HIGHSCORE_TOP * (MAX_NAME_LENGTH + 1)];
    int scoresAddresses[HIGHSCORE_TOP * 2];
    const int startNamesAddress = 3;
    const int startScoresAddress = startNamesAddress + HIGHSCORE_TOP * 2;

    // about section
    const int aboutInterval = 500;
    unsigned long lastChangedAbout;
    
public:
  Game() {
    // memory addresses for highscore top
    // scores
    for (int i = 0; i < HIGHSCORE_TOP * 2; i++)
      scoresAddresses[i] = startNamesAddress + i; 

    // names
    for (int i = 0; i < HIGHSCORE_TOP * (MAX_NAME_LENGTH + 1); i++)
      namesAddresses[i] = startScoresAddress + i;

    // reading highscore top from memory
    // scores
    for (int i = 0; i < HIGHSCORE_TOP; i++) {
      // int value needs 2 bytes to be stored
      // => the value is stored into 2 addresses on the EEPROM
      byte byte1 = EEPROM.read(scoresAddresses[2 * i]);
      byte byte2 = EEPROM.read(scoresAddresses[2 * i + 1]);
      
      // reconstrucitng the int value from the 2 bytes:
      // the first byte needs to be shifted left by 8 then added with the second byte
      scores[i] = (byte1 << 8) + byte2;
    }

    // names
    for (int i = 0; i < HIGHSCORE_TOP; i ++) {
      // string value has each of its characters stored at consecutive addresses on the EEPROM
      // the length of the string is also stored in the address right before its characters
      // in order to know how many characters need to be read to reconstruct the string
      int length =  EEPROM.read(namesAddresses[i * (MAX_NAME_LENGTH + 1)]);

      // reconstructing the string value:
      // concatenating all the characters  
      for (int j = 1; j < length + 1; j++)
        names[i] += (char)EEPROM.read(namesAddresses[i * (MAX_NAME_LENGTH + 1) + j]);
      
    }
  }


  // --- SYSTEM STATE ---
  // getting the curent state of the system
  int getSystemState() {
    return joystick.getSystemState();
  }

  // setting the curent state of the system
  int setSystemState(int state) {
    joystick.setSystemState(state);
  }

  
  // --- INTRO ---
  // showing game intro
  void intro() {
    joystick.setSystemState(INTRO_STATE);
    matrix.startAnimation();
//    display.startText();
    joystick.setSystemState(PRINCIPAL_MENU_STATE);
  }


  // --- PRINCIPAL MENU ---
  // showing the principal menu of the game
  void showPrincipalMenu() {
    display.clear();
    display.showPrincipalMenu();
  }

  // navigating through the pricipal menu
  void navigatePrincipalMenu() {
    int cursorPosition = display.getPrincipalMenuCursor();
    int newCursorPosition = joystick.updateMenuPosition(cursorPosition, display.getPrincipalMenuItems());

    // cursor moved => update display
    if(cursorPosition != newCursorPosition) {
      display.setPrincipalMenuCursor(newCursorPosition);
      showPrincipalMenu();
    }
  }


  // --- IN GAME DISPLAY ---
  // showing the in game display
  void showGameDisplay() {
    display.clear();
    display.showGameDisplay(level, lives, currentScore); 
  }

  // game initialization based on level
  void initializeLevel() {
    // the level indicates the number of enemies, food items and size of the map
    noEnemies = (int)((level - 1) / LEVEL_MAP_STEP) * ENEMIES_STEP + 1;
    noFoodItems = level * FOOD_ITEMS_STEP;
    mapSize = MIN_MAP_SIZE + (int)((level - 1) / LEVEL_MAP_STEP) * MAP_STEP;
    
    // TO DO: generate positions for enemies and food items
    
//    matrix.initializeMap(mapSize);
//    matrix.showCurrentPosition(currentPosition);
//    matrix.showEnemies(enemies, noEnemies);
//    matrix.showFoodItems(foodItems, noFoodItems);
  }

  void playGame() {
    // TO DO
  }


  // --- HIGHSCORE BOARD ---
  // updating highscore board
  void updateHighscoreBoard(String name, int score) {
    // inserting the value into the top
    for (int i = 0; i < HIGHSCORE_TOP; i++) {
      if (score > scores[i]) {
        // moving the bottom of the top with one position lower
        for (int j = HIGHSCORE_TOP - 1; j > i; j--) {
          scores[j] = scores[j - 1];
          names[j] = names[j - 1];
        }

        // inserting value
        scores[i] = score;
        names[i] = name;

        break;
      }
    }

    // updating values into the memory
    // scores
    for (int i = 0; i < HIGHSCORE_TOP; i++) {
      // int value needs to be stored into 2 addresses on the EEPROM
      // => the value is split into 2 bytes:
      // the first byte needs to be shifted right by 8 (in order to only get the firts 8 bits of the number)
      EEPROM.update(scoresAddresses[2 * i], (scores[i] >> 8));

      // the second byte needs to go through an AND operation with 0xFF (in order to only get the last 8 bits of the number)
      EEPROM.update(scoresAddresses[2 * i + 1], (scores[i] & 0xFF));
    }


    // names
    for (int i = 0; i < HIGHSCORE_TOP; i ++) {
      // string value needs to have each of its characters stored at consecutive addresses on the EEPROM
      for (int j = 1; j < names[i].length() + 1; j++)
        EEPROM.update(namesAddresses[i * (MAX_NAME_LENGTH + 1) + j], names[i][j - 1]);

      // the length of the string is also stored in the address right before its characters
      EEPROM.update(namesAddresses[i * (MAX_NAME_LENGTH + 1)], names[i].length());
    }
    
  }
  
  // showing the highscore board of the game
  void showHighscoreBoard() {
    display.clear();
    display.showHighscoreBoard(names, scores);
  }

  // navigating through the highscore board
  void navigateHighscoreBoard() {
    int cursorPosition = display.getHighscoreBoardCursor();
    int newCursorPosition = joystick.updateMenuPosition(cursorPosition, HIGHSCORE_TOP + 1);

    // cursor moved => update display
    if(cursorPosition != newCursorPosition) {
      display.setHighscoreBoardCursor(newCursorPosition);
      showHighscoreBoard();
    }
  }


  // --- SETTINGS MENU ---
  // showing the settings menu of the game
  void showSettingsMenu() {
    display.clear();
    display.showSettingsMenu();
  }

  // navigating through the settings menu
  void navigateSettingsMenu() {
    int cursorPosition = display.getSettingsMenuCursor();
    int newCursorPosition = joystick.updateMenuPosition(cursorPosition, display.getSettingsMenuItems());

    // cursor moved => update display
    if(cursorPosition != newCursorPosition) {
      display.setSettingsMenuCursor(newCursorPosition);
      showSettingsMenu();
    }
  }


  // --- START LEVEL SETTINGS SECTION ---
  // showing the start level settings section
  void showStartLevelSettings() {
    display.clear();
    display.showStartLevelSettings(startLevelValue);
  }

  // navigating through the levels values 
  void navigateStartLevelValues() {
    int newStartLevelValue = joystick.updateValue(startLevelValue, minLevelValue, maxLevelValue, levelStep);

    // value changed => update display
    if(startLevelValue != newStartLevelValue) {
      startLevelValue = newStartLevelValue;
      showStartLevelSettings();
    }
  }

  // --- CONTRAST SETTINGS SECTION ---
  // showing the contrast settings section
  void showContrastSettings() {
    display.clear();
    display.showContrastSettings();
  }

  // navigating through the contrast values 
  void navigateContrastValues() {
    int contrastValue = display.getContrast();
    int newContrastValue = joystick.updateValue(contrastValue, display.getMinSetting(), display.getMaxSetting(), display.getContrastStep());

    // value changed => update display
    if(contrastValue != newContrastValue) {
      display.setContrast(newContrastValue);
      showContrastSettings();
    }
  }


  // --- BRIGHTNESS SETTINGS SECTION ---
  // showing the contrast settings section
  void showBrightnessSettings() {
    display.clear();
    display.showBrightnessSettings();
  }

  // navigating through the contrast values 
  void navigateBrightnessValues() {
    int brightnessValue = display.getBrightness();
    int newBrightnessValue = joystick.updateValue(brightnessValue, display.getMinSetting(), display.getMaxSetting(), display.getBrightnessStep());

    // value changed => update display
    if(brightnessValue != newBrightnessValue) {
      display.setBrightness(newBrightnessValue);
      showBrightnessSettings();
    }
  }


  // --- INTENSITY SETTINGS SECTION ---
  // showing the contrast settings section
  void showIntensitySettings() {
    display.clear();
    display.showIntensitySettings(matrix.getIntensity());
  }

  // navigating through the contrast values 
  void navigateIntensityValues() {
    int intensityValue = matrix.getIntensity();
    int newIntensityValue = joystick.updateValue(intensityValue, matrix.getMinIntensity(), matrix.getMaxIntensity(), matrix.getIntensityStep());

    // value changed => update display
    if(intensityValue != newIntensityValue) {
      matrix.setIntensity(newIntensityValue);
      showIntensitySettings();
    }
  }


  // --- ABOUT SECTION ---
  // showing the about section
  void showAbout() {
    display.clear();
    display.showAbout();
  }

  // navigate through the about section
  void navigateAbout() {
    int cursorPosition = display.getAboutCursor();
    int newCursorPosition = joystick.updateMenuPosition(cursorPosition, display.getAboutItems());

    // cursor moved => update display
    if(cursorPosition != newCursorPosition) {
      display.setAboutCursor(newCursorPosition);
      showAbout();
    }

    if (millis() - lastChangedAbout > aboutInterval) {
      scrollAbout();
      lastChangedAbout = millis();
    }
  }

  // scrolling the text in the about section
  void scrollAbout() {
    display.scrollCurrentAbout();
  }

  
  // --- TRANSITIONS FROM MENUS ---
  // changing from the principal menu to one of the options
  void changeFromPrincipalMenu() {
    int cursorPosition = display.getPrincipalMenuCursor();
    
    if (cursorPosition == START_GAME_POSITION) {
      // start game
      // resseting game variables
      lives = MAX_LIVES;
      currentScore = 0;
      level = startLevelValue;
      currentPosition[0] = 0;
      currentPosition[1] = 0;

      setSystemState(IN_GAME_STATE);
      showGameDisplay();
      initializeLevel();
    } else if (cursorPosition == HIGHSCORE_POSITION) {
      // highscore
      setSystemState(HIGHSCORE_BOARD_STATE);
      display.setHighscoreBoardCursor(0);
      showHighscoreBoard();
    } else if (cursorPosition == SETTINGS_POSITION) {
      // settings menu
      setSystemState(SETTINGS_MENU_STATE);
      display.setSettingsMenuCursor(0);
      showSettingsMenu();
    } else {
      // about section
      // resetting timer
      lastChangedAbout = millis();
      
      setSystemState(ABOUT_STATE);
      display.setAboutCursor(0);
      showAbout();
    }
  }

  // changing from the highscore board
  void changeFromHighscoreBoard() {
    int cursorPosition = display.getHighscoreBoardCursor();
    
    if (cursorPosition == HIGHSCORE_TOP) {
      // going back to the principal menu
      // only if the button was pressed from the position of the "back" option
      setSystemState(PRINCIPAL_MENU_STATE);
      showPrincipalMenu();
    } else {
      // otherwise nothing happenes and the system remains in the same state
      setSystemState(HIGHSCORE_BOARD_STATE);
    }
  }

  // changing from the settings menu to one of the options
  void changeFromSettingsMenu() {
    int cursorPosition = display.getSettingsMenuCursor();
    
    if (cursorPosition == START_LEVEL_POSITION) {
      // starting level
      setSystemState(START_LEVEL_SETTINGS_STATE);
      showStartLevelSettings();
    } else if (cursorPosition == CONTRAST_POSITION) {
      // lcd contrast
      setSystemState(CONTRAST_SETTINGS_STATE);
      showContrastSettings();
    } else if (cursorPosition == BRIGHTNESS_POSITION) {
      // lcd brightness
      setSystemState(BRIGHTNESS_SETTINGS_STATE);
      showBrightnessSettings();
    } else if (cursorPosition == INTENSITY_POSITION) {
      // matrix brightness intensity
      setSystemState(INTENSITY_SETTINGS_STATE);
      showIntensitySettings();
    } else {
      // back to principal menu
      setSystemState(PRINCIPAL_MENU_STATE);
      showPrincipalMenu();
    }
  }

  // changing from one of the settings' section back to the settings menu
  void changeFromSettingsSection() {
     setSystemState(SETTINGS_MENU_STATE);
     showSettingsMenu();
  }

  // changing from the about section
  void changeFromAbout() {
    int cursorPosition = display.getAboutCursor();
    
    if (cursorPosition == display.getAboutItems() - 1) {
      // going back to the principal menu
      // only if the button was pressed from the position of the "back" option
      setSystemState(PRINCIPAL_MENU_STATE);
      showPrincipalMenu();
    } else {
      // otherwise nothing happenes and the system remains in the same state
      setSystemState(ABOUT_STATE);
    }
  }
};
