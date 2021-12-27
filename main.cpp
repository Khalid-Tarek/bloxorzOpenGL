/*********************************************************************************************************	
								 ____  _     __   __        ______
								|  _ \| |    \ \ / /       |___  /
								| |_) | | ___ \ V / ___  _ __ / / 
								|  _ <| |/ _ \ > < / _ \| '__/ /  
								| |_) | | (_) / . \ (_) | | / /__ 
								|____/|_|\___/_/ \_\___/|_|/_____|

  Description:	This game was made by The Rad Squad (B) for 3D Graphics Course - EELU (Fall 2021-22)

				In this 3D game, you take control of a cuboid on a 2D board. Your goal is to reach
				the goal by moving the cuboid around, rolling on its sides.

				But be careful not to fall! As an added challenge, try to solve the level in the
				optimal number of moves. Doing so will turn your block golden

  Developers:	Abdallah Kareem
				Osama Sakr


  Project
  Structure:	main.cpp		& main.h	--------------> Program Entry Point + main logic
				glutHelper.cpp	& glutHelper.h -----------> All OpenGL rendering happens here in its 
															respective functions
				board.cpp --------------------------------> Board class + Position Class
				block.cpp --------------------------------> Block class + Dimensions Class

  Rights:		All rights go back to the original designer(s) of the game(s)
						Bloxorz (1984):		Damien Clarke


				This is a simple reimplementation of the original game(s), hope you enjoy it!
*********************************************************************************************************/
#include "main.h"

//Current Game State
int gameState = PLAYING;

//The angles of view of the scene
double sceneRotateX = 35;
double sceneRotateY = 25;

//Variables used for animation
bool isMoving = false;
bool isFalling = false;
int currentAngle = 0;
int currentRotateAround = NONE;
queue<int> queuedMovement;
int axisPosition = BASE;

//The main objects used in the game
Board level;
Block block;

//Periodically Checks for change in the gameState variable
void checkGameState(){
	if(gameState != PLAYING) return;
	if(gameState == LOST && block.blockPosition.y < -block.blockDimens.y * 2) {isFalling = true; return;}

	double x = block.blockPosition.x;
	double z = block.blockPosition.z;

	if(x == level.winningPos.x &&
		z == level.winningPos.z) {
		gameState = WON;
		isFalling = true;
		//TODO: ADD "YOU WON" POP UP
		return;
	}
	
	bool outOfBoundX = x < 0 || x > level.dimension - 1;
	bool outOfBoundZ = z < 0 || z > level.dimension - 1; 

	if(block.blockDimens.orientation == BLOCK_VERTICAL){
		if(outOfBoundX || outOfBoundZ || level.board[(int)z][(int)x] == 'X'){
			cout << "Fell vertically at (" << z << ", " << x << ")\n";
			gameState = LOST;
			isFalling = true;
			//TODO: ADD "YOU LOST" POP UP
		}
	}
	else if(block.blockDimens.orientation == BLOCK_HORIZONTAL){
		bool emptyUnder1 = false;
		bool emptyUnder2 = false;

		if(block.blockDimens.direction == DIRECTION_X){
			int x1 = x - 0.5;
			int x2 = x + 0.5;

			emptyUnder1 = outOfBoundZ || x1 < 0 || x1 > level.dimension - 1 || level.board[(int)z][x1] == 'X';
			emptyUnder2 = outOfBoundZ || x2 < 0 || x2 > level.dimension - 1 || level.board[(int)z][x2] == 'X';
		}
		else if(block.blockDimens.direction == DIRECTION_Z){
			int z1 = z - 0.5;
			int z2 = z + 0.5;

			emptyUnder1 = outOfBoundX || z1 < 0 || z1 > level.dimension - 1 || level.board[z1][(int)x] == 'X';
			emptyUnder2 = outOfBoundX || z2 < 0 || z2 > level.dimension - 1 || level.board[z2][(int)x] == 'X';
		}

		if(emptyUnder1 && emptyUnder2){
			gameState = LOST;
			cout << "Fell straight horizontally at (" << z << ", " << x << ")\n";
			isFalling = true;
			//TODO: ADD "YOU LOST" POP UP
		}
		else if(emptyUnder1){
			if(block.blockDimens.direction == DIRECTION_X){
				currentRotateAround = Z;
				queuedMovement.push(RIGHT);
			}
			else {
				currentRotateAround = -X;
				queuedMovement.push(FORWARDS);
			}
			axisPosition = MIDDLE;
			queuedMovement.push(DOWNWARDS);
			isMoving = true;

			gameState = LOST;
			cout << "Fell skewed horizontally at (" << block.blockPosition.z << ", " << block.blockPosition.x << ")\n";
			//TODO: ADD "YOU LOST" POP UP
		}
		else if(emptyUnder2){
			if(block.blockDimens.direction == DIRECTION_X){
				currentRotateAround = -Z;
				queuedMovement.push(LEFT);
			}
			else{
				currentRotateAround = X;
				queuedMovement.push(BACKWARDS);
			}
			axisPosition = MIDDLE;
			queuedMovement.push(DOWNWARDS);
			isMoving = true;

			gameState = LOST;
			cout << "Fell skewed horizontally at (" << block.blockPosition.z << ", " << block.blockPosition.x << ")\n";
			//TODO: ADD "YOU LOST" POP UP
		}
	}
}

