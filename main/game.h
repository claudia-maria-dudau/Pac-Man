#include "display.h"
#include "matrix.h"
#include "joystick.h"

using namespace std;

class Game {
    Display display = Display();
    Matrix matrix = Matrix();
    Joystick joystick = Joystick();

    // in game variables
    short int lives;
    int currentScore;

    // level
    short int level;
    unsigned int doneLevelTime;
    short int startLevelValue;

    // player
    short int playerPosition[2];
    unsigned int lastChangedPlayerPosition;
    unsigned int lastChangedPlayerBlinking;
    String currentName;

    // enemies
    short int noEnemies;
    short int enemies[MAX_ENEMIES][2]; // each enemy has a set of (x, y) coordinates to indicate its position
    short int enemiesSpeed;
    unsigned int lastChangedEnemiesPositions;
    unsigned int lastChangedEnemiesBlinking;
    const short int movesX[MOVES] = { -1, -1, -1, 0, 1, 1, 1, 0 };
    const short int movesY[MOVES] = { -1, 0, 1, 1, 1, 0, -1, -1 };

    // food items
    short int noFoodItems;
    short int foodItems[MAX_FOOD_ITEMS][2]; // each food item has a set of (x, y) coordinates to indicate its position

    // map size
    short int mapSize;

    // time variables
    unsigned int startStatisticsTime;
    unsigned int startBeatHighscoreTime;
    unsigned int startEnterNameTime;

    bool shownStatistics;
    short int highscoreDisplay;

    // highscore board
    String names[HIGHSCORE_TOP] = { "", "", "" };
    short int scores[HIGHSCORE_TOP] = { 0, 0, 0 };
    short int namesAddresses[HIGHSCORE_TOP * (MAX_NAME_LENGTH + 1)];
    short int scoresAddresses[HIGHSCORE_TOP * 2];

    // about section
    unsigned int lastChangedScrollAbout;

  public:
    Game() {
      // memory addresses for highscore top
      // scores
      for (short int i = 0; i < HIGHSCORE_TOP * 2; i++)
        scoresAddresses[i] = START_NAMES_ADDRESS + i;

      // names
      for (short int i = 0; i < HIGHSCORE_TOP * (MAX_NAME_LENGTH + 1); i++)
        namesAddresses[i] = START_SCORES_ADDRESS + i;

      // reading highscore top from memory
      // scores
      for (short int i = 0; i < HIGHSCORE_TOP; i++) {
        // int value needs 2 bytes to be stored
        // => the value is stored into 2 addresses on the EEPROM
        byte byte1 = EEPROM.read(scoresAddresses[2 * i]);
        byte byte2 = EEPROM.read(scoresAddresses[2 * i + 1]);

        // reconstrucitng the int value from the 2 bytes:
        // the first byte needs to be shifted left by 8 then added with the second byte
        scores[i] = (byte1 << 8) + byte2;
      }

      // names
      for (short int i = 0; i < HIGHSCORE_TOP; i ++) {
        // string value has each of its characters stored at consecutive addresses on the EEPROM
        // the length of the string is also stored in the address right before its characters
        // in order to know how many characters need to be read to reconstruct the string
        short int length =  EEPROM.read(namesAddresses[i * (MAX_NAME_LENGTH + 1)]);

        // reconstructing the string value:
        // concatenating all the characters
        for (short int j = 1; j < length + 1; j++)
          names[i] += (char)EEPROM.read(namesAddresses[i * (MAX_NAME_LENGTH + 1) + j]);

      }

      startLevelValue = MIN_LEVEL;
    }

    // resetting the matrix to the start animation
    void resetMatrix() {
      matrix.clear();
      matrix.startAnimation();
    }


    // --- SYSTEM STATE ---
    // getting the curent state of the system
    int getSystemState() {
      return joystick.getSystemState();
    }


