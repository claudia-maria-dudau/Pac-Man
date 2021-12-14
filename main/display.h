#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "system_constants.h"

class Display {
    // display settings
    int contrast;
    int brightness;

    // display
    LiquidCrystal lcd = LiquidCrystal(RS, ENABLE, D4, D5, D6, D7);

    // dog character
    const byte dogChar[LINES] = {
      B00000,
      B00000,
      B01000,
      B11001,
      B01110,
      B01110,
      B01010,
      B00000
    };

    // down arrow character
    const byte downArrowChar[LINES] = {
      B00000,
      B00000,
      B00000,
      B00000,
      B00000,
      B10001,
      B01010,
      B00100
    };

    // intro
    int introSection;
    int animationCursor = 0;
    bool isDoneIntro;
    int startIntroTime;
    int startAnimationTime;

    // principal menu
    const String principalMenu[PRINCIPAL_MENU_ITEMS] = {
      "Start game",
      "Highscore",
      "Settings",
      "About"
    };
    int principalMenuCursor = 0;

    // pause game menu
    int pauseGameMenuCursor = 0;
    const String pauseGameMenuTitle = "PAUSE";

    // enter name menu
    const String keyboard[KEYBOARD_LINES] = {
      "abcdefghijklm",
      "nopqrstuvwxyz"
    };
    const String enterNameMenu[ENTER_NAME_MENU_OPTIONS] = {
      "Delete",
      "Done"
    };
    int enterNameMenuCursor[2] = { 0 , 0 };

    // end game menu
    int endGameMenuCursor = 0;
    const String endGameMenuTitle = "END GAME";

    // highscore board
    int highscoreBoardCursor = 0;

    // settings menu
    const String settingsMenu[SETTINGS_MENU_ITEMS] = {
      "Start level",
      "Contrast",
      "Brightness",
      "Intensity",
      "Back"
    };
    int settingsMenuCursor = 0;

    // about section
//    const String aboutSection[ABOUT_SECTION_ITEMS] = {
//      "Name: Marinel's epic quest for food   ",
//      "Creator: Claudia Dudau   ",
//      "Github: https://github.com/claudia-maria-dudau/Pac-Man   ",
//      "Back"
//    };
    int aboutSectionCursor = 0;
    int scrollPosition = 0;

    bool upArrow = false;
    bool downArrow = true;


    // --- GENERAL FUNCTIONS FOR SHOWING MENUS ---
    // setting the navigation arrows
    void setArrows(int cursorPosition, int noItems) {
      if (cursorPosition < 1) {
        // the cursor is on one of the first two items of the menu
        // => up arrow is not shown anymore
        upArrow = false;
      } else {
        upArrow = true;
      }

      if (cursorPosition >= noItems - 2) {
        // the cursor is on one of the last two items of the menu
        // => down arrow is not shown anymore
        downArrow = false;
      } else {
        downArrow = true;
      }
    }

    // showing navigation arrows on the last column of the display
    void showNavigationArrows() {
      if (upArrow) {
        // showing up arrow
        lcd.setCursor(DISPLAY_COLUMNS - 1, 0);
        lcd.write(0b01011110);
      }

      if (downArrow) {
        // showing down arrow
        lcd.setCursor(DISPLAY_COLUMNS - 1, 1);
        lcd.write((byte)1);
      }
    }

    // showing the cursor at a given position on the display
    void showCursor(int col, int row) {
      lcd.setCursor(col, row);
      lcd.write(0b00111110);
    }

    // showing a menu option at a given position
    void showMenuOption(int col, int row, String menuOption) {
      lcd.setCursor(col, row);
      if (menuOption.length() < MENU_ITEM_LENGTH) {
        lcd.print(menuOption);
      } else {
        lcd.print(menuOption.substring(0, MENU_ITEM_LENGTH));
      }
    }

    // showing the given menu
    void showMenu(String* menu, int cursorPosition, int noItems) {
      // shwoing the current 2 menu items
      if (cursorPosition == noItems - 1) {
        // the cursor is on the last item of the menu
        // => showing the last 2 items
        showMenuOption(OPTIONS_OFFSET, 0, menu[cursorPosition - 1]);
        showMenuOption(OPTIONS_OFFSET, 1, menu[cursorPosition]);

        // showing where the cursor is currently
        showCursor(0, 1);

      } else {
        // showing the item the cursor is pointing at + the next one
        showMenuOption(OPTIONS_OFFSET, 0, menu[cursorPosition]);
        showMenuOption(OPTIONS_OFFSET, 1, menu[cursorPosition + 1]);

        // showing where the cursor is currently
        showCursor(0, 0);
      }

      showNavigationArrows();
    }

