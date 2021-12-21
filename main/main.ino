#include "game.h"

Game game = Game();

void setup() {
}

void loop() {
  int systemState = game.getSystemState();

  // each system state has a different flow
  if (systemState == INTRO_STATE) {
    // in intro
    game.intro();
  } else if (systemState == TRANSITION_FROM_INTRO_STATE){
    // in transition from intro
    game.changeFromIntro();
  } else if (systemState == PRINCIPAL_MENU_STATE) {
    // in principal menu
    game.navigatePrincipalMenu();
  } else if (systemState == TRANSITION_FROM_PRINCIPAL_MENU_STATE) {
    // in transition from principal menu
    game.changeFromPrincipalMenu();
  } else if (systemState == START_GAME_STATE) {
    // in start game
    game.startGame();
  } else if (systemState == IN_GAME_STATE) {
    // in game
    game.playGame();
  } else if (systemState == TRANSITION_TO_PAUSE_GAME_STATE) {
    // changing from in game to pause game
    game.changeToPause();
  } else if (systemState == PAUSE_GAME_STATE) {
    // in pause game
    game.navigatePauseGameMenu();
  } else if (systemState == TRANSITION_FROM_PAUSE_GAME_STATE) {
    // in transition from pause game menu
    game.changeFromPauseGameMenu();
  } else if (systemState == LOST_GAME_STATE) {
    // in lost game
    game.lostGame();
  } else if (systemState == DONE_LEVEL_STATE) {
    // in done level
    game.doneLevel();
  } else if (systemState == WON_GAME_STATE) {
    // in won game
    game.wonGame();
  } else if (systemState == STATISTICS_STATE) {
    // in statistics
    game.statistics();
  } else if (systemState == BEAT_HIGHSCORE_STATE) {
    // in beat highscore
    game.beatHighscore();
  } else if (systemState == TRANSITION_FROM_END_SECTIONS_STATE) {
    // in transition from end sections
    game.changeFromEndSections();
  } else if (systemState == ENTER_NAME_STATE) {
    // in enter name menu
    game.navigateEnterNameMenu();
  } else if (systemState == TRANSITION_FROM_ENTER_NAME_STATE) {
    // in transition from enter name menu
    game.changeFromEnterNameMenu();
  } else if (systemState == END_GAME_STATE) {
    // in end game
    game.navigateEndGameMenu();
  } else if (systemState == TRANSITION_FROM_END_GAME_STATE) {
    // in transition form end game menu
    game.changeFromEndGameMenu();
  } else if (systemState == HIGHSCORE_BOARD_STATE) {
    // in highscore board
    game.navigateHighscoreBoard();
  } else if (systemState == SETTINGS_MENU_STATE) {
    // in settings menu
    game.navigateSettingsMenu();
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
  } else if (systemState == SOUND_SETTINGS_STATE) {
    // in sound
    game.navigateSoundValues();
  } else if (systemState == RESET_HIGHSCORE_SETTINGS_STATE) {
    // in reset highscore
    game.navigateResetHighscoreMenu();
  } else if (systemState == TRANSITION_FROM_RESET_HIGHSCORE_MENU_STATE) {
    // in transition from reset highscore
    game.changeFromResetHighscoreMenu();
  } else if (systemState == TRANSITION_FROM_SETTINGS_MENU_STATE) {
    // in transition from settings menu
    game.changeFromSettingsMenu();
  } else if (systemState == TRANSITION_FROM_SETTINGS_SECTION_STATE) {
    // in transition from a section within the settings menu
    game.changeFromSettingsSection();
  } else if (systemState == ABOUT_STATE) {
    // in about section
    game.navigateAbout();
  } else if (systemState == TRANSITION_FROM_ABOUT_STATE) {
    // in transition from about section
    game.changeFromAbout();
  }
}