    // --- INTRO ---
    // showing game intro
    void intro() {
      joystick.setSystemState(INTRO_STATE);
      matrix.startAnimation();
//      display.startText();
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
      short int cursorPosition = display.getPrincipalMenuCursor();
      short int newCursorPosition = joystick.updateMenuPositionX(cursorPosition, PRINCIPAL_MENU_ITEMS);

      // cursor moved => update display
      if (cursorPosition != newCursorPosition) {
        display.setPrincipalMenuCursor(newCursorPosition);
        showPrincipalMenu();
      }
    }


    // --- GAME ---
    // resetting game variables
    void resetGame() {
      lives = MAX_LIVES;
      currentScore = 0;
      level = startLevelValue;
      currentName = "";
      highscoreDisplay = BEAT_HIGHSCORE_DISPLAY - 1;
      shownStatistics = false;
    }

    // showing the in game display
    void showGameDisplay() {
      display.clear();
      display.showGameDisplay(level, lives, currentScore);
    }

    // intializing the position of the player
    void initializePlayer() {
      // generating a pair of random coordinates for
      // the intial position of the player
      playerPosition[0] = random(mapSize);
      playerPosition[1] = random(mapSize);
      matrix.setPosition(playerPosition, PLAYER_SYMBOL);
    }

    // initializing the positions of the enemies
    void initializeEnemies() {
      short int xCoord, yCoord;

      // generating random positions for enemies
      for (short int i = 0; i < noEnemies; i++) {
        bool coordOk = false;

        while (!coordOk) {
          // generating a random pair (x, y) of coordinates
          xCoord = random(mapSize);
          yCoord = random(mapSize);

          coordOk = true;

          // verifying that the generated coordinates are not too close to the
          // initial position of the player
          if ((xCoord < playerPosition[0] + DIST_FROM_INITIAL_PLAYER_POSITION && yCoord < playerPosition[1] + DIST_FROM_INITIAL_PLAYER_POSITION) &&
              (xCoord > playerPosition[0] - DIST_FROM_INITIAL_PLAYER_POSITION && yCoord > playerPosition[1] - DIST_FROM_INITIAL_PLAYER_POSITION))
            coordOk = false;

          // verifying that the position of the coordinates is empty
          if (matrix.getSymbol(xCoord, yCoord) != EMPTY_SYMBOL)
            coordOk = false;
        }

        enemies[i][0] = xCoord;
        enemies[i][1] = yCoord;

        matrix.setPosition(enemies[i], ENEMY_SYMBOL);
      }
    }

    // initialize the positions of the food items
    void initializeFoodItems() {
      short int xCoord, yCoord;

      // generating random positions for food items
      for (short int i = 0; i < noFoodItems; i++) {
        bool coordOk = false;

        while (!coordOk) {
          // generating a random pair (x, y) of coordinates
          xCoord = random(mapSize);
          yCoord = random(mapSize);

          coordOk = true;

          // verifying that the position of the generated coordinates is empty
          if (matrix.getSymbol(xCoord, yCoord) != EMPTY_SYMBOL)
            coordOk = false;
        }

        foodItems[i][0] = xCoord;
        foodItems[i][1] = yCoord;

        matrix.setPosition(foodItems[i], FOOD_ITEM_SYMBOL);
      }
    }

    // game initialization based on level
    void initializeLevel() {
      showGameDisplay();

      // initializing the seed for the random numbers
      randomSeed(millis());

      // the level indicates the number of enemies, food items and size of the map
      noEnemies = (level - 1) / LEVEL_MAP_STEP * ENEMIES_STEP + 1;
      enemiesSpeed = ENEMIES_MIN_SPEED - level * ENEMIES_SPEED_STEP;
      noFoodItems = level * FOOD_ITEMS_STEP;
      mapSize = MIN_MAP_SIZE + (level - 1) / LEVEL_MAP_STEP * MAP_STEP;

      matrix.initialize(mapSize);

      initializePlayer();
      initializeEnemies();
      initializeFoodItems();

      // resetting timers
      lastChangedPlayerPosition = millis();
      lastChangedPlayerBlinking = millis();
      lastChangedEnemiesPositions = millis();
      lastChangedEnemiesBlinking = millis();
    }