    // showing the given game menu
    void showGameMenu(String title, String menuItem, int cursorPosition) {
      // showing the title of the menu
      int offset = (DISPLAY_COLUMNS - title.length()) / 2;
      lcd.setCursor(offset, 0);
      lcd.print(title);

      // showing the options
      lcd.setCursor(2, 1);
      lcd.print(menuItem);
      lcd.setCursor(2 + BETWEEN_OPTIONS_OFFSET, 1);
      lcd.print(EXIT_BUTTON);

      // showing where4 the cursor is currently
      showCursor(cursorPosition * BETWEEN_OPTIONS_OFFSET, 1);
    }

  public:
    // display initialization
    Display() {
      // contrast + brightness pins are in OUTPUT mode
      pinMode(CONTRAST_PIN, OUTPUT);
      pinMode(BRIGHTNESS_PIN, OUTPUT);

      // loading values from memory
      contrast = EEPROM.read(CONTRAST_ADDRESS);
      brightness = EEPROM.read(BRIGHTNESS_ADDRESS);

      lcd.begin(16, 2);

      showContrast();
      showBrightness();

      // creating custom characters
      lcd.createChar(0, dogChar);
      lcd.createChar(1, downArrowChar);

      // initializing intro variables
      startIntroTime = 0;
      introSection = TEXT_SECTION_1;
      isDoneIntro = false;
    }

    // claering the display
    void clear() {
      lcd.clear();
    }


    // --- INTRO ---
    bool getIsDoneIntro() {
      return isDoneIntro;
    }

    // showing text at the start of the game
    void startText() {
      // advancing with the intro as time passes
      if (millis() - startIntroTime > INTRO_SECTIONS_INTERVAL && introSection != ANIMATION_SECTION) {
        // changing current intro section
        clear();
        introSection++;
        startIntroTime = millis();

        if (introSection == ANIMATION_SECTION) {
          // initializing animation variables
          startAnimationTime = millis();
          animationCursor = 0;
        }
      }

      if (introSection == TEXT_SECTION_1) {
        lcd.setCursor(0, 0);
        lcd.print("Once upon a time");
        lcd.setCursor(1, 1);
        lcd.print("lived Marinel");
      } else if (introSection == TEXT_SECTION_2) {
        lcd.setCursor(0, 0);
        lcd.print("A dog with a big");
        lcd.setCursor(0, 1);
        lcd.print("passion for food");
      } else if (introSection == TEXT_SECTION_3) {
        lcd.setCursor(2, 0);
        lcd.print("But in order");
        lcd.setCursor(3, 1);
        lcd.print("to get it");
      } else if (introSection == TEXT_SECTION_4) {
        lcd.setCursor(0, 0);
        lcd.print("He had to travel");
        lcd.setCursor(0, 1);
        lcd.print("uncharted ground");
      } else if (introSection == ANIMATION_SECTION) {
        if (millis() - startAnimationTime > INTRO_ANIMATION_INTERVAL) {
          animationCursor ++;
          startAnimationTime = millis();
        }

        if (animationCursor < DISPLAY_COLUMNS) {
          lcd.setCursor(animationCursor, animationCursor % 2);
          lcd.write((byte)0);
        } else {
          isDoneIntro = true;
        }
      }
    }


    // --- PRINCIPAL MENU ---
    // getting the position of the principal menu cursor
    int getPrincipalMenuCursor() {
      return principalMenuCursor;
    }

    // setting the position of the principal menu cursor
    void setPrincipalMenuCursor(int position) {
      principalMenuCursor = position;
      setArrows(principalMenuCursor, PRINCIPAL_MENU_ITEMS);
    }

    // showing the principal game menu
    void showPrincipalMenu() {
      showMenu(principalMenu, principalMenuCursor, PRINCIPAL_MENU_ITEMS);
    }