//Using theoretical knowledge we studied in the course, we animated the rotation based on the block's location and direction of rotation
void animateMovement(int rotateAround, int axisPosition){
	if(isMoving){
		double translateX = 0;
		double translateY = -0.12;
		double translateZ = 0;

		if(rotateAround == X || rotateAround == -X)
			translateZ = -block.blockPosition.z;
		else
			translateX = -block.blockPosition.x;

		if(axisPosition == BASE){
			switch(rotateAround){
			case -X:
				translateZ += block.blockDimens.z / 2.0;
				break;
			case X:
				translateZ -= block.blockDimens.z / 2.0;
				break;
			case -Z:
				translateX -= block.blockDimens.x / 2.0;
				break;
			case Z:
				translateX += block.blockDimens.x / 2.0;
				break;
			}
		}

		glTranslated(-translateX, -translateY, -translateZ);
		if(rotateAround == X || rotateAround == -X)
			glRotated((rotateAround / X) * currentAngle, 1, 0, 0);
		else
			glRotated((rotateAround / Z) * currentAngle, 0, 0, 1);
		glTranslated(+translateX, +translateY, +translateZ);
	}
}

//This method is queued to fire after the animation is finished
void actuateMovement(){

	if(axisPosition == BASE){
		switch(queuedMovement.front()){
		case FORWARDS:
			block.moveForwards();
			break;
		case LEFT:
			block.moveLeft();
			break;
		case BACKWARDS:
			block.moveBackwards();
			break;
		case RIGHT:
			block.moveRight();
			break;
		case DOWNWARDS:
			isFalling = true;
			break;
		}
	}
	else {
		switch(queuedMovement.front()){
		case FORWARDS:
			block.blockDimens.flipZ();
			block.blockPosition.z -= 0.5;
			break;
		case LEFT:
			block.blockDimens.flipX();
			block.blockPosition.x += 0.5;
			break;
		case BACKWARDS:
			block.blockDimens.flipZ();
			block.blockPosition.z += 0.5;
			break;
		case RIGHT:
			block.blockDimens.flipX();
			block.blockPosition.x -= 0.5;
			break;
		case DOWNWARDS:
			isFalling = true;
			break;
		}
		block.blockPosition.y -= 0.2;
	}

	queuedMovement.pop();
	currentAngle = 0;
	isMoving = false;

	if(queuedMovement.size() != 0) actuateMovement();
}

