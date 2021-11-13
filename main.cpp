#include "glutHelper.h"

#define WINDOW_WIDTH			720
#define WINDOW_HEIGHT			720
#define BACKGROUND_COLOR		1, 0.4, 0, 1

#define PLAYING					100
#define WON						200
#define LOST					300

int gameState = PLAYING;

Board level;
Block block;

double rotateX = 30;
double rotateY = 30;

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
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslated( -(level.dimension / 2), 0, -10 - level.dimension);
	glRotated(rotateX, 1, 0, 0);
	glRotated(rotateY, 0, 1, 0);

	drawCoordinateSystem();

	drawBoard(level);

	

	if(gameState == PLAYING)
		drawCuboid(block);

	glutSwapBuffers();
}

void lockResizing(int width, int height){
	glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	display();
}

void arrows(int key, int x, int y){
	switch(key){
		case GLUT_KEY_DOWN:
			rotateX += 5;
			break;
		case GLUT_KEY_UP:
			rotateX -= 5;
			break;
		case GLUT_KEY_LEFT:
			rotateY += 5;
			break;
		case GLUT_KEY_RIGHT:
			rotateY -= 5;
			break;
	}
	if(rotateX >= 360)	rotateX -= 360;
	if(rotateX < 0)		rotateX += 360;
	if(rotateY >= 360)	rotateY -= 360;
	if(rotateY < 0)		rotateY += 360;
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	if(key == 27) exit(0);
	if(gameState != PLAYING) return;
	switch(key){
	case 'w':
		block.moveForwards();
		break;
	case 's':
		block.moveBackwards();
		break;
	case 'a':
		block.moveLeft();
		break;
	case 'd':
		block.moveRight();
		break;
	}
	glutPostRedisplay();
}

void timer(int x){
	checkGameState();
	glutTimerFunc(1, timer, x);
	glutPostRedisplay();
}

int main(int argc, char* argv[]) {

	level = Board(1);
	
	Position startingPosition(level.startingPos.x, BLOCK_HAB, level.startingPos.z);
	Dimensions dimensions(1, 2, 1, BLOCK_VERTICAL, DIRECTION_X);
	double color[3] = {1, 0, 0};
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
	gluPerspective(60, WINDOW_WIDTH / WINDOW_HEIGHT, 2.0, 50.0);
	glMatrixMode(GL_MODELVIEW);

	//Depth and Multisampling Setup
	glEnable(GL_DEPTH_TEST);

	//glutFullScreen();
	glutMainLoop(); 
}