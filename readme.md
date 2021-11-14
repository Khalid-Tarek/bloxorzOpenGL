# bloxorzOpenGL
This project was created by The RAD Squad (B) for the Three Dimensional Graphics Course. We use the [GLUT](https://en.wikipedia.org/wiki/OpenGL_Utility_Toolkit) library in this project to manipulate the window, among other OpenGL functionalities.

We acknowledge that the idea for this game goes back to [Damien Clark](https://damienclarke.me/#bloxorz). All rights go back to them. This is merely a reimplementation of the game for the course's project.

## Prerequisites to run the game
To run this game you must have OpenGL installed in the correct locations on your device:
1.  Move  **glut.h** into  **C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include\GL\\**
2.  Move  **glut32.lib**  into  **C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\lib\\**
3.  **Move  glut32.dll into  C:\WindowsSystem32\\**

emphasis on the last one.

Just run the .exe file and have fun.

## Technologies Used

 - C/C++ OOP
	 - Position Class
	 - Dimension Class
	 - Board Class
	 - Block Class
 - OpenGL 
	 - Transformations (To facilitate movement animation + scene movement)
		 - glTranslated()
		 - glRotated()
	 - Window Manipulations
		 - gluPerspective()
		 - glViewPort()
	 - GLUT callback functions
		 - timer() - A timer to animate movement and update game state.
		 - arrows() - Move around the scene (Around X and Y axes).
		 - keyboard() - Control the movable block (WASD) and exit the game (ESC).

## Goal of the Game
The goal of this game is to move around the playable block until it falls upright into the goal tile of the board.
