#include "glutHelper.cpp"

//Window Properties
#define WINDOW_WIDTH		1024
#define WINDOW_HEIGHT		1024
#define FOV					60
#define BACKGROUND_COLOR	0.6, 0.2, 0, 1

//Game States
#define PLAYING				10
#define WON					20
#define LOST				30

//Represention of the axis
#define X					100
#define Y					200
#define Z					300

//Representations of the movements
#define FORWARDS			1000
#define LEFT				2000
#define BACKWARDS			3000
#define RIGHT				4000

//The steps of animating (whether rotation, or later on falling)
#define ANIMATION_STEP		5

//Current Game State
int gameState = PLAYING;

//The angles of view of the scene
double sceneRotateX = 35;
double sceneRotateY = 25;

//Variables used for animation
bool isMoving = false;
int currentAngle = 0;
int currentRotateAround = 0;
int queuedMovement = 0;

//The main objects used in the game
Board level;
Block block;

//Periodically Checks for change in the gameState variable
void checkGameState(){
	if(gameState != PLAYING) return;
	if(block.blockPosition.x == level.winningPos.x && 
		block.blockPosition.z == level.winningPos.z) {
		gameState = WON;
		//TODO: ADD "YOU WON" POP UP
	}

	if(block.blockPosition.x < 0 || block.blockPosition.x > level.dimension - 1 ||
		block.blockPosition.z < 0 || block.blockPosition.z > level.dimension - 1){
		gameState = LOST;
		//TODO: ADD "YOU LOST" POP UP
	}

	else if(block.blockDimens.orientation == BLOCK_VERTICAL){
		if(level.board[(int)block.blockPosition.z][(int)block.blockPosition.x] == 'X'){
			gameState = LOST;
			cout << "Fell vertically at (" << block.blockPosition.z << ", " << block.blockPosition.x << ")\n";
		}
	}
	else if(block.blockDimens.orientation == BLOCK_HORIZONTAL){
		if(block.blockDimens.direction == DIRECTION_X){
			if(level.board[(int)block.blockPosition.z][(int)(block.blockPosition.x - 0.5)] == 'X' ||
				level.board[(int)block.blockPosition.z][(int)(block.blockPosition.x + 0.5)] == 'X'){
				gameState = LOST;
				cout << "Fell horizontally at (" << block.blockPosition.z << ", " << block.blockPosition.x << ")\n";
			}
		
		}else if(block.blockDimens.direction == DIRECTION_Z){
			if(level.board[(int)(block.blockPosition.z - 0.5)][(int)block.blockPosition.x] == 'X' ||
				level.board[(int)(block.blockPosition.z + 0.5)][(int)block.blockPosition.x] == 'X'){
				gameState = LOST;
				cout << "Fell horizontally at (" << block.blockPosition.z << ", " << block.blockPosition.x << ")\n";
			}
		}
	}

	double playingColors[3] = {1, 0, 0};
	double wonColors[3] = {0, 1, 0};
	double lostColors[3] = {0, 0, 1};

	if(gameState == PLAYING) {
		block.blockColors[0] = playingColors[0];
		block.blockColors[1] = playingColors[1];
		block.blockColors[2] = playingColors[2];
	}
	else if(gameState == WON) {
		block.blockColors[0] = wonColors[0];
		block.blockColors[1] = wonColors[1];
		block.blockColors[2] = wonColors[2];
	}
	else if(gameState == LOST) {
		block.blockColors[0] = lostColors[0];
		block.blockColors[1] = lostColors[1];
		block.blockColors[2] = lostColors[2];
	}
}

//Using theoretical knowledge we studied in the course, we animated the rotation based on the block's location and direction of rotation
void animateMovement(int rotateAround){
	if(isMoving){
		double translateX = 0;
		double translateY = -0.12;
		double translateZ = 0;

		switch(rotateAround){
		case -X:
			translateZ = -(block.blockPosition.z - block.blockDimens.z / 2.0);
			break;
		case X:
			translateZ = -(block.blockPosition.z + block.blockDimens.z / 2.0);
			break;
		case -Z:
			translateX = -(block.blockPosition.x + block.blockDimens.x / 2.0);
			break;
		case Z:
			translateX = -(block.blockPosition.x - block.blockDimens.x / 2.0);
			break;
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
	
	switch(queuedMovement){
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
	}

	queuedMovement = 0;
	currentAngle = 0;
	isMoving = false;
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslated(-(level.dimension / 2), 0, -10 - level.dimension);
	glRotated(sceneRotateX, 1, 0, 0);
	glRotated(sceneRotateY, 0, 1, 0);

	//renderCoordinateSystem();

	renderBoard(level);

	animateMovement(currentRotateAround);

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
		isMoving = true;
		queuedMovement = FORWARDS;
		break;
	case 's':
		currentRotateAround = X;
		isMoving = true;
		queuedMovement = BACKWARDS;
		break;
	case 'a':
		currentRotateAround = Z;
		isMoving = true;
		queuedMovement = LEFT;
		break;
	case 'd':
		currentRotateAround = -Z;
		isMoving = true;
		queuedMovement = RIGHT;
		break;
	}
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
	glutPostRedisplay();
}

int main(int argc, char* argv[]) {

	level = Board(1);
	
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
	gluPerspective(FOV, WINDOW_WIDTH / WINDOW_HEIGHT, 2.0, 100.0);
	glMatrixMode(GL_MODELVIEW);

	//Depth and Multisampling Setup
	glEnable(GL_DEPTH_TEST);

	//glutFullScreen();
	glutMainLoop(); 
}