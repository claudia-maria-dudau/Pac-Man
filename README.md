# Marinel's epic quest for food
 A take on the classic PAC_MAN game following Marinel's journey to collect all the food in the galaxy, while avoiding the guard dogs. 
 <br />
 The reason why I chose this game is because it's one of my favorite childhood games and it was really fun creating my own version of it. The game was built using basic hardware components and oop notions for modularization.
 
## Game Description
 The goal of the player is to gather all the food items from the map, without crossing paths with the enemies. The player has 3 lives which do not reset once a level is completed. The game is finished when the player either loses all of his lives, or has successfully completed all the levels.
 
### How to play
 Using the joystick that comes with the game, the player moves up, down, right, left or diagonally on the map. The player must collect all the food items on the current level, while trying his best to not land on the same position as one of the enemies. With each enemy the player encounters, he loses a life and a point is substracted from his score. Each collected food item increases the player's score by the number of the current level.

## Used Components
 - ARDUINO UNO board
 - 8x8 matrix (used to display the game)
 - 15x2 lcd (used to display the menu of the game)
 - joystick (used to navigate through the menu and to move in the game)
 - buzzer (used for sounds)
 
## Requirements
### Menu Requirments
 The game has a principal menu, a settings menu and some smaller menus. On start up, the intro of the game is played, followed by the principal menu.

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
  - the top highscores are updated after every game, if the score of the player is higher than at least the last highscore in the top
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
  - Sound => sets whether the sound is on or off
  - Res Highscore => goes into the reset highscore menu
  - Back => goes back to the principal menu

 Motions:
  - scrolling through the options is accomplished using the X axis of the joystick
  - scroolling through the values for each setting is accomplished using the X axis of the joystick
  - accesing one of the options is done using the button of the joystick

### Reset Highscore
 Characteristics:
  - accessed via click from the settings menu
  
 Sections:  
  - No => goes back to the settings menu (nothing happenes)
  - Yes => resets the highscore top (deletes the stored values) and goes back to the settings menu

 Motions:
  - scrolling through the options is accomplished using the Y axis of the joystick
  - accesing one of the options is done using the button of the joystick

#### About
 Characteristics:
  - accessed via click from the principal menu
  - each section starts scrolling once its selected

 Sections:
  - Game Name
  - Creator Name
  - Github => the github link where the game's repository can be found
  - Back => goes back to the principal menu
 
 Motions:
  - scrolling through the options is accomplished using the X axis of the joystick
  - a button press has effect only when pressed on the "Back" option

#### Pause Game Menu
 Characteristics:
  - accessed via click while in game
  - the game freezes in its current state

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
  - Done => saves the chosen name, updates the highscore top and moves to the end game menu

 Motions:
  - scrolling through the options is accomplished using the X axis of the joystick
  - scrolling through the characters of the keyboard is accomplished using both the X and Y axis of the joystick
  - selecting one of the characters from the keyboard is done using the button of the joystick
  - accesing one of the options is done using the button of the joystick

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
  - The Player 
    - represented by the fastest blinking dot on the map
    - controlled by the joystick's movement
  - The Enemies 
    - represented by the slower blinking dots on the map
    - move at a constat interval during the level (up, down, left, right or diagonally)
    - not allowed to move to an already taken position, unless it's the position of the player
    - if an enemy finds itself in the visual range of the player, then it starts moving towards him, otherwise it moves at random
    - if an enemy intersects with the player, a specific sound is emitted from the buzzer
  - The Food Items
    - represented by a static dot on the map
    - if a food item is collected by the player, a specific sound is emitted from the buzzer

 The difficulty of the game is based on the level, which means that it increases liniary as the game progresses. On each level, there are more food items to gather and the speed of the enemies increases. At given intervals between levels, the size of the map and the number of enemies increases as well. 

<details>
 <summary>Details about the game's displays</summary>
 <h4>Intro Display</h4>
 Characteristics:
 <br/>
 <br/>
 <ul>
  <li>plays at the begining of the game</li>
  <li>nothings happenes during this part if the joystick moves</li>
  <li>skipping this part is done by pressing the button of the joystick</li>
 </ul>
 
#### Start Game Display
 Characteristics:
  - shown at the begining of each game to prepare the player for the start of the game
  - buzzer plays a song while the screen is shown
  - nothings happenes during this part if the joystick moves or if the button is pressed

#### In Game Display
 Characteristics:
  - shows the level, number of lives and socore of the player
  - updates when either the score, or the number of lives changes
  - the joystick movements control the movement of the player on the matrix
  - the press of the button pauses the game and enteres the puase game menu

#### Won Game Display / Lost Game Display
 Characteristics:
  - shows corresponding message for the player
  - nothings happenes during this part if the joystick moves
  - skipping this part is done by pressing the button of the joystick

#### Statisctics Display
 Characteristics:
  - shows the statistics of the game (lives remaining + score obtained)
  - nothings happenes during this part if the joystick moves
  - skipping this part is done by pressing the button of the joystick

#### Beat Highscore Display
 Characteristics:
  - shown when the player has beaten the highscore to anounce him of his accomplishment
  - shows corresponding message
  - informs the player to enter his name and about the name restrictions
  - nothings happenes during this part if the joystick moves
  - skipping this part is done by pressing the button of the joystick
</details>

## Implementation details
 - The system has 33 states to ensure the flow of the game. The button has attached an interrupt function which helps change between the different states.
 - For the bigger maps of the game, which exceed the 8x8 bounds of the matrix, the 'moving camera' technique is used to navigate its surface.
 - The highscore top, the display settings and the matrix settings are stored in the EEPROM memory of the ARDUINO UNO.
 - Each component of the game has its own separate class (display, matrix, joystick, buzzer). There is also a class for the game logic which combines all the components' functionlities and ensures the correct flow of the game.
 - There are 2 custom characters created for the lcd display: a dog and a down arrow. There is also a custom design for the matrix which shows a drawing of PAC-MAN and an arrow animation which is shown between levels.
 - The initial positions for the player, enemies and food items are randomly generated using the current system time as the seed, to avoid any repetitiveness which may occur.
 

## Pciture of setup
<img src="https://user-images.githubusercontent.com/62221313/146199540-f3c28b32-d60d-4e60-9280-0f0152c128d4.jpeg" width="400" height="400" />

## Video of setup
<a href="https://youtu.be/IfucYv657mc" target="_blank">Video</a>

## BTW
In case you're wondering who Marinel is, here's a picture:
<br/>
<br/>
<img src="https://user-images.githubusercontent.com/62221313/145267826-f689cb21-f235-435f-a546-14fdef400233.jpeg" width="400" height="300" />