    // --- GAME ---
    // shwoing the start game display
    void showStartGameDisplay() {
      lcd.setCursor(6, 0);
      lcd.print("OKAY,");
      lcd.setCursor(4, 1);
      lcd.print("LET'S GO!");    
    }
    
    // showing the in game display
    void showGameDisplay(int level, int lives, int score) {
      // showing the level of the game
      lcd.setCursor(0, 0);
      lcd.print("Level:");
      lcd.setCursor(7, 0);
      lcd.print(level);

      // showing the number of lives of the player
      for (int i = 0; i < lives; i++) {
        lcd.setCursor(DISPLAY_COLUMNS - 1 - i, 0);
        lcd.write((byte)0);
      }

      // showing the current score of the player
      lcd.setCursor(0, 1);
      lcd.print("Score:");
      lcd.setCursor(7, 1);
      lcd.print(score);
    }

    // showing the lost game display
    void showLostGameDisplay() {
      lcd.setCursor(2, 0);
      lcd.print("You lost! :(");
      lcd.setCursor(0, 1);
      lcd.print("Sad Marinel bark");
    }

    // showing the won game display
    void showWonGameDisplay() {
      lcd.setCursor(0, 0);
      lcd.print("CONGRATULATIONS!");
      lcd.setCursor(0, 1);
      lcd.print("You won the game");
    }

    // shwoing the statistics of the game
    void showStatistics(int lives, int score) {
      lcd.setCursor(0, 0);
      lcd.print("Lives:");
      lcd.setCursor(7, 0);
      lcd.print(lives);
      
      lcd.setCursor(0, 1);
      lcd.print("Score:");
      lcd.setCursor(7, 1);
      lcd.print(score);
    }

    // showing the beat highscore displays
    void showBeatHighscoreDisplay() {
      lcd.setCursor(2, 0);
      lcd.print("You beat the");
      lcd.setCursor(3, 1);
      lcd.print("HIGHSCORE!");
    }

    void showEnterNameTextDisplay() {
      lcd.setCursor(2, 0);
      lcd.print("Please enter");
      lcd.setCursor(3, 1);
      lcd.print("your name");
    }
    

    // --- PAUSE GAME MENU ---
    // getting the pause game menu cursor
    int getPauseGameMenuCursor() {
      return pauseGameMenuCursor;
    }

    // setting the pause game menu cursor
    void setPauseGameMenuCursor(int position) {
      pauseGameMenuCursor = position;
    }

    // showing the end game menu
    void showPauseGameMenu() {
      showGameMenu(PAUSE_GAME_TITLE, RESUME_BUTTON, pauseGameMenuCursor);
    }

    // --- ENTER NAME MENU ---
    // getting the character at a certain position from the keyboard
    char getCharacter(int row, int col) {
      return keyboard[row][col];
    }

    // gettings the position of the enter menu cursor
    int getEnterNameMenuCursorX() {
      return enterNameMenuCursor[0];
    }

    int getEnterNameMenuCursorY() {
      return enterNameMenuCursor[1];
    }

    // setting the position of the enter menu cursor
    void setEnterNameMenuCursorX(int position) {
      enterNameMenuCursor[0] = position;
      setArrows(enterNameMenuCursor[0], ENTER_NAME_MENU_ITEMS + 1);
    }

    void setEnterNameMenuCursorY(int position) {
      enterNameMenuCursor[1] = position;
    }

    // showing the enter name menu
    void showEnterNameMenu(String name) {
      showNavigationArrows();

      // the name is always showing on the first row of the display
      lcd.setCursor(0, 0);
      lcd.print("Name:");
      lcd.setCursor(6, 0);
      lcd.print(name);

      lcd.setCursor(OPTIONS_OFFSET, 1);

      if (enterNameMenuCursor[0] < KEYBOARD_LINES) {
        // the cursor is on a character on the keyboard
        lcd.print(keyboard[enterNameMenuCursor[0]]);

        // showing the selected character on the keyboard
        lcd.setCursor(enterNameMenuCursor[1] + KEYBOARD_START_COLUMN, 1);
        lcd.blink();
      } else {
        // the cursor is on an option from the menu
        setEnterNameMenuCursorY(0);
        lcd.print(enterNameMenu[enterNameMenuCursor[0] - KEYBOARD_LINES]);

        // showing where the cursor is currently
        showCursor(0, 1);
        lcd.noBlink();
      }
    }