//This method sets the block color based on the game state
void setBlockColor(){
	double playingColors[3]		= {1, 0, 0};
	double wonColors[3]			= {0, 1, 0};
	double optimalWinColors[3]	= {1, 0.84, 0};
	double lostColors[3]		= {0, 0, 1};

	if(gameState == PLAYING) {
		block.blockColors[0] = playingColors[0];
		block.blockColors[1] = playingColors[1];
		block.blockColors[2] = playingColors[2];
	}
	else if(gameState == WON) {
		if(level.moves <= level.optimal){
			block.blockColors[0] = optimalWinColors[0];
			block.blockColors[1] = optimalWinColors[1];
			block.blockColors[2] = optimalWinColors[2];
		}
		else {
			block.blockColors[0] = wonColors[0];
			block.blockColors[1] = wonColors[1];
			block.blockColors[2] = wonColors[2];
		}
	}
	else if(gameState == LOST) {
		block.blockColors[0] = lostColors[0];
		block.blockColors[1] = lostColors[1];
		block.blockColors[2] = lostColors[2];
	}
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	renderGUI(level.moves, level.optimal);

	glTranslated(-(level.dimension / 2), 0, -10 - level.dimension);
	glRotated(sceneRotateX, 1, 0, 0);
	glRotated(sceneRotateY, 0, 1, 0);

	//renderCoordinateSystem();

	renderBoard(level);

	animateMovement(currentRotateAround, axisPosition);

	setBlockColor();
	renderCuboid(block);

	glutSwapBuffers();
}

void lockResizing(int width, int height){
	glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	display();
}

void arrows(int key, int x, int y){
	switch(key){
		case GLUT_KEY_DOWN:
			sceneRotateX += 5;
			break;
		case GLUT_KEY_UP:
			sceneRotateX -= 5;
			break;
		case GLUT_KEY_LEFT:
			sceneRotateY += 5;
			break;
		case GLUT_KEY_RIGHT:
			sceneRotateY -= 5;
			break;
	}
	if(sceneRotateX >= 360)	sceneRotateX -= 360;
	if(sceneRotateX < 0)		sceneRotateX += 360;
	if(sceneRotateY >= 360)	sceneRotateY -= 360;
	if(sceneRotateY < 0)		sceneRotateY += 360;
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	if(key == 27) exit(0);

	if(gameState != PLAYING) return;

	if(isMoving) return;
	switch(key){
	case 'w':
		currentRotateAround = -X;
		queuedMovement.push(FORWARDS);
		break;
	case 's':
		currentRotateAround = X;
		queuedMovement.push(BACKWARDS);
		break;
	case 'a':
		currentRotateAround = Z;
		queuedMovement.push(LEFT);
		break;
	case 'd':
		currentRotateAround = -Z;
		queuedMovement.push(RIGHT);
		break;
	}
	axisPosition = BASE;
	isMoving = true;

	level.moves++;

	glutPostRedisplay();
}

void timer(int x){
	if(isMoving){
		currentAngle = x;
		if(x > 90) actuateMovement();
		glutTimerFunc(1, timer, x + ANIMATION_STEP);
	}
	else {
		checkGameState();
		glutTimerFunc(1, timer, 0);
	}
	if(isFalling) block.blockPosition.y -= ANIMATION_STEP / 100.0;

	glutPostRedisplay();
}

int main(int argc, char* argv[]) {

	level = Board(1);

	cout << level.optimal << endl;

	Position startingPosition(level.startingPos.x, BLOCK_HAB, level.startingPos.z);
	Dimensions dimensions(1, 2, 1, BLOCK_VERTICAL, DIRECTION_X);
	double color[3] = {1, 1, 1};
	block = Block(startingPosition, dimensions, color);

	//GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

	//GLUT Window Initialization
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - WINDOW_WIDTH) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - WINDOW_HEIGHT) / 2);
	glutCreateWindow("Bloxorz");
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	//Callback Attatchment
	glutDisplayFunc(display);
	glutReshapeFunc(lockResizing);
	glutSpecialFunc(arrows);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(1, timer, 0);

	//Perspective Projection Setup
	glClearColor(BACKGROUND_COLOR);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOV, WINDOW_WIDTH / WINDOW_HEIGHT, 2.0, 50.0);
	glMatrixMode(GL_MODELVIEW);

	//Depth and Multisampling Setup
	glEnable(GL_DEPTH_TEST);

	//glutFullScreen();
	glutMainLoop();
}
