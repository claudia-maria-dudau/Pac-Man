# Marinel's epic quest for food
 A take on the classic PAC_MAN game using the ARDUINO UNO board, a 8x8 matrix, a lcd 16x2 display, a joystick and a buzzer.
 
## Game Description
 The goal of the player is to gather all the food items from the map, without crossing paths with the enemies. The player has 3 lives which do not reset once a level is complete. The game is finished when the player either loses all of his lives, or has successfully completed all the levels.
 
## Requirements
### Menu Requirments
 The menu is shown on the lcd display and the joystick is used to scroll through it. On power up, the intro of the game is shown, followed by the principal menu of the game.

<details>
 <summary>Details about the game's menus</summary>
 <h4>Principal Menu</h4>
 Characteristics:
 <br/>
 <br/>
 <ul>
  <li>shown right after the intro or after the game has ended</li>
 </ul>
 
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
  - the top highscores (player name + score) are stored in the memory

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
</details>

### Game Requirments
 There are 3 distinct objects in the game:
  - The Player => shown by a fast blinking led
  - The Enemies => shown by a blinking led (slower than the one of the player)
  - The Food Items => shown by a static led

 The difficulty of the game is based on the level, which means the difficulty increases liniary as the game progresses. On each level, there are more food items to gather and the speed of the enemies increases. At given intervals between levels, the size of the map and the number of enemies increases as well.
 <br/>
 Both the player and the enemies are allowed to move up, down, right, left and diagonally. The enemies are not allowed to move to a position where there is either already another enemy, or a food item. If the player finds himself on the same position as an enemy, he loses a life.

<details>
 <summary>Details about the game's displays</summary>
 <h4>Intro Display</h4>
 Characteristics:
 <br/>
 <br/>
 <ul>
  <li>plays at the begining of the game</li>
  <li>nothings happenes during this part if the joystick moves or the button is pressed</li>
 </ul>

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
</details>

## Implementation details
 - The system has 27 states to ensure the flow of the game. The button has attached an interrupt function which helps change between the different states.
 - The highscore top, the display settings and the matrix settings are stored in the EEPROM memory of the ARDUINO UNO.
 - Each component of the game has its own separate class (display, matrix, joystick). There is also a class for the game logic which combines all the components' functionlities and ensures the correct flow of the game.
 - There are 2 custom characters created for the lcd display: a dog and a down arrow. There is also a custom design for the matrix which shows a drawing of PAC-MAN.
 - The initial positions for the player, enemies and food items are randomly generated using the current system time as the seed, to avoid any repetitiveness which may occur.
 

## Pciture of setup
<img src="https://user-images.githubusercontent.com/62221313/145267815-c676f33c-b4ff-4640-9214-0565705099f5.jpeg" width="400" height="400" />

## Video of setup

## BTW
In case you are wondering who Marinel is, here's a picture:
<br/>
<img src="https://user-images.githubusercontent.com/62221313/145267826-f689cb21-f235-435f-a546-14fdef400233.jpeg" width="400" height="300" />