    // decreasing the number of lives of the player
    void decreaseLife() {
      if ( lives > 1) {
        // player loses a life
        lives--;

        // decresing the score
        updateScore(-1 * LIFE_SCORE);

        // player lives changed => update display
        showGameDisplay();
      } else {
        // the player has no more lives => loses game
        joystick.setSystemState(LOST_GAME_STATE);
        matrix.clear();
      }
    }

    // updating the score of the player with a given value
    void updateScore(int value) {
      currentScore += value;

      // player score changed => update display
      showGameDisplay();
    }

    // removing food item from the map
    void removeFoodItem() {
      for (short int i = 0; i < noFoodItems; i++) {
        if (foodItems[i][0] == playerPosition[0] && foodItems[i][1] == playerPosition[1]) {
          // erasing the respective food item form the map
          matrix.removeFoodItem(foodItems[i]);

          // erasing the respective food item from the list of food items
          for (short int j = noFoodItems - 1; j > i; j--) {
            foodItems[j - 1][0] = foodItems[j][0];
            foodItems[j - 1][1] = foodItems[j][1];
          }

          break;
        }
      }

      noFoodItems--;
    }

    // blinking the player's position at a given interval
    void blinkPlayer() {
      if (millis() - lastChangedPlayerBlinking > PLAYER_BLINKING_INTERVAL) {
        matrix.blinkPlayer(playerPosition);

        lastChangedPlayerBlinking = millis();
      }
    }

    // blinking the enemies' position at a given intervla
    void blinkEnemies() {
      if (millis() - lastChangedEnemiesBlinking > ENEMIES_BLINKING_INTERVAL) {
        for (short int i = 0; i < noEnemies; i++) {
          matrix.blinkEnemy(enemies[i], i == 0);
        }

        lastChangedEnemiesBlinking = millis();
      }
    }

    // moving enemies at a given interval
    void moveEnemies() {
      char currentSymbol;

      if (millis() - lastChangedEnemiesPositions > enemiesSpeed) {
        short int move;
        short int next[2];

        // generating a random move for each enemy
        for (short int i = 0; i < noEnemies; i++) {
          bool moveOk = false;

          while (!moveOk) {
            // generate random move
            move = random(MOVES);
            next[0] = enemies[i][0] + movesX[move];
            next[1] = enemies[i][1] + movesY[move];

            moveOk = true;

            // verifying the move is valid
            // the next position doesn't exced the bounds of the map
            if (next[0] < 0 || next[0] > mapSize - 1 || next[1] < 0 || next[1] > mapSize - 1)
              moveOk = false;

            if (moveOk) {
              currentSymbol = matrix.getSymbol(next[0], next[1]);

              // the next position is not already occupied by
              // another enemy or a food item
              if (currentSymbol != EMPTY_SYMBOL && currentSymbol != PLAYER_SYMBOL)
                moveOk = false;
            }
          }

          matrix.changePosition(enemies[i], next, ENEMY_SYMBOL);
          enemies[i][0] = next[0];
          enemies[i][1] = next[1];


          // verifying if the enemy landed on the same position as the player
          if (currentSymbol == PLAYER_SYMBOL)
            decreaseLife();
        }

        lastChangedEnemiesPositions = millis();
      }
    }

    // moving the player with the joystick
    void movePlayer() {
      char currentSymbol;

      if (millis() - lastChangedPlayerPosition > PLAYER_SPEED) {
        short int newPosition[2] = { joystick.movePlayerX(playerPosition[0], mapSize), joystick.movePlayerY(playerPosition[1], mapSize)};

        // player moved => update position
        if (newPosition[0] != playerPosition[0] || newPosition[1] != playerPosition[1]) {
          currentSymbol = matrix.getSymbol(newPosition[0], newPosition[1]);

          matrix.changePosition(playerPosition, newPosition, PLAYER_SYMBOL);
          playerPosition[0] = newPosition[0];
          playerPosition[1] = newPosition[1];

          //verifying if the player landed on the same position as one of the enemies
          if (currentSymbol == ENEMY_SYMBOL)
            decreaseLife();

          // verifying id the player landed on the position of a food item
          // => the player eats the food item
          if (currentSymbol == FOOD_ITEM_SYMBOL) {
            removeFoodItem();

            // increasing the player's score
            // (each food item values as much as the current level of the game)
            updateScore(level);
          }
        }

        lastChangedPlayerPosition = millis();
      }
    }

