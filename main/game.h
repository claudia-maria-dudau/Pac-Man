#include "display.h"
#include "matrix.h"
#include "joystick.h"
#include "buzzer.h"

class Game {
    Display display = Display();
    Matrix matrix = Matrix();
    Joystick joystick = Joystick();
    Buzzer buzzer = Buzzer();

    // in game variables
    int lives;
    int currentScore;

    // level
    int level;
    unsigned long doneLevelTime;
    int startLevelValue;

    // player
    int playerPosition[2];
    unsigned long lastChangedPlayerPosition;
    unsigned long lastChangedPlayerBlinking;
    String currentName;

    // enemies
    int noEnemies;
    int enemies[MAX_ENEMIES][2]; // each enemy has a set of (x, y) coordinates to indicate its position
    int enemiesSpeed;
    unsigned long lastChangedEnemiesPositions;
    unsigned long lastChangedEnemiesBlinking;
    const int movesX[MOVES] = { -1, -1, -1, 0, 1, 1, 1, 0 };
    const int movesY[MOVES] = { -1, 0, 1, 1, 1, 0, -1, -1 };

    // food items
    int noFoodItems;
    int foodItems[MAX_FOOD_ITEMS][2]; // each food item has a set of (x, y) coordinates to indicate its position

    // map
    int mapSize;
    int firstLine;
    int firstColumn;

    // timers
    unsigned long lastChangedGameSection;

    // end sections
    bool shownMessage;
    bool shownStatistics;
    bool shownBeatHighscore;
    int highscoreDisplay;

    // highscore board
    String names[HIGHSCORE_TOP] = { "", "", "", "", "" };
    int scores[HIGHSCORE_TOP] = { 0, 0, 0, 0, 0 };
    int namesAddresses[HIGHSCORE_TOP * (MAX_NAME_LENGTH + 1)];
    int scoresAddresses[HIGHSCORE_TOP * 2];

    // about section
    unsigned long lastChangedScrollAbout;


    // --- GAME LOGIC ---
    // verifying whether an element is in the bounds of the given map section or not
    bool isInSectionBound(int line, int column, int positionX, int positionY) {
      return (positionX >= line && positionX < line + MATRIX_SIZE) && (positionY >= column && positionY < column + MATRIX_SIZE);
    }

    // intializing the position of the player
    void initializePlayer() {
      // generating a pair of random coordinates for
      // the intial position of the player
      playerPosition[0] = random(mapSize);
      playerPosition[1] = random(mapSize);

      // centering the position of the player on the matrix
      firstLine = min(max(playerPosition[0] - HALF_MATRIX_SIZE, 0), mapSize - MATRIX_SIZE);
      firstColumn = min(max(playerPosition[1] - HALF_MATRIX_SIZE, 0), mapSize - MATRIX_SIZE);

      matrix.setPosition(playerPosition[0] - firstLine, playerPosition[1] - firstColumn);
    }

    // initializing the positions of the enemies
    void initializeEnemies() {
      int xCoord, yCoord;

      // generating random positions for enemies
      for (int i = 0; i < noEnemies; i++) {
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

          if (coordOk) {
            // verifying that the position of the coordinates is empty
            // (there isn;t already another enemy placed there)
            for (int j = 0; j < i; j++) {
              if (enemies[j][0] == xCoord && enemies[j][1] == yCoord) {
                coordOk = false;
                break;
              }
            }
          }
        }

        enemies[i][0] = xCoord;
        enemies[i][1] = yCoord;

        if (isInSectionBound(firstLine, firstColumn, enemies[i][0], enemies[i][1])) {
          matrix.setPosition(enemies[i][0] - firstLine, enemies[i][1] - firstColumn);
        }
      }
    }

    // initialize the positions of the food items
    void initializeFoodItems() {
      int xCoord, yCoord;

      // generating random positions for food items
      for (int i = 0; i < noFoodItems; i++) {
        bool coordOk = false;

        while (!coordOk) {
          // generating a random pair (x, y) of coordinates
          xCoord = random(mapSize);
          yCoord = random(mapSize);

          coordOk = true;

          // verifying that the position of the generated coordinates
          // is not already taken by the player
          if (xCoord == playerPosition[0] && yCoord == playerPosition[1])
            coordOk = false;

          if (coordOk) {
            // verifying that the position of the generated coordinates
            // is not already taken by an enemy
            for (int j = 0; j < noEnemies; j++) {
              if (enemies[j][0] == xCoord && enemies[j][1] == yCoord) {
                coordOk = false;
                break;
              }
            }
          }

          if (coordOk) {
            // verifying that the position of the generated coordinates
            // is not already taken by another food item
            for (int j = 0; j < i; j++) {
              if (foodItems[j][0] == xCoord && foodItems[j][1] == yCoord) {
                coordOk = false;
                break;
              }
            }
          }
        }

        foodItems[i][0] = xCoord;
        foodItems[i][1] = yCoord;

        if (isInSectionBound(firstLine, firstColumn, foodItems[i][0], foodItems[i][1])) {
          matrix.setPosition(foodItems[i][0] - firstLine, foodItems[i][1] - firstColumn);
        }
      }
    }

    // changing the position of an element (enemy or food item) on the map section
    void changePosition(int* position, int lastFirstLine, int lastFirstColumn) {
      if (isInSectionBound(lastFirstLine, lastFirstColumn, position[0], position[1])) {
        // previous position was in the bounds of the last section
        if (isInSectionBound(firstLine, firstColumn, position[0], position[1])) {
          // actual position is in the bounds of the new section
          // => chnaging the position on the matrix
          matrix.changePosition(position[0] - lastFirstLine, position[1] - lastFirstColumn,
                                position[0] - firstLine, position[1] - firstColumn);
        } else {
          // actual position is no longer in the bounds of the new section
          // => resetting position on the matrix
          matrix.resetPosition(position[0] - lastFirstLine, position[1] - lastFirstColumn);
        }
      } else {
        // previous position was not in the bounds of the last section
        if (isInSectionBound(firstLine, firstColumn, position[0], position[1])) {
          // actual position is in the bounds of the new section
          // => setting the position on the matrix
          matrix.setPosition(position[0] - firstLine, position[1] - firstColumn);
        }
      }
    }

    // showing a section of the map according to the position of the player
    void showMap(int newPlayerPositionX, int newPlayerPositionY) {
      int lastFirstLine = min(max(playerPosition[0] - HALF_MATRIX_SIZE, 0), mapSize - MATRIX_SIZE);
      int lastFirstColumn = min(max(playerPosition[1] - HALF_MATRIX_SIZE, 0), mapSize - MATRIX_SIZE);

      // centering the position of the player on the matrix
      firstLine = min(max(newPlayerPositionX - HALF_MATRIX_SIZE, 0), mapSize - MATRIX_SIZE);
      firstColumn = min(max(newPlayerPositionY - HALF_MATRIX_SIZE, 0), mapSize - MATRIX_SIZE);

      // changing the position of the player on the matrix
      matrix.changePosition(playerPosition[0] - lastFirstLine, playerPosition[1] - lastFirstColumn,
                            newPlayerPositionX - firstLine, newPlayerPositionY - firstColumn);

      if (lastFirstLine != firstLine || lastFirstColumn != firstColumn) {
        // changing the position of the enemies on the matrix
        for (int i = 0; i < noEnemies; i++) {
          changePosition(enemies[i], lastFirstLine, lastFirstColumn);
        }

        // changing the position of the food items on the matrix
        for (int i = 0; i < noFoodItems; i++) {
          changePosition(foodItems[i], lastFirstLine, lastFirstColumn);
        }
      }
    }

    // decreasing the number of lives of the player
    void decreaseLife() {
      buzzer.playMeetEnemy();

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
    void eatFoodItem(int position) {
      buzzer.playEatFoodItem();

      // erasing the respective food item form the map
      matrix.resetPosition(foodItems[position][0] - firstLine, foodItems[position][1] - firstColumn);

      // erasing the respective food item from the list of food items
      for (int j = position; j < noFoodItems - 1; j++) {
        foodItems[j][0] = foodItems[j + 1][0];
        foodItems[j][1] = foodItems[j + 1][1];
      }

      noFoodItems--;

      // increasing the player's score
      // (each food item values as much as the current level of the game)
      updateScore(level);
    }

    // blinking the player's position at a given interval
    void blinkPlayer() {
      if (millis() - lastChangedPlayerBlinking > PLAYER_BLINKING_INTERVAL) {
        matrix.blinkPlayer(playerPosition[0] - firstLine, playerPosition[1] - firstColumn);

        lastChangedPlayerBlinking = millis();
      }
    }

    // blinking the enemies' position at a given intervla
    void blinkEnemies() {
      if (millis() - lastChangedEnemiesBlinking > ENEMIES_BLINKING_INTERVAL) {
        for (int i = 0; i < noEnemies; i++) {
          if (isInSectionBound(firstLine, firstColumn, enemies[i][0], enemies[i][1])) {
            matrix.blinkEnemy(enemies[i][0] - firstLine, enemies[i][1] - firstColumn);
          }
        }

        matrix.doneBlinkEnemies();

        lastChangedEnemiesBlinking = millis();
      }
    }

    // moving enemies at a given interval
    void moveEnemies() {
      if (millis() - lastChangedEnemiesPositions > enemiesSpeed) {
        int move;
        int next[2];

        // generating a random move for each enemy
        for (int i = 0; i < noEnemies; i++) {
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
              // verifying that the next position is not already occupied by another enemy
              for (int j = 0; j < i; j++) {
                if (enemies[j][0] == next[0] && enemies[j][1] == next[1]) {
                  moveOk = false;
                  break;
                }
              }
            }

            if (moveOk) {
              // verifying that the next position is not already occupied by a food item
              for (int j = 0; j < noFoodItems; j++) {
                if (foodItems[j][0] == next[0] && foodItems[j][1] == next[1]) {
                  moveOk = false;
                  break;
                }
              }
            }
          }

          // changing the position of the enemy on the matrix
          if (isInSectionBound(firstLine, firstColumn, enemies[i][0], enemies[i][1])) {
            // previous position of the enemy was in the bounds of the last section
            if (isInSectionBound(firstLine, firstColumn, next[0], next[1])) {
              // actual position of the enemy is in the bounds of the new section
              // => chnaging the position on the matrix
              matrix.changePosition(enemies[i][0] - firstLine, enemies[i][1] - firstColumn,
                                    next[0] - firstLine, next[1] - firstColumn);
            } else {
              // actual position of the enemy is no longer in the bounds of the new section
              // => resetting position on the matrix
              matrix.resetPosition(enemies[i][0] - firstLine, enemies[i][1] - firstColumn);
            }
          } else {
            // previous position of the enemy was not in the bounds of the last section
            if (isInSectionBound(firstLine, firstColumn, next[0], next[1])) {
              // actual position of the enemy is in the bounds of the new section
              // => setting the position on the matrix
              matrix.setPosition(next[0] - firstLine, next[1] - firstColumn);
            }
          }

          enemies[i][0] = next[0];
          enemies[i][1] = next[1];


          // verifying if the enemy landed on the same position as the player
          if (playerPosition[0] == enemies[i][0] && playerPosition[1] == enemies[i][1])
            decreaseLife();
        }

        lastChangedEnemiesPositions = millis();
      }
    }

    // moving the player with the joystick
    void movePlayer() {
      if (millis() - lastChangedPlayerPosition > PLAYER_SPEED) {
        int newPosition[2] = { joystick.movePlayerX(playerPosition[0], mapSize), joystick.movePlayerY(playerPosition[1], mapSize)};

        // player moved => update position
        if (newPosition[0] != playerPosition[0] || newPosition[1] != playerPosition[1]) {
          showMap(newPosition[0], newPosition[1]);
          playerPosition[0] = newPosition[0];
          playerPosition[1] = newPosition[1];

          //verifying if the player landed on the same position as one of the enemies
          for (int i = 0; i < noEnemies; i++) {
            if (enemies[i][0] == playerPosition[0] && enemies[i][1] == playerPosition[1]) {
              // player loses a life
              decreaseLife();
              break;
            }
          }

          // verifying id the player landed on the position of a food item
          for (int i = 0; i < noFoodItems; i++) {
            if (foodItems[i][0] == playerPosition[0] && foodItems[i][1] == playerPosition[1]) {
              // player eats the food item
              eatFoodItem(i);
              break;
            }
          }
        }

        lastChangedPlayerPosition = millis();
      }
    }

  public:
    Game() {
      // memory addresses for highscore top
      // scores
      for (int i = 0; i < HIGHSCORE_TOP * 2; i++)
        scoresAddresses[i] = START_NAMES_ADDRESS + i;

      // names
      for (int i = 0; i < HIGHSCORE_TOP * (MAX_NAME_LENGTH + 1); i++)
        namesAddresses[i] = START_SCORES_ADDRESS + i;

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

      startLevelValue = MIN_LEVEL;
      joystick.setSystemState(INTRO_STATE);
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
      matrix.startAnimation();
      display.startText();

      if (display.getIsDoneIntro()) {
        changeFromIntro();
      }
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
      int newCursorPosition = joystick.updateMenuPositionX(cursorPosition, PRINCIPAL_MENU_ITEMS);

      // cursor moved => update display
      if (cursorPosition != newCursorPosition) {
        display.setPrincipalMenuCursor(newCursorPosition);
        showPrincipalMenu();
      }
    }


    // --- GAME ---
    // showing the start game display
    void showStartGame() {
      display.clear();
      display.showStartGameDisplay();

      buzzer.startSong();
    }

    // starting the game
    void startGame() {
      if (millis() - lastChangedGameSection > BETWEEN_LEVELS_INTERVAL) {
        // initializing the first level of the game
        resetGame();
        initializeLevel();
        joystick.setSystemState(IN_GAME_STATE);
      } else {
        // playing the song before the game starts
        buzzer.playGameSong();
      }
    }

    // resetting game variables
    void resetGame() {
      lives = MAX_LIVES;
      currentScore = 0;
      level = startLevelValue;
      currentName = "";
      shownMessage = false;
      shownStatistics = false;
      shownBeatHighscore = false;
    }

    // showing the in game display
    void showGameDisplay() {
      display.clear();
      display.showGameDisplay(level, lives, currentScore);
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

      matrix.initialize();

      initializePlayer();
      initializeEnemies();
      initializeFoodItems();

      // resetting timers
      lastChangedPlayerPosition = millis();
      lastChangedPlayerBlinking = millis();
      lastChangedEnemiesPositions = millis();
      lastChangedEnemiesBlinking = millis();
    }

    // the flow of the game
    void playGame() {
      if (noFoodItems == 0) {
        // no more food items on the map => leve is done
        joystick.setSystemState(DONE_LEVEL_STATE);
        matrix.clear();
        matrix.startDoneLevelAnimation();

        buzzer.startSong();

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
      shownMessage = true;
    }

    // the player lost the game
    void lostGame() {
      resetMatrix();
      showLostGame();
      moveToStatistics();
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
        } else {
          matrix.showDoneLevelAnimation();
          buzzer.playGameSong();
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
      shownMessage = true;
    }

    // the player won the game
    void wonGame() {
      resetMatrix();
      showWonGame();
      moveToStatistics();
    }

    // moving to the statistics section
    void moveToStatistics() {
      joystick.setSystemState(STATISTICS_STATE);
      lastChangedGameSection = millis();
      shownStatistics = false;
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
      if (millis() - lastChangedGameSection > BETWEEN_END_SECTIONS && !shownStatistics) {
        showStatistics();

        if (currentScore > scores[HIGHSCORE_TOP - 1]) {
          // the current score of the player is in greater than the last score in the highscore board
          joystick.setSystemState(BEAT_HIGHSCORE_STATE);
          highscoreDisplay = BEAT_HIGHSCORE_DISPLAY - 1;
          lastChangedGameSection = millis();
        }
      } else if (millis() - lastChangedGameSection > 2 * BETWEEN_END_SECTIONS) {
        joystick.setSystemState(END_GAME_STATE);
        display.setEndGameMenuCursor(0);
        showEndGameMenu();
      }
    }

    // shwoing the beat highscore displays
    void showBeatHighscore() {
      display.clear();
      display.showBeatHighscoreDisplay();
      shownBeatHighscore = true;
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
      if (millis() - lastChangedGameSection > BETWEEN_END_SECTIONS) {
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

        lastChangedGameSection = millis();
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
      int cursorPosition = display.getPauseGameMenuCursor();
      int newCursorPosition = joystick.updateMenuPositionY(cursorPosition, PAUSE_GAME_MENU_ITEMS);

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
      int cursorPositionX = display.getEnterNameMenuCursorX();
      int newCursorPositionX = joystick.updateMenuPositionX(cursorPositionX, ENTER_NAME_MENU_ITEMS);

      // cursor moved => update display
      if (newCursorPositionX != cursorPositionX) {
        display.setEnterNameMenuCursorX(newCursorPositionX);
        showEnterNameMenu();
      }

      // cursor on the Y axis
      int cursorPositionY = display.getEnterNameMenuCursorY();
      int newCursorPositionY = joystick.updateMenuPositionY(cursorPositionY, KEYBOARD_COLUMNS);

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
      int cursorPosition = display.getEndGameMenuCursor();
      int newCursorPosition = joystick.updateMenuPositionY(cursorPosition, END_GAME_MENU_ITEMS);

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
      for (int i = 0; i < HIGHSCORE_TOP; i++) {
        if (currentScore > scores[i]) {
          // moving the bottom of the top with one position lower
          for (int j = HIGHSCORE_TOP - 1; j > i; j--) {
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
      int newCursorPosition = joystick.updateMenuPositionX(cursorPosition, HIGHSCORE_TOP + 1);

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
      int cursorPosition = display.getSettingsMenuCursor();
      int newCursorPosition = joystick.updateMenuPositionX(cursorPosition, SETTINGS_MENU_ITEMS);

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
      int newStartLevelValue = joystick.updateValue(startLevelValue, MIN_LEVEL, MAX_LEVEL, LEVEL_STEP);

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
      int contrastValue = display.getContrast();
      int newContrastValue = joystick.updateValue(contrastValue, MIN_SETTINGS, MAX_SETTINGS_LCD, CONTRAST_STEP);

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
      int brightnessValue = display.getBrightness();
      int newBrightnessValue = joystick.updateValue(brightnessValue, MIN_SETTINGS, MAX_SETTINGS_LCD, BRIGHTNESS_STEP);

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
      int intensityValue = matrix.getIntensity();
      int newIntensityValue = joystick.updateValue(intensityValue, MIN_SETTINGS, MAX_SETTINGS_LC, INTENSITY_STEP);

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
      int cursorPosition = display.getAboutSectionCursor();
      int newCursorPosition = joystick.updateMenuPositionX(cursorPosition, ABOUT_SECTION_ITEMS);

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
    // changing from intro to the principal menu
    void changeFromIntro() {
      joystick.setSystemState(PRINCIPAL_MENU_STATE);
      showPrincipalMenu();
    }

    // changing from the principal menu to one of the options
    void changeFromPrincipalMenu() {
      int cursorPosition = display.getPrincipalMenuCursor();

      if (cursorPosition == START_GAME_POSITION) {
        // start game
        joystick.setSystemState(START_GAME_STATE);
        showStartGame();
        lastChangedGameSection = millis();
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

    // changing from pause game menu to one of the options
    void changeFromPauseGameMenu() {
      int cursorPosition = display.getPauseGameMenuCursor();

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

    // changing from one end section to another
    void changeFromEndSections() {
      if (shownMessage && !shownStatistics && !shownBeatHighscore) {
        // moving to statistics display
        joystick.setSystemState(STATISTICS_STATE);
        lastChangedGameSection = millis() - BETWEEN_END_SECTIONS;
      } else if (shownStatistics && !shownBeatHighscore) {
        if (currentScore > scores[HIGHSCORE_TOP - 1]) {
          // moving to beat highscore display
          joystick.setSystemState(BEAT_HIGHSCORE_STATE);
          highscoreDisplay = BEAT_HIGHSCORE_DISPLAY - 1;
          lastChangedGameSection = millis() - BETWEEN_END_SECTIONS;
        } else {
          // moving to end game dispaly
          joystick.setSystemState(END_GAME_STATE);
          display.setEndGameMenuCursor(0);
          showEndGameMenu();
        }
      } else if (shownBeatHighscore) {
        // moving to the next display of the beat highscore state
        joystick.setSystemState(BEAT_HIGHSCORE_STATE);
        lastChangedGameSection = millis() - BETWEEN_END_SECTIONS;
      }
    }

    // changing from enter name menu
    void changeFromEnterNameMenu() {
      int cursorPositionX = display.getEnterNameMenuCursorX();
      int cursorPositionY = display.getEnterNameMenuCursorY();

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
          if (currentName != "") {
            // updating highscore board
            updateHighscoreBoard();

            // going to the next state
            joystick.setSystemState(END_GAME_STATE);
            display.setEndGameMenuCursor(0);
            showEndGameMenu();
          } else {
            joystick.setSystemState(ENTER_NAME_STATE);
            showEnterNameMenu();
          }
        } else {
          // "delete" option
          if (currentName.length() > 0) {
            // deletting the last character of the current name if the
            // name is different than ""
            currentName = currentName.substring(0, currentName.length() - 1);
          }

          joystick.setSystemState(ENTER_NAME_STATE);
          showEnterNameMenu();
        }
      }
    }

    // changing from end game menu to one of the options
    void changeFromEndGameMenu() {
      int cursorPosition = display.getEndGameMenuCursor();

      if (cursorPosition == END_GAME_MENU_ITEMS - 1) {
        // "exit" option => going back to the principal menu
        joystick.setSystemState(PRINCIPAL_MENU_STATE);
        display.setPrincipalMenuCursor(0);
        showPrincipalMenu();
      } else {
        // "restart" option => restarting the game
        joystick.setSystemState(START_GAME_STATE);
        showStartGame();
        lastChangedGameSection = millis();
      }
    }

    // changing from the highscore board
    void changeFromHighscoreBoard() {
      int cursorPosition = display.getHighscoreBoardCursor();

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
      int cursorPosition = display.getSettingsMenuCursor();

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
      int cursorPosition = display.getAboutSectionCursor();

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
