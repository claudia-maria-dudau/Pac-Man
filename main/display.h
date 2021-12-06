#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "system_constants.h"

class Display {
  // lcd display pins
  const int RS = 13;
  const int enable = 8;
  const int d4 = 7;
  const int d5 = 6;
  const int d6 = 5;
  const int d7 = 4;
  const int contrastPin = 3;
  const int brightnessPin = 9;

  // display settings
  int contrast;
  const int contrastAddress = 0;
  int brightness;
  const int brightnessAddress = 1;

  const int minSetting = 0;
  const int maxSetting = 255;
  const int contrastStep = 5;
  const int brightnessStep = 10;

  // display
  LiquidCrystal lcd = LiquidCrystal(RS, enable, d4, d5, d6, d7);

  static const int displayColumns = 16;

  // dog character
  static const int lines = 8;
  byte dogChar[lines] = {
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
  byte downArrowChar[lines] = {
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
  static const int principalMenuItems = 4;
  String principalMenu[principalMenuItems] = {
    "Start game",
    "Highscore",
    "Settings",
    "About"
  };
  int principalMenuCursor = 0;

  // highscore board
  int highscoreBoardCursor = 0;

  // settings menu
  static const int settingsMenuItems = 5;
  String settingsMenu[settingsMenuItems] = {
    "Start level",
    "Contrast",
    "Brightness",
    "Intensity",
    "Back"
  };
  int settingsMenuCursor = 0;

  // about section
  static const int aboutItems = 4;
  String about[aboutItems] = {
    "Name: Marinel's epic quest for food   ",
    "Creator: Claudia Dudau   ",
    "Github: ...   ",
    "Back"
  };
  int aboutCursor = 0;
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
      lcd.setCursor(displayColumns - 1, 0);
      lcd.write(0b01011110);
    }

    if (downArrow) {
      // showing down arrow
      lcd.setCursor(displayColumns - 1, 1);
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
      lcd.print(menu[cursorPosition - 1]);
      lcd.setCursor(2, 1);
      lcd.print(menu[cursorPosition]);

      // showing where the cursor is currently
      showCursor(1);

    } else {
      // showing the item the cursor is pointing at + the next one
      lcd.setCursor(2, 0);
      lcd.print(menu[cursorPosition]);
      lcd.setCursor(2, 1);
      lcd.print(menu[cursorPosition + 1]);

      // showing where the cursor is currently
      showCursor(0);
    }

    showNavigationArrows();
  }

public:
  // display initialization
  Display() {
    // contrast + brightness pins are in OUTPUT mode
    pinMode(contrastPin, OUTPUT);
    pinMode(brightnessPin, OUTPUT);

    // loading values from memory
    contrast = EEPROM.read(contrastAddress);
    brightness = EEPROM.read(brightnessAddress);
    
    lcd.begin(16, 2);

    showContrast();
    showBrightness();

    // creating custom characters
    lcd.createChar(0, dogChar);
    lcd.createChar(1, downArrowChar);
  }

  // getting minimum value for the display settings
  int getMinSetting() {
    return minSetting;
  }

  // getting maximum value fot the display settings
  int getMaxSetting() {
    return maxSetting;
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

    for (int i = 0; i < displayColumns; i++) {
      lcd.setCursor(i, i % 2);
      lcd.write((byte)0);
      delay(300);
    }
  }


  // --- PRINCIPAL MENU ---
  // getting the number of items of the principal menu
  static int getPrincipalMenuItems() {
    return principalMenuItems;
  }

  // getting the position of the principal menu cursor
  int getPrincipalMenuCursor() {
    return principalMenuCursor;
  }

  // setting the position of the principal menu cursor
  void setPrincipalMenuCursor(int position) {
    principalMenuCursor = position;
    setArrows(principalMenuCursor, principalMenuItems);
  }

  // showing the principal game menu
  void showPrincipalMenu() {
    showMenu(principalMenu, principalMenuCursor, principalMenuItems);
  }

  // --- IN GAME DISPLAY ---
  // showing the in game display
  void showGameDisplay(int level, int lives, int score) {
    // showing the level of the game
    lcd.setCursor(0, 0);
    lcd.print("Level: " + String(level));

    // showing the number of lives of the player
    for (int i = 0; i < lives; i++) {
      lcd.setCursor(displayColumns - 1 - i, 0);
      lcd.write((byte)0);
    }

    // showing the current score of the player
    lcd.setCursor(0,1);
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
  // getting the number of items of the principal menu
  static int getSettingsMenuItems() {
    return settingsMenuItems;
  }

  // getting the position of the principal menu cursor
  int getSettingsMenuCursor() {
    return settingsMenuCursor;
  }
  
  // showing the settings menu
  void showSettingsMenu() {
    showMenu(settingsMenu, settingsMenuCursor, settingsMenuItems);
  }

  // setting the position of the settingsmenu cursor
  void setSettingsMenuCursor(int position) {
    settingsMenuCursor = position;
    setArrows(settingsMenuCursor, settingsMenuItems);
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
    analogWrite(contrastPin, contrast);
  }

  // changing contrast value
  void setContrast(int value) {
    contrast = value;
    EEPROM.update(contrastAddress, value);
    showContrast();
  }

  // getting contrast value
  int getContrast() {
    return contrast;
  }

  // getting contrast step
  int getContrastStep() {
    return contrastStep;
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
    analogWrite(brightnessPin, brightness);
  }

  // changing brightness value
  void setBrightness(int value) {
    brightness = value;
    EEPROM.update(brightnessAddress, value);
    showBrightness();
  }

  // getting brightness value
  int getBrightness() {
    return brightness;
  }  

  // getting brightness step
  int getBrightnessStep() {
    return brightnessStep;
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
  // getting the number of items of the about section
  static int getAboutItems() {
    return aboutItems;
  }

  // getting the position of the about section cursor
  int getAboutCursor() {
    return aboutCursor;
  }
  
  // setting the position of the about cursor
  void setAboutCursor(int position) {
    aboutCursor = position;
    setArrows(aboutCursor, aboutItems);
  }

  // showing the about section
  void showAbout() {
    showMenu(about, aboutCursor, aboutItems);
    scrollPosition = displayColumns + 1;
  }

  // scrolling the current line in the about section
  void scrollCurrentAbout() {
    if (aboutCursor != aboutItems - 1) {
      int startPosition = scrollPosition - displayColumns;
      
      lcd.setCursor(2, 0);
      lcd.print(about[aboutCursor].substring(startPosition));
      lcd.print(about[aboutCursor].substring(0, startPosition));
      
      if (startPosition < about[aboutCursor].length() - 1) {
        scrollPosition++;
      } else {
        scrollPosition = displayColumns;
      }

      showNavigationArrows();
    }
  }
};