    // the flow of the game
    void playGame() {
      if (noFoodItems == 0) {
        // no more food items on the map => leve is done
        joystick.setSystemState(DONE_LEVEL_STATE);
        matrix.clear();

        // memorying the time when the level was finished
        doneLevelTime = millis();
      } else {
        // continue playing
        blinkPlayer();
        blinkEnemies();

        moveEnemies();
        movePlayer();
      }
    }

    // pausing the game
    void changeToPause() {
      joystick.setSystemState(PAUSE_GAME_STATE);
      display.setPauseGameMenuCursor(0);
      showPauseGameMenu();
    }

    // showing the lots game display
    void showLostGame() {
      display.clear();
      display.showLostGameDisplay();
    }

    // the player lost the game
    void lostGame() {
      resetMatrix();
      showLostGame();
      joystick.setSystemState(STATISTICS_STATE);
      startStatisticsTime = millis();
    }

    // moving to the next level
    void doneLevel() {
      if (level < MAX_LEVEL) {
        // taking a few moments before next level starts
        if (millis() - doneLevelTime > BETWEEN_LEVELS_INTERVAL) {
          // increasing level
          level++;
          initializeLevel();
          joystick.setSystemState(IN_GAME_STATE);
        }
      } else {
        // the player has completed the last level of the game => the game is done
        joystick.setSystemState(WON_GAME_STATE);
      }
    }

    // showing the won game display
    void showWonGame() {
      display.clear();
      display.showWonGameDisplay();
    }

    // the player won the game
    void wonGame() {
      resetMatrix();
      showWonGame();
      joystick.setSystemState(STATISTICS_STATE);
      startStatisticsTime = millis();
    }

    // showing the statistics of the game
    void showStatistics() {
      display.clear();
      display.showStatistics(lives, currentScore);
      shownStatistics = true;
    }

    // statistics of the game
    void statistics() {
      // taking a few moments before the statistics are shown
      if (millis() - startStatisticsTime > BETWEEN_END_SECTIONS && !shownStatistics) {
        showStatistics();

        if (currentScore > scores[HIGHSCORE_TOP - 1]) {
          // the current score of the player is in greater than the last score in the highscore board
          joystick.setSystemState(BEAT_HIGHSCORE_STATE);
          startBeatHighscoreTime = millis();
        }
      } else if (millis() - startStatisticsTime > 2 * BETWEEN_END_SECTIONS) {
        joystick.setSystemState(END_GAME_STATE);
        display.setEndGameMenuCursor(0);
        showEndGameMenu();
      }
    }

    // shwoing the beat highscore displays
    void showBeatHighscore() {
      display.clear();
      display.showBeatHighscoreDisplay();
    }

    void showEnterNameText() {
      display.clear();
      display.showEnterNameTextDisplay();
    }

    void showNameRestrictions() {
      display.clear();
      display.showNameRestrictionsDisplay();
    }

    // the player has beaten the highscore
    void beatHighscore() {
      // taking a few moments before changing between the messages
      if (millis() - startBeatHighscoreTime > BETWEEN_END_SECTIONS) {
        highscoreDisplay++;

        if (highscoreDisplay == BEAT_HIGHSCORE_DISPLAY) {
          showBeatHighscore();
        } else if (highscoreDisplay == ENTER_NAME_TEXT_DISPLAY) {
          showEnterNameText();
        } else if (highscoreDisplay == NAME_RESTRICTIONS_DISPLAY) {
          showNameRestrictions();
        } else if (highscoreDisplay == ENTER_NAME_DISPLAY) {
          joystick.setSystemState(ENTER_NAME_STATE);
          display.setEnterNameMenuCursorX(0);
          display.setEnterNameMenuCursorY(0);
          showEnterNameMenu();
        }

        startBeatHighscoreTime = millis();
      }
    }