    // -- END GAME MENU ---
    // getting the end game menu cursor
    int getEndGameMenuCursor() {
      return endGameMenuCursor;
    }

    // setting the end game menu cursor
    void setEndGameMenuCursor(int position) {
      endGameMenuCursor = position;
    }

    // showing the end game menu
    void showEndGameMenu() {
      showGameMenu(END_GAME_TITLE, RESTART_BUTTON, endGameMenuCursor);
    }


    // --- HIGHSCORE BOARD ---
    // getting the position of the highscore board cursor
    int getHighscoreBoardCursor() {
      return highscoreBoardCursor;
    }

    // setting the position of the principal menu cursor
    void setHighscoreBoardCursor(int position) {
      highscoreBoardCursor = position;
      setArrows(highscoreBoardCursor, HIGHSCORE_TOP + 1);
    }

    // showing a highscore on the board at a given line
    void showHighscore(String name, int score, int line) {
      lcd.setCursor(OPTIONS_OFFSET, line);
      lcd.print(name);

      // aligning score to the right
      if (score > 999) {
        lcd.setCursor(11, line);
      } else if (score > 99) {
        lcd.setCursor(12, line);
      } else if (score > 9) {
        lcd.setCursor(13, line);
      } else {
        lcd.setCursor(14, line);
      }
      lcd.print(score);
    }

    // showing the highscore board
    void showHighscoreBoard(String* names, int* scores) {
      // shwoing the current 2 highscores
      if (highscoreBoardCursor == HIGHSCORE_TOP) {
        // the cursor is on the "back" option
        // => showing the last highscore + the "back" option
        showHighscore(names[highscoreBoardCursor - 1], scores[highscoreBoardCursor - 1], 0);

        lcd.setCursor(OPTIONS_OFFSET, 1);
        lcd.print(BACK_BUTTON);

        // showing where the cursor is currently
        showCursor(0, 1);

      } else  if (highscoreBoardCursor == HIGHSCORE_TOP - 1) {
        // the cursor is on the last highscore
        // => showing showing the last highscore + the "back" option
        showHighscore(names[highscoreBoardCursor], scores[highscoreBoardCursor], 0);

        lcd.setCursor(OPTIONS_OFFSET, 1);
        lcd.print(BACK_BUTTON);

        // showing where the cursor is currently
        showCursor(0, 0);

      } else {
        // showing the highscore the cursor is pointing at + the next one
        showHighscore(names[highscoreBoardCursor], scores[highscoreBoardCursor], 0);
        showHighscore(names[highscoreBoardCursor + 1], scores[highscoreBoardCursor + 1], 1);

        // showing where the cursor is currently
        showCursor(0, 0);
      }

      showNavigationArrows();
    }


    // --- SETTINGS MENU ---
    // getting the position of the principal menu cursor
    int getSettingsMenuCursor() {
      return settingsMenuCursor;
    }

    // showing the settings menu
    void showSettingsMenu() {
      showMenu(settingsMenu, settingsMenuCursor, SETTINGS_MENU_ITEMS);
    }

    // setting the position of the settingsmenu cursor
    void setSettingsMenuCursor(int position) {
      settingsMenuCursor = position;
      setArrows(settingsMenuCursor, SETTINGS_MENU_ITEMS);
    }


    // --- START LEVEL SETTINGS SECTION ---
    // showing the starting level settings section
    void showStartLevelSettings(int startLevel) {
      // showing current position
      showCursor(0, 0);
      lcd.setCursor(OPTIONS_OFFSET, 0);
      lcd.print("Start level");

      // showing starting level value
      lcd.setCursor(SETTINGS_OFFSET, 1);
      lcd.print(startLevel);
    }


    // --- CONTRAST SETTINGS SECTION ---
    // setting contrast value
    void showContrast() {
      analogWrite(CONTRAST_PIN, contrast);
    }

    // changing contrast value
    void setContrast(int value) {
      contrast = value;
      EEPROM.update(CONTRAST_ADDRESS, value);
      showContrast();
    }

    // getting contrast value
    int getContrast() {
      return contrast;
    }

    // showing the scontrast settings section
    void showContrastSettings() {
      // showing current position
      showCursor(0, 0);
      lcd.setCursor(OPTIONS_OFFSET, 0);
      lcd.print("Contrast");

      // showing contrast value
      lcd.setCursor(SETTINGS_OFFSET, 1);
      lcd.print(contrast);
    }


