#include "game.h"

Game game = Game();

void setup() {
  Serial.begin(9600);

  game.intro();
  game.showPrincipalMenu();
}

void loop() {
  int systemState = game.getSystemState();
  
  // each system state has a different flow
  if (systemState == PRINCIPAL_MENU_STATE) {
    // in principal menu
    game.navigatePrincipalMenu();
  } else if (systemState == TRANSITION_FROM_PRINCIPAL_MENU_STATE) {
    // in transition from principal menu
    game.changeFromPrincipalMenu();
  } else if (systemState == IN_GAME_STATE) {
    // in game
    game.playGame();
  } else if (systemState == HIGHSCORE_BOARD_STATE) {
    // in highscore board
    game.navigateHighscoreBoard();
  } else if (systemState == SETTINGS_MENU_STATE) {
    // in settings menu
    game.navigateSettingsMenu();
  } else if (systemState == ABOUT_STATE) {
    // in about section
    game.navigateAbout();
  } else if (systemState == TRANSITION_FROM_HIGHSCORE_BOARD_STATE) {
    // in transition from principal menu
    game.changeFromHighscoreBoard();
  } else if (systemState == START_LEVEL_SETTINGS_STATE) {
    // in starting level settings
    game.navigateStartLevelValues();
  } else if (systemState == CONTRAST_SETTINGS_STATE) {
    // in contrast settings
    game.navigateContrastValues();
  } else if (systemState == BRIGHTNESS_SETTINGS_STATE) {
    // in brightness settings
    game.navigateBrightnessValues();
  } else if (systemState == INTENSITY_SETTINGS_STATE) {
    // in brightness settings
    game.navigateIntensityValues();
  } else if (systemState == TRANSITION_FROM_SETTINGS_MENU_STATE) {
    // in transition from settings menu
    game.changeFromSettingsMenu();
  } else if (systemState == TRANSITION_FROM_SETTINGS_SECTION_STATE) {
    // in transition from a section within the settings menu
    game.changeFromSettingsSection();
  } else if (systemState == TRANSITION_FROM_ABOUT_STATE) {
    // in transition from about section
    game.changeFromAbout();
  }
}
