// --- PINS ---
// lcd display pins
#define RS 13
#define ENABLE 8
#define D4 7
#define D5 5
#define D6 4
#define D7 3
#define CONTRAST_PIN 6
#define BRIGHTNESS_PIN 9

// joystick pins
#define X_PIN A0
#define Y_PIN A1
#define SW_PIN 2

// shift register pins for matrix
#define DIN_PIN 12
#define CLOCK_PIN 1
#define LOAD_PIN 10

// buzzer pins
#define BUZZER_PIN 11


// --- SYSTEM STATES ---
#define INTRO_STATE 0
#define TRANSITION_FROM_INTRO_STATE 1
#define PRINCIPAL_MENU_STATE 2
#define TRANSITION_FROM_PRINCIPAL_MENU_STATE 3
#define START_GAME_STATE 4
#define IN_GAME_STATE 5
#define TRANSITION_TO_PAUSE_GAME_STATE 6
#define PAUSE_GAME_STATE 7
#define TRANSITION_FROM_PAUSE_GAME_STATE 8
#define LOST_GAME_STATE 9
#define DONE_LEVEL_STATE 10
#define WON_GAME_STATE 11
#define STATISTICS_STATE 12
#define BEAT_HIGHSCORE_STATE 13
#define TRANSITION_FROM_END_SECTIONS_STATE 14
#define ENTER_NAME_STATE 15
#define TRANSITION_FROM_ENTER_NAME_STATE 16
#define END_GAME_STATE 17
#define TRANSITION_FROM_END_GAME_STATE 18
#define HIGHSCORE_BOARD_STATE 19
#define TRANSITION_FROM_HIGHSCORE_BOARD_STATE 20
#define SETTINGS_MENU_STATE 21
#define START_LEVEL_SETTINGS_STATE 22
#define CONTRAST_SETTINGS_STATE 23
#define BRIGHTNESS_SETTINGS_STATE 24
#define INTENSITY_SETTINGS_STATE 25
#define SOUND_SETTINGS_STATE 26
#define TRANSITION_FROM_SETTINGS_SECTION_STATE 27
#define RESET_HIGHSCORE_SETTINGS_STATE 28
#define TRANSITION_FROM_RESET_HIGHSCORE_MENU_STATE 29
#define TRANSITION_FROM_SETTINGS_MENU_STATE 30
#define ABOUT_STATE 31
#define TRANSITION_FROM_ABOUT_STATE 32


// --- DISPLAY CONSTANTS ---
#define DISPLAY_COLUMNS 16
#define DISPLAY_LINES 2
#define MENU_ITEM_LENGTH DISPLAY_COLUMNS - 3
#define LINES 8


// --- JOYSTICK CONSTANTS ---
#define DEBOUNCE_INTERVAL 200

// joystick thresholds
#define MIN_THRESHOLD 200
#define MAX_THRESHOLD 600


// --- MATRIX CONSTANTS ---
#define MATRIX_SIZE 8
#define HALF_MATRIX_SIZE (int)(MATRIX_SIZE / 2)
#define ANIMATION_INTERVAL 150


// --- BUZZER CONSTANTS ---
#define SONG_TEMPO 105
#define WHOLENOTE (60000 * 4) / SONG_TEMPO
#define BETWEEN_NOTES_INTERVAL 100


// --- INTRO ---
#define INTRO_SECTIONS_INTERVAL 4000
#define INTRO_ANIMATION_INTERVAL 420
#define TEXT_SECTION_1 1
#define TEXT_SECTION_2 2
#define TEXT_SECTION_3 3
#define TEXT_SECTION_4 4
#define ANIMATION_SECTION 5

// intro text
#define INTRO_TEXT_1 "Once upon a time"
#define INTRO_TEXT_2 "lived Marinel"
#define INTRO_TEXT_3 "A dog with a big"
#define INTRO_TEXT_4 "passion for food"
#define INTRO_TEXT_5 "But in order"
#define INTRO_TEXT_6 "to get it"
#define INTRO_TEXT_7 "He had to travel"
#define INTRO_TEXT_8 "uncharted ground"


// --- PRINCIPAL MENU ---
#define PRINCIPAL_MENU_ITEMS 4
#define OPTIONS_OFFSET 2

// principal menu items' corresponding positions
#define START_GAME_POSITION 0
#define HIGHSCORE_POSITION 1
#define SETTINGS_POSITION 2
#define ABOUT_POSITION 3

// principal menu items
#define START_GAME "Start Game"
#define HIGHSCORE "Highscore"
#define SETTINGS "Settings"
#define ABOUT "About"


// --- GAME ---
// level
#define MIN_LEVEL 1
#define MAX_LEVEL 9
#define LEVEL_STEP 1
#define BETWEEN_LEVELS_INTERVAL ANIMATION_INTERVAL * MATRIX_SIZE

// lives
#define MAX_LIVES 3
#define LIFE_SCORE 1

// player
#define PLAYER_BLINKING_INTERVAL 100
#define PLAYER_SPEED 200

// map constants
#define MAP_STEP 4
#define MAPS 3
#define MIN_MAP_SIZE 8
#define MAX_MAP_SIZE MIN_MAP_SIZE + MAP_STEP * (MAPS - 1)
#define LEVEL_MAP_STEP (int)((MAX_LEVEL - 1) / MAPS + 1)

// enemies
#define ENEMIES_STEP 2
#define MAX_ENEMIES (int)((MAX_LEVEL - 1) / LEVEL_MAP_STEP * ENEMIES_STEP + 1)
#define DIST_FROM_INITIAL_PLAYER_POSITION 3
#define ENEMIES_SPEED_STEP 100
#define ENEMIES_MAX_SPEED 400
#define ENEMIES_MIN_SPEED ENEMIES_MAX_SPEED + MAX_LEVEL * ENEMIES_SPEED_STEP
#define ENEMIES_BLINKING_INTERVAL 200
#define MOVES 8
#define POSSIBLE_STEPS 3