    // --- PAUSE GAME MENU ---
    // shwoing the pause game menu
    void showPauseGameMenu() {
      display.clear();
      display.showPauseGameMenu();
    }

    // navigating through the pause game menu
    void navigatePauseGameMenu() {
      short int cursorPosition = display.getPauseGameMenuCursor();
      short int newCursorPosition = joystick.updateMenuPositionY(cursorPosition, PAUSE_GAME_MENU_ITEMS);

      // cursor moved => update display
      if (cursorPosition != newCursorPosition) {
        display.setPauseGameMenuCursor(newCursorPosition);
        showPauseGameMenu();
      }
    }

    // --- ENTER NAME MENU ---
    // showing the enter name menu
    void showEnterNameMenu() {
      display.clear();
      display.showEnterNameMenu(currentName);
    }

    // navigating through the enter name menu
    void navigateEnterNameMenu() {
      // cursor on the X axis
      short int cursorPositionX = display.getEnterNameMenuCursorX();
      short int newCursorPositionX = joystick.updateMenuPositionX(cursorPositionX, ENTER_NAME_MENU_ITEMS);

      // cursor moved => update display
      if (newCursorPositionX != cursorPositionX) {
        display.setEnterNameMenuCursorX(newCursorPositionX);
        showEnterNameMenu();
      }

      // cursor on the Y axis
      short int cursorPositionY = display.getEnterNameMenuCursorY();
      short int newCursorPositionY = joystick.updateMenuPositionY(cursorPositionY, KEYBOARD_COLUMNS);

      if (newCursorPositionY != cursorPositionY) {
        display.setEnterNameMenuCursorY(newCursorPositionY);
        showEnterNameMenu();
      }
    }


    //--- END GAME MENU ---
    // shwoing the end game menu
    void showEndGameMenu() {
      display.clear();
      display.showEndGameMenu();
    }

    // navigating through the end game menu
    void navigateEndGameMenu() {
      short int cursorPosition = display.getEndGameMenuCursor();
      short int newCursorPosition = joystick.updateMenuPositionY(cursorPosition, END_GAME_MENU_ITEMS);

      // cursor moved => update display
      if (cursorPosition != newCursorPosition) {
        display.setEndGameMenuCursor(newCursorPosition);
        showEndGameMenu();
      }
    }