    // --- BRIGHTNESS SETTINGS SECTION ---
    // setting brightness value
    void showBrightness() {
      analogWrite(BRIGHTNESS_PIN, brightness);
    }

    // changing brightness value
    void setBrightness(int value) {
      brightness = value;
      EEPROM.update(BRIGHTNESS_ADDRESS, value);
      showBrightness();
    }

    // getting brightness value
    int getBrightness() {
      return brightness;
    }

    // showing the brightness settings section
    void showBrightnessSettings() {
      // showing current position
      showCursor(0, 0);
      lcd.setCursor(OPTIONS_OFFSET, 0);
      lcd.print("Brightness");

      // showing contrast value
      lcd.setCursor(SETTINGS_OFFSET, 1);
      lcd.print(brightness);
    }


    // --- INTENSITY SETTINGS SECTION ---
    // showing the intensity settings section
    void showIntensitySettings(int intensity) {
      // showing current position
      showCursor(0, 0);
      lcd.setCursor(OPTIONS_OFFSET, 0);
      lcd.print("Intensity");

      // showing starting level value
      lcd.setCursor(SETTINGS_OFFSET, 1);
      lcd.print(intensity);
    }


    // --- ABOUT SECTION ---
    // getting the position of the about section cursor
    int getAboutSectionCursor() {
      return aboutSectionCursor;
    }

    // setting the position of the about cursor
    void setAboutSectionCursor(int position) {
      aboutSectionCursor = position;
      setArrows(aboutSectionCursor, ABOUT_SECTION_ITEMS);
    }

    // showing the about section
    void showAboutSection() {
//      showMenu(aboutSection, aboutSectionCursor, ABOUT_SECTION_ITEMS);

      if (aboutSectionCursor != ABOUT_SECTION_ITEMS - 1) {
        if (aboutSectionCursor == NAME_POSITION) {
          showMenuOption(OPTIONS_OFFSET, 0, NAME);
          showMenuOption(OPTIONS_OFFSET, 1, CREATOR);
        } else if (aboutSectionCursor == CREATOR_POSITION) {
          showMenuOption(OPTIONS_OFFSET, 0, CREATOR);
          showMenuOption(OPTIONS_OFFSET, 1, GITHUB);
        } else if (aboutSectionCursor == GUTHUB_POSITION) {
          showMenuOption(OPTIONS_OFFSET, 0, GITHUB);
          showMenuOption(OPTIONS_OFFSET, 1, BACK_BUTTON);
        } 

        // showing where the cursor is currently
        showCursor(0, 0);
      } else {
        showMenuOption(OPTIONS_OFFSET, 0, GITHUB);
        showMenuOption(OPTIONS_OFFSET, 1, BACK_BUTTON);

        // showing where the cursor is currently
        showCursor(0, 1);
      }

      showNavigationArrows();
      scrollPosition = DISPLAY_COLUMNS + 1;
    }

    // scrolling the current about item
    void scrollAboutItem(String item) {
      int startPosition = scrollPosition - DISPLAY_COLUMNS;

        lcd.setCursor(OPTIONS_OFFSET, 0);
        lcd.print(item.substring(startPosition, startPosition + MENU_ITEM_LENGTH));

        if (item.length() - startPosition < MENU_ITEM_LENGTH) {
          // the end of the text has been shown on the display
          // => showing the beginning of the text as well
          lcd.print(item.substring(0, MENU_ITEM_LENGTH - (item.length() - startPosition)));
        }

        if (startPosition < item.length() - 1) {
          scrollPosition++;
        } else {
          scrollPosition = DISPLAY_COLUMNS;
        }
    }

    // scrolling the current line in the about section
    void scrollCurrentAboutSection() {
      if (aboutSectionCursor != ABOUT_SECTION_ITEMS - 1) {
        if (aboutSectionCursor == NAME_POSITION) {
          scrollAboutItem(NAME);
        } else if (aboutSectionCursor == CREATOR_POSITION) {
          scrollAboutItem(CREATOR);
        } else if (aboutSectionCursor == GUTHUB_POSITION) {
          scrollAboutItem(GITHUB);
        } 
        
        showNavigationArrows();
      }
    }
};