// food items
#define FOOD_ITEMS_STEP 3
#define MAX_FOOD_ITEMS MAX_LEVEL * FOOD_ITEMS_STEP

// timers between states
#define BETWEEN_END_SECTIONS 5000


// --- START GAME DISPLAY ---
#define START_GAME_TEXT_1 "OKAY,"
#define START_GAME_TEXT_2 "LET'S GO!"


// --- IN GAME DISPLAY ---
#define LEVEL "Level:"
#define SCORE "Score:"


// --- PAUSE GAME MENU ---
#define PAUSE_GAME_MENU_ITEMS 2
#define BETWEEN_OPTIONS_OFFSET 10
#define PAUSE_GAME_TITLE "PAUSE"

// pause game menu items
#define RESUME_BUTTON "Resume"
#define EXIT_BUTTON "Exit"


// --- LOST GAME DISPLAY ---
#define LOST_GAME_TEXT_1 "You lost! :("
#define LOST_GAME_TEXT_2 "Sad Marinel bark"


// --- WON GAME DISPLAY ---
#define WON_GAME_TEXT_1 "CONGRATULATIONS!"
#define WON_GAME_TEXT_2 "You won the game"


// --- BEAT HIGHSCORE DISPLAYS ---
#define BEAT_HIGHSCORE_DISPLAY 0
#define ENTER_NAME_TEXT_DISPLAY 1
#define NAME_RESTRICTIONS_DISPLAY 2
#define ENTER_NAME_DISPLAY 3

// text
#define BEAT_HIGHSCORE_TEXT_1 "You beat the"
#define BEAT_HIGHSCORE_TEXT_2 "HIGHSCORE!"
#define ENTER_NAME_TEXT_1 "Please enter"
#define ENTER_NAME_TEXT_2 "your name"
#define NAME_RESTRICTIONS_TEXT_1 "Name must have"
#define NAME_RESTRICTIONS_TEXT_2 "max " + String(MAX_NAME_LENGTH) + " charact."


// --- ENTER NAME MENU ---
#define KEYBOARD_LINES 3
#define KEYBOARD_COLUMNS 13
#define KEYBOARD_START_COLUMN 2
#define ENTER_NAME_MENU_OPTIONS 2
#define ENTER_NAME_MENU_ITEMS KEYBOARD_LINES + ENTER_NAME_MENU_OPTIONS
#define NAME_TEXT "Name:"

// keyboard
#define KEYBOARD_LINE_1 "abcdefghijklm"
#define KEYBOARD_LINE_2 "nopqrstuvwxyz"
#define KEYBOARD_LINE_3 "1234567890-_$"

// enter menu items
#define DELETE_BUTTON "Delete"
#define DONE_BUTTON "Done"


// --- END GAME MENU ---
#define END_GAME_MENU_ITEMS 2
#define END_GAME_TITLE "END GAME"

// end game menu items
#define RESTART_BUTTON "Restart"


// --- HIGHSCORE BOARD ---
#define HIGHSCORE_TOP 5
#define MAX_NAME_LENGTH 8
#define BACK_BUTTON "Back"
#define MIN_HIGHSCORE -4
#define DEFAULT_NAME "???"
#define SCORE_OFFSET 14


// --- SETTINGS MENU ---
#define SETTINGS_MENU_ITEMS 7
#define SETTINGS_OFFSET 4

// setttings menu items' corresponding positions
#define START_LEVEL_POSITION 0
#define CONTRAST_POSITION 1
#define BRIGHTNESS_POSITION 2
#define INTENSITY_POSITION 3
#define SOUND_POSITION 4
#define RESET_HIGHSCORE_POSITION 5
#define BACK_POSITION 6

// settings menu items
#define START_LEVEL "Start level"
#define CONTRAST "Contrast"
#define BRIGHTNESS "Brightness"
#define INTENSITY "Intensity"
#define SOUND "Sound"
#define RESET_HIGHSCORE "Res Highscore"

// settings limits
// contrast
#define MIN_CONTRAST 10
#define MAX_CONTRAST 60
#define CONTRAST_STEP 5

// brightness
#define MIN_BRIGHTNESS 0
#define MAX_BRIGHTNESS 255
#define BRIGHTNESS_STEP 10

// intensity
#define MIN_INTENSITY 0
#define MAX_INTENSITY 15
#define INTENSITY_STEP 1

// sound
#define MIN_SOUND 0
#define MAX_SOUND 1
#define SOUND_STEP 1
#define SOUND_ON "On"
#define SOUND_OFF "Off"

// reset highscore
#define RESET_HIGHSCORE_MENU_ITEMS 2
#define NO_BUTTON "No"
#define YES_BUTTON "Yes"


// --- ABOUT SECTION ---
#define ABOUT_SECTION_ITEMS 4
#define SCROLL_ABOUT_SECTION_INTERVAL 500

// about items
#define NAME "Name: Marinel's epic quest for food   "
#define CREATOR "Creator: Claudia Dudau   "
#define GITHUB "Github: https://github.com/claudia-maria-dudau/Pac-Man   "


// --- MEMORY ADDRESSES ---
// settings
#define CONTRAST_ADDRESS 0
#define BRIGHTNESS_ADDRESS 1
#define INTENSITY_ADDRESS 2

// highscore
#define START_NAMES_ADDRESS 3
#define START_SCORES_ADDRESS START_NAMES_ADDRESS + HIGHSCORE_TOP * 2


// --- FUNCTIONS ---
#define sgn(x) ((x) < 0 ? -1 : ((x) > 0 ? 1 : 0))
