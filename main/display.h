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
    byte dogChar[LINES] = {
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
    byte downArrowChar[LINES] = {
      B00000,
      B00000,
      B00000,
      B00000,
      B00000,
      B10001,
      B01010,
      B00100
    };

    // principal menu
    String principalMenu[PRINCIPAL_MENU_ITEMS] = {
      "Start game",
      "Highscore",
      "Settings",
      "About"
    };
    int principalMenuCursor = 0;

    // highscore board
    int highscoreBoardCursor = 0;

    // settings menu
    String settingsMenu[SETTINGS_MENU_ITEMS] = {
      "Start level",
      "Contrast",
      "Brightness",
      "Intensity",
      "Back"
    };
    int settingsMenuCursor = 0;

    // about section
    String aboutSection[ABOUT_SECTION_ITEMS] = {
      "Name: Marinel's epic quest for food   ",
      "Creator: Claudia Dudau   ",
      "Github: https://github.com/claudia-maria-dudau/Pac-Man   ",
      "Back"
    };
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

    // showing the cursor at the given line
    void showCursor(int line) {
      lcd.setCursor(0, line);
      lcd.write(0b00111110);
    }

    // showing the given menu
    void showMenu(String* menu, int cursorPosition, int noItems) {
      // shwoing the current 2 menu items
      if (cursorPosition == noItems - 1) {
        // the cursor is on the last item of the menu
        // => showing the last 2 items
        lcd.setCursor(2, 0);
        lcd.print(menu[cursorPosition - 1].substring(0, MENU_ITEM_LENGTH));
        lcd.setCursor(2, 1);
        lcd.print(menu[cursorPosition].substring(0, MENU_ITEM_LENGTH));

        // showing where the cursor is currently
        showCursor(1);

      } else {
        // showing the item the cursor is pointing at + the next one
        lcd.setCursor(2, 0);
        lcd.print(menu[cursorPosition].substring(0, MENU_ITEM_LENGTH));
        lcd.setCursor(2, 1);
        lcd.print(menu[cursorPosition + 1].substring(0, MENU_ITEM_LENGTH));

        // showing where the cursor is currently
        showCursor(0);
      }

      showNavigationArrows();
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
    }

    // claering the display
    void clear() {
      lcd.clear();
    }


    // --- INTRO ---
    // showing text at the start of the game
    void startText() {
      lcd.setCursor(0, 0);
      lcd.print("Once upon a time");

      delay(4000);
      lcd.clear();

      lcd.setCursor(2, 0);
      lcd.print("In a galaxy");
      lcd.setCursor(2, 1);
      lcd.print("far away...");

      delay(4000);
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("There was a dog");
      lcd.setCursor(1, 1);
      lcd.print("named Marinel");

      delay(4000);
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("Who had a great");
      lcd.setCursor(1, 1);
      lcd.print("love for food");

      delay(4000);
      lcd.clear();

      lcd.setCursor(2, 0);
      lcd.print("But in order");
      lcd.setCursor(3, 1);
      lcd.print("to get it");

      delay(4000);
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("He had to travel");
      lcd.setCursor(0, 1);
      lcd.print("uncharted ground");

      delay(4000);
      lcd.clear();

      for (int i = 0; i < DISPLAY_COLUMNS; i++) {
        lcd.setCursor(i, i % 2);
        lcd.write((byte)0);
        delay(300);
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
    // showing the in game display
    void showGameDisplay(int level, int lives, int score) {
      // showing the level of the game
      lcd.setCursor(0, 0);
      lcd.print("Level: " + String(level));

      // showing the number of lives of the player
      for (int i = 0; i < lives; i++) {
        lcd.setCursor(DISPLAY_COLUMNS - 1 - i, 0);
        lcd.write((byte)0);
      }

      // showing the current score of the player
      lcd.setCursor(0, 1);
      lcd.print("Score: " + String(score));
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
      lcd.setCursor(2, line);
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

        lcd.setCursor(2, 1);
        lcd.print("Back");

        // showing where the cursor is currently
        showCursor(1);

      } else  if (highscoreBoardCursor == HIGHSCORE_TOP - 1) {
        // the cursor is on the last highscore
        // => showing showing the last highscore + the "back" option
        showHighscore(names[highscoreBoardCursor], scores[highscoreBoardCursor], 0);

        lcd.setCursor(2, 1);
        lcd.print("Back");

        // showing where the cursor is currently
        showCursor(0);

      } else {
        // showing the highscore the cursor is pointing at + the next one
        showHighscore(names[highscoreBoardCursor], scores[highscoreBoardCursor], 0);
        showHighscore(names[highscoreBoardCursor + 1], scores[highscoreBoardCursor + 1], 1);

        // showing where the cursor is currently
        showCursor(0);
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
      lcd.setCursor(0, 0);
      lcd.write(0b00111110);
      lcd.setCursor(2, 0);
      lcd.print("Start level");

      // showing starting level value
      lcd.setCursor(4, 1);
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
      lcd.setCursor(0, 0);
      lcd.write(0b00111110);
      lcd.setCursor(2, 0);
      lcd.print("Contrast");

      // showing contrast value
      lcd.setCursor(4, 1);
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
      lcd.setCursor(0, 0);
      lcd.write(0b00111110);
      lcd.setCursor(2, 0);
      lcd.print("Brightness");

      // showing contrast value
      lcd.setCursor(4, 1);
      lcd.print(brightness);
    }


    // --- INTENSITY SETTINGS SECTION ---
    // showing the intensity settings section
    void showIntensitySettings(int intensity) {
      // showing current position
      lcd.setCursor(0, 0);
      lcd.write(0b00111110);
      lcd.setCursor(2, 0);
      lcd.print("Intensity");

      // showing starting level value
      lcd.setCursor(4, 1);
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
      showMenu(aboutSection, aboutSectionCursor, ABOUT_SECTION_ITEMS);
      scrollPosition = DISPLAY_COLUMNS + 1;
    }

    // scrolling the current line in the about section
    void scrollCurrentAboutSection() {
      if (aboutSectionCursor != ABOUT_SECTION_ITEMS - 1) {
        int startPosition = scrollPosition - DISPLAY_COLUMNS;

        lcd.setCursor(2, 0);
        lcd.print(aboutSection[aboutSectionCursor].substring(startPosition, startPosition + MENU_ITEM_LENGTH));

        if (aboutSection[aboutSectionCursor].length() - startPosition < MENU_ITEM_LENGTH) {
          // the end of the text has been shown on the display
          // => showing the beginning of the text as well
          lcd.print(aboutSection[aboutSectionCursor].substring(0, MENU_ITEM_LENGTH - (aboutSection[aboutSectionCursor].length() - startPosition)));
        }

        if (startPosition < aboutSection[aboutSectionCursor].length() - 1) {
          scrollPosition++;
        } else {
          scrollPosition = DISPLAY_COLUMNS;
        }

        showNavigationArrows();
      }
    }
};