    // --- HIGHSCORE BOARD ---
    // updating highscore board
    void updateHighscoreBoard() {
      // inserting the value into the top
      for (short int i = 0; i < HIGHSCORE_TOP; i++) {
        if (currentScore > scores[i]) {
          // moving the bottom of the top with one position lower
          for (short int j = HIGHSCORE_TOP - 1; j > i; j--) {
            scores[j] = scores[j - 1];
            names[j] = names[j - 1];
          }

          // inserting value
          scores[i] = currentScore;
          names[i] = currentName;

          break;
        }
      }

      // updating values into the memory
      // scores
      for (short int i = 0; i < HIGHSCORE_TOP; i++) {
        // int value needs to be stored into 2 addresses on the EEPROM
        // => the value is split into 2 bytes:
        // the first byte needs to be shifted right by 8 (in order to only get the firts 8 bits of the number)
        EEPROM.update(scoresAddresses[2 * i], (scores[i] >> 8));

        // the second byte needs to go through an AND operation with 0xFF (in order to only get the last 8 bits of the number)
        EEPROM.update(scoresAddresses[2 * i + 1], (scores[i] & 0xFF));
      }


      // names
      for (short int i = 0; i < HIGHSCORE_TOP; i ++) {
        // string value needs to have each of its characters stored at consecutive addresses on the EEPROM
        for (short int j = 1; j < names[i].length() + 1; j++)
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
      short int cursorPosition = display.getHighscoreBoardCursor();
      short int newCursorPosition = joystick.updateMenuPositionX(cursorPosition, HIGHSCORE_TOP + 1);

      // cursor moved => update display
      if (cursorPosition != newCursorPosition) {
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
      short int cursorPosition = display.getSettingsMenuCursor();
      short int newCursorPosition = joystick.updateMenuPositionX(cursorPosition, SETTINGS_MENU_ITEMS);

      // cursor moved => update display
      if (cursorPosition != newCursorPosition) {
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
      short int newStartLevelValue = joystick.updateValue(startLevelValue, MIN_LEVEL, MAX_LEVEL, LEVEL_STEP);

      // value changed => update display
      if (startLevelValue != newStartLevelValue) {
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
      short int contrastValue = display.getContrast();
      short int newContrastValue = joystick.updateValue(contrastValue, MIN_SETTINGS, MAX_SETTINGS_LCD, CONTRAST_STEP);

      // value changed => update display
      if (contrastValue != newContrastValue) {
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
      short int brightnessValue = display.getBrightness();
      short int newBrightnessValue = joystick.updateValue(brightnessValue, MIN_SETTINGS, MAX_SETTINGS_LCD, BRIGHTNESS_STEP);

      // value changed => update display
      if (brightnessValue != newBrightnessValue) {
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
      short int intensityValue = matrix.getIntensity();
      short int newIntensityValue = joystick.updateValue(intensityValue, MIN_SETTINGS, MAX_SETTINGS_LC, INTENSITY_STEP);

      // value changed => update display
      if (intensityValue != newIntensityValue) {
        matrix.setIntensity(newIntensityValue);
        showIntensitySettings();
      }
    }


    // --- ABOUT SECTION ---
    // showing the about section
    void showAbout() {
      display.clear();
      display.showAboutSection();
    }

    // navigate through the about section
    void navigateAbout() {
      short int cursorPosition = display.getAboutSectionCursor();
      short int newCursorPosition = joystick.updateMenuPositionX(cursorPosition, ABOUT_SECTION_ITEMS);

      // cursor moved => update display
      if (cursorPosition != newCursorPosition) {
        display.setAboutSectionCursor(newCursorPosition);
        showAbout();

        // resetting scrolling timer
        lastChangedScrollAbout = millis();
      }

      if (millis() - lastChangedScrollAbout > SCROLL_ABOUT_SECTION_INTERVAL) {
        scrollAbout();
        lastChangedScrollAbout = millis();
      }
    }

    // scrolling the text in the about section
    void scrollAbout() {
      display.scrollCurrentAboutSection();
    }


    // --- TRANSITIONS FROM MENUS ---
    // changing from the principal menu to one of the options
    void changeFromPrincipalMenu() {
      short int cursorPosition = display.getPrincipalMenuCursor();

      if (cursorPosition == START_GAME_POSITION) {
        // start game
        joystick.setSystemState(IN_GAME_STATE);
        resetGame();
        initializeLevel();
      } else if (cursorPosition == HIGHSCORE_POSITION) {
        // highscore
        joystick.setSystemState(HIGHSCORE_BOARD_STATE);
        display.setHighscoreBoardCursor(0);
        showHighscoreBoard();
      } else if (cursorPosition == SETTINGS_POSITION) {
        // settings menu
        joystick.setSystemState(SETTINGS_MENU_STATE);
        display.setSettingsMenuCursor(0);
        showSettingsMenu();
      } else {
        // about section
        // resetting timer
        lastChangedScrollAbout = millis();

        joystick.setSystemState(ABOUT_STATE);
        display.setAboutSectionCursor(0);
        showAbout();
      }
    }

    // changing from pause game menu
    void changeFromPauseGameMenu() {
      short int cursorPosition = display.getPauseGameMenuCursor();

      if (cursorPosition == PAUSE_GAME_MENU_ITEMS - 1) {
        // "exit" option => going back to the principal menu
        resetMatrix();
        joystick.setSystemState(PRINCIPAL_MENU_STATE);
        display.setPrincipalMenuCursor(0);
        showPrincipalMenu();
      } else {
        // "resume" option => resuming the game
        joystick.setSystemState(IN_GAME_STATE);
        showGameDisplay();
      }
    }

    // changing from enter name menu
    void changeFromEnterNameMenu() {
      short int cursorPositionX = display.getEnterNameMenuCursorX();
      short int cursorPositionY = display.getEnterNameMenuCursorY();

      if (cursorPositionX < KEYBOARD_LINES) {
        // one of the characters has been selected
        if (currentName.length() < MAX_NAME_LENGTH) {
          // adding the character to the existing name only if the name dones't
          // surpasses the maxim accepted length
          currentName += display.getCharacter(cursorPositionX, cursorPositionY);
        }

        joystick.setSystemState(ENTER_NAME_STATE);
        showEnterNameMenu();
      } else {
        // one of the options has been selected
        if (cursorPositionX == ENTER_NAME_MENU_ITEMS - 1) {
          // "done" option
          // updating highscore board
          updateHighscoreBoard();

          // going to the next state
          joystick.setSystemState(END_GAME_STATE);
          display.setEndGameMenuCursor(0);
          showEndGameMenu();
        } else {
          // "delete" option
          // deletting the last character of the current name
          currentName = currentName.substring(0, currentName.length() - 1);

          joystick.setSystemState(ENTER_NAME_STATE);
          showEnterNameMenu();
        }
      }
    }

    // changing from end game menu
    void changeFromEndGameMenu() {
      short int cursorPosition = display.getEndGameMenuCursor();

      if (cursorPosition == END_GAME_MENU_ITEMS - 1) {
        // "exit" option => going back to the principal menu
        joystick.setSystemState(PRINCIPAL_MENU_STATE);
        display.setPrincipalMenuCursor(0);
        showPrincipalMenu();
      } else {
        // "restart" option => restarting the game
        joystick.setSystemState(IN_GAME_STATE);
        resetGame();
        initializeLevel();
      }
    }

    // changing from the highscore board
    void changeFromHighscoreBoard() {
      short int cursorPosition = display.getHighscoreBoardCursor();

      if (cursorPosition == HIGHSCORE_TOP) {
        // going back to the principal menu
        // only if the button was pressed from the position of the "back" option
        joystick.setSystemState(PRINCIPAL_MENU_STATE);
        showPrincipalMenu();
      } else {
        // otherwise nothing happenes and the system remains in the same state
        joystick.setSystemState(HIGHSCORE_BOARD_STATE);
      }
    }

    // changing from the settings menu to one of the options
    void changeFromSettingsMenu() {
      short int cursorPosition = display.getSettingsMenuCursor();

      if (cursorPosition == START_LEVEL_POSITION) {
        // starting level
        joystick.setSystemState(START_LEVEL_SETTINGS_STATE);
        showStartLevelSettings();
      } else if (cursorPosition == CONTRAST_POSITION) {
        // lcd contrast
        joystick.setSystemState(CONTRAST_SETTINGS_STATE);
        showContrastSettings();
      } else if (cursorPosition == BRIGHTNESS_POSITION) {
        // lcd brightness
        joystick.setSystemState(BRIGHTNESS_SETTINGS_STATE);
        showBrightnessSettings();
      } else if (cursorPosition == INTENSITY_POSITION) {
        // matrix brightness intensity
        joystick.setSystemState(INTENSITY_SETTINGS_STATE);
        showIntensitySettings();
      } else {
        // back to principal menu
        joystick.setSystemState(PRINCIPAL_MENU_STATE);
        showPrincipalMenu();
      }
    }

    // changing from one of the settings' section back to the settings menu
    void changeFromSettingsSection() {
      joystick.setSystemState(SETTINGS_MENU_STATE);
      showSettingsMenu();
    }

    // changing from the about section
    void changeFromAbout() {
      short int cursorPosition = display.getAboutSectionCursor();

      if (cursorPosition == ABOUT_SECTION_ITEMS - 1) {
        // going back to the principal menu
        // only if the button was pressed from the position of the "back" option
        joystick.setSystemState(PRINCIPAL_MENU_STATE);
        showPrincipalMenu();
      } else {
        // otherwise nothing happenes and the system remains in the same state
        joystick.setSystemState(ABOUT_STATE);
      }
    }
};
