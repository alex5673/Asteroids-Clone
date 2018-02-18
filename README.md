# Asteroids-Clone
Second year Games Technology project

C++ Asteroids clone developed using Microsoft Visual Studio.

Aim of game is to control the spaceship and destroy as many asteroids as possible. 

Custom game engine was provided for me to use by lecturer. I used the engine to make an Asteroids clone.
My Asteroids clone has a variety of different features including: main menu, high score table and an AI demo mode.
The main menu is implemented using simple GUI labels with controls for displaying the high score table.
The high score table features the all time high scores, which is implemented using files to save the top 5 scores and player's name.
The demo mode is implemented using a finite state machine. The AI player in the demo mode uses an algorithm I designed to constantly
check calculate a radius around the ship. If an asteroid(s) is within the radius then the AI will turn towards the nearest asteroid
and fire at it (a defense mode). If there is not an asteroid(s) within the radius then the AI will turn towards the nearest asteroid
in general and move towards it, plus fire at it (an attack mode).
