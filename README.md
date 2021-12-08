# Marinel's epic quest for food
 A take on the classic PAC_MAN game using the ARDUINO UNO board, a 8x8 matrix, a lcd 16x2 display, a joystick and a buzzer.
 
## Game Description
 The goal of the player is to gather all the food items from the map, without crossing paths with the enemies. The player has 3 lives which do not reset once a level is complete. The game is finished when the player either loses all of his lives, or has successfully completed all the levels.
 
## Requirements
### Menu Requirments
 The menu is shown on the lcd display and the joystick is used to scroll through it. On power up, the intro of the game is shown, followed by the principal menu of the game.
 
#### Principal Menu
 Sections:
  - Start Game => starts the game at the initial level set by the player
  - Highscore => shows highscore board 
  - Settings => goes into the settings menu of the game
  - About => shows details about the game

 Motions:
  - scrolling through the options is accomplished using the X axis of the joystick
  - accesing one of the options is done using the button of the joystick
 
 #### Highscore
 Characteristics:
  - accessed via click from the principal menu
  - the top highscores update after every game end if the score of the player is higher than at least the last highscore in the top
  - the top highscores (player name + score) are stored in the EEPROM memory of the ARDUINO UNO
  
 Sections:
  - Top 5 Highscores => player name + score shown in descending order
  - Back => goes back to the principal menu

 Motions:
  - scrolling through the values is accomplished using the X axis of the joystick
  - a button press has effect only when pressed on the "Back" option
  
#### Settings Menu
 Characteristics:
  - accessed via click from the principal menu
  - each settings has its own specific step with which the value is modified
  
 Sections:  
  - Start Level => sets the initial level from which the game to start playing (step: 1)
  - Contrast => sets the contrast value for the lcd display (step: 5)
  - Brightness => sets the brightness value for the lcd display (step: 10)
  - Intensity => sets the intensity of the matrix's leds (step: 1)
  - Back => goes back to the principal menu

 Motions:
  - scrolling through the options is accomplished using the X axis of the joystick
  - scroolling through the values for each setting is accomplished using the Y axis of the joystick
  - accesing one of the options is done using the button of the joystick

#### About
 Characteristics:
  - accessed via click from the principal menu
  - each section starts scrolling once its selected

 Sections:
  - Game Name
  - Creator Name
  - Github => github link of the game's code
  - Back => goes back to the principal menu
 
 Motions:
  - scrolling through the options is accomplished using the X axis of the joystick
  - a button press has effect only when pressed on the "Back" option

#### Pause Game Menu
 Characteristics:
  - accessed via click while in game

 Sections:
  - Resume => resumes the current game
  - Exit => goes back to the principal menu (all the progress is lost)

 Motions:
  - scrolling through the options is accomplished using the Y axis of the joystick
  - accesing one of the options is done using the button of the joystick

#### Enter Name Menu
 Characteristics:
  - accessed when the highscore is beaten by the player
  - only players that beat the highscore are asked to enter their names in order to update the highscore top 
  - the name must have maximum 8 characters

 Sections:
  - Keyboard => displays all the possible characters which may go into the name
  - Delete => deletes the last charcater of the name
  - Done => saves the chosen name, updates the highscore top and moves to the end of the game menu

 Motions:
  - scrolling through the options is accomplished using the X axis of the joystick
  - scrolling through the characters of the keyboard is accomplished using both the X and Y axis of the joystick
  - selecting one of the characters from the keyboard is done using the button of the joystick

#### End Game Menu
 Characteristics:
  - accessed when the player has either lost, or won the game

 Sections:
  - Restart => restarts the game from the initial level
  - Exit => goes back to the principal menu

 Motions:
  - scrolling through the options is accomplished using the Y axis of the joystick
  - accesing one of the options is done using the button of the joystick
  
### Game Requirments
 There are 3 distinct objects in the game:
  - The Player => shown by a fast blinking led
  - The Enemies => shown by a blinking led (slower than the one of the player)
  - The Food Items => shown by a static led

#### Intro Display
 Characteristics:
  - plays at the begining of the game
  - nothings happenes during this part if the joystick moves or the button is pressed

#### In Game Display
 Characteristics:
  - shows the level, number of lives and socore of the player
  - updates when either the score, or the number of lives changes
  - the joystick movements control the movement of the player on the matrix
  - the press of the button pauses the game and enteres to the puase game menu

#### Won Game Display / Lost Game Display
 Characteristics:
  - shows corresponding message for the player
  - nothings happenes during this part if the joystick moves or the button is pressed

#### Statisctics Display
 Characteristics:
  - shows the statistics of the game (lives remaining + score obtained)
  - nothings happenes during this part if the joystick moves or the button is pressed

#### Beat Highscore Display
 Characteristics:
  - shown when the player has beaten the highscore to anounce him of his accomplishment
  - shows corresponding message
  - informs the player to enter his name and about the name restrictions
  - nothings happenes during this part if the joystick moves or the button is pressed
 
## Implementation details


## Pciture of setup
![Setup](https://user-images.githubusercontent.com/62221313/145267815-c676f33c-b4ff-4640-9214-0565705099f5.jpeg = 250x250)

## Video of setup

## BTW
In case you are wondering who Marinel is, here's a picture:
![Marinel](https://user-images.githubusercontent.com/62221313/145267826-f689cb21-f235-435f-a546-14fdef400233.jpeg = 250x250)
