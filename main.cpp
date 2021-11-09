#include <GL\glut.h>

#define WINDOW_WIDTH			720
#define WINDOW_HEIGHT			720
#define BACKGROUND_COLOR		1, 0.4, 0, 1

#define BOARD_DIMENSIONS		5

#define BLOCK_VERTICAL			true
#define BLOCK_HORIZONTAL		false
#define DIRECTION_X				true
#define DIRECTION_Z				false

#define X						0
#define Y						1
#define Z						2

#define PLAYING					100
#define WON						200
#define LOST					300

int gameState = PLAYING;

double rotateX = 30;
double rotateY = 30;

/*double angleStep = 5;
double rotateBlock = 0;*/

/**
 * F -> Filled Tile
 * X -> Empty Tile
 * W -> Win Tile
 * T -> Trap
 */
char board[BOARD_DIMENSIONS][BOARD_DIMENSIONS] = {
	{'F', 'F', 'F', 'F', 'F'},
	{'F', 'F', 'F', 'F', 'F'},
	{'F', 'F', 'F', 'F', 'F'},
	{'F', 'F', 'F', 'W', 'F'},
	{'F', 'F', 'F', 'F', 'F'}
};
double winningPosition[2] = {3,3};
double startingPosition[2] = {0, 4};

double blockPosition[3] = {startingPosition[0], 1.12, startingPosition[1]};
double blockDimens[3] = {1, 2, 1};
double blockColor[3] = {1, 0, 0};
bool   blockOrientation = BLOCK_VERTICAL;
bool   movementDirection = DIRECTION_X;

/*double blockPosition[3] = {startingPosition[0], 1.12 - 0.5, startingPosition[1]};
double blockDimens[3] = {2, 1, 1};
double blockColor[3] = {1, 0, 0};
int	   blockOrientation = BLOCK_HORIZONTAL;
bool   movementDirection = DIRECTION_X;*/

bool isMoving = false;

void drawCuboid(double* position, double* dimens, double* color) {
	double x = position[0]; double y = position[1]; double z = position[2];
	
	double hL = dimens[0] / 2.0; double hH = dimens[1] / 2.0; double hB = dimens[2] / 2.0;

	double faces[6][4][3] = {
		{{x - hL, y + hH, z + hB}, {x + hL, y + hH, z + hB}, {x + hL, y - hH, z + hB}, {x - hL, y - hH, z + hB}},	//front
		{{x + hL, y + hH, z + hB}, {x + hL, y + hH, z - hB}, {x + hL, y - hH, z - hB}, {x + hL, y - hH, z + hB}},	//right
		{{x + hL, y + hH, z - hB}, {x - hL, y + hH, z - hB}, {x - hL, y - hH, z - hB}, {x + hL, y - hH, z - hB}},	//back
		{{x - hL, y + hH, z - hB}, {x - hL, y + hH, z + hB}, {x - hL, y - hH, z + hB}, {x - hL, y - hH, z - hB}},	//left
		{{x - hL, y + hH, z - hB}, {x + hL, y + hH, z - hB}, {x + hL, y + hH, z + hB}, {x - hL, y + hH, z + hB}},	//top
		{{x - hL, y - hH, z + hB}, {x + hL, y - hH, z + hB}, {x + hL, y - hH, z - hB}, {x - hL, y - hH, z - hB}},	//bottom
	};

	glColor4d(color[0], color[1], color[2], color[3]);
	glLineWidth(1);
	glBegin(GL_QUADS);
		for(int i = 0; i < 6; i++){
			for(int j = 0; j < 4; j++){
				glVertex3dv(faces[i][j]);
			}
		}
	glEnd();

	glColor4d(0, 0, 0, 1);
	glLineWidth(1.5);
	for(int i = 0; i < 6; i++){
		glBegin(GL_LINE_LOOP);
			for(int j = 0; j < 4; j++){
				glVertex3dv(faces[i][j]);
			}
		glEnd();
	}
}

/**
 * Draws 3 simple lines signifying the 3 positive axis. Only used for debugging purposes
 */
void drawCoordinateSystem(){
	double origin[3] = {0,0,0};
	double x[3] = {100,0,0};
	double y[3] = {0,100,0};
	double z[3] = {0,0,100};

	glColor3d(0.5,0.5,0.5);
	glLineWidth(1);
	glBegin(GL_LINES);
		glVertex3dv(origin);		
		glVertex3dv(x);

		glVertex3dv(origin);		
		glVertex3dv(y);

		glVertex3dv(origin);		
		glVertex3dv(z);
	glEnd();
}

void drawBoard(char board[BOARD_DIMENSIONS][BOARD_DIMENSIONS]){
	double position[3] = {0, 0, 0};
	double dimens[3] = {1, 0.2, 1};
	double color[3] = {1, 1, 1};
	double goalColor[3] = {1, 0, 1};

	for(int i = 0; i < BOARD_DIMENSIONS; i++){
		position[0] = 0;
		for(int j = 0; j < BOARD_DIMENSIONS; j++){
			if(board[i][j] == 'F')
				drawCuboid(position, dimens, color);
			if(board[i][j] == 'W')
				drawCuboid(position, dimens, goalColor);
			position[0]++;
		}
		position[2]++;
	}
}

/*void animateMovement(double* position, int axis){
	if(axis == X){

		glRotated(rotateblock, 1, 0, 0);
	}
	else if(axis == Z){
		
	}
}*/

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslated(-(BOARD_DIMENSIONS	/ 2),0,-10 - (BOARD_DIMENSIONS	/ 2));
	glRotated(rotateX, 1, 0, 0);
	glRotated(rotateY, 0, 1, 0);

	drawCoordinateSystem();

	drawBoard(board);

	if(blockPosition[X] == winningPosition[0] && blockPosition[Z] == winningPosition[1]) {
		gameState = WON;
		//TODO: ADD "YOU WON" POP UP
	}

	if(blockPosition[X] < 0 || blockPosition[X] > BOARD_DIMENSIONS - 1 ||
		blockPosition[Z] < 0 || blockPosition[Z] > BOARD_DIMENSIONS - 1){
		gameState = LOST;
		//TODO: ADD "YOU LOST" POP UP
	}

	if(blockOrientation == BLOCK_VERTICAL){
		if(board[(int)blockPosition[Z]][(int)blockPosition[X]] == 'X'){
			gameState = LOST;
		}
	}
	else if(blockOrientation == BLOCK_HORIZONTAL){
		if(movementDirection == DIRECTION_X){
			if(board[(int)(blockPosition[Z])][(int)(blockPosition[X] - 0.5)] == 'X' ||
				board[(int)(blockPosition[Z])][(int)(blockPosition[X] + 0.5)] == 'X'){
				gameState = LOST;
			}
		
		}else if(movementDirection == DIRECTION_Z){
			if(board[(int)(blockPosition[Z] - 0.5)][(int)(blockPosition[X])] == 'X' ||
				board[(int)(blockPosition[Z] + 0.5)][(int)(blockPosition[X])] == 'X'){
				gameState = LOST;
			}
		}
	}

	if(gameState == PLAYING)
		drawCuboid(blockPosition, blockDimens, blockColor);

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
		if(blockOrientation == BLOCK_VERTICAL){
			blockOrientation = BLOCK_HORIZONTAL;
			blockPosition[Y] -= 0.5;
			blockPosition[Z] -= 1.5;
			blockDimens[Y] = 1;
			blockDimens[Z] = 2;
			movementDirection = DIRECTION_Z;
			isMoving = true;
		}
		else{
			if(movementDirection == DIRECTION_X)
				blockPosition[Z] -= 1;
			else if(movementDirection == DIRECTION_Z){
				blockOrientation = BLOCK_VERTICAL;
				blockPosition[Y] += 0.5;
				blockPosition[Z] -= 1.5;
				blockDimens[Y] = 2;
				blockDimens[Z] = 1;
			}
		}
		break;
	case 's':
		if(blockOrientation == BLOCK_VERTICAL){
			blockOrientation = BLOCK_HORIZONTAL;
			blockPosition[Y] -= 0.5;
			blockPosition[Z] += 1.5;
			blockDimens[Y] = 1;
			blockDimens[Z] = 2;
			movementDirection = DIRECTION_Z;
		}
		else{
			if(movementDirection == DIRECTION_X)
				blockPosition[Z] += 1;
			else if(movementDirection == DIRECTION_Z){
				blockOrientation = BLOCK_VERTICAL;
				blockPosition[Y] += 0.5;
				blockPosition[Z] += 1.5;
				blockDimens[Y] = 2;
				blockDimens[Z] = 1;
			}
		}
		break;
	case 'a':
		if(blockOrientation == BLOCK_VERTICAL){
			blockOrientation = BLOCK_HORIZONTAL;
			blockPosition[X] -= 1.5;
			blockPosition[Y] -= 0.5;
			blockDimens[X] = 2;
			blockDimens[Y] = 1;
			movementDirection = DIRECTION_X;
		}
		else {
			if(movementDirection == DIRECTION_X){
				blockOrientation = BLOCK_VERTICAL;
				blockPosition[X] -= 1.5;
				blockPosition[Y] += 0.5;
				blockDimens[X] = 1;
				blockDimens[Y] = 2;
			} else if(movementDirection == DIRECTION_Z)
				blockPosition[X] -= 1;
		}
		break;
	case 'd':
		if(blockOrientation == BLOCK_VERTICAL){
			blockOrientation = BLOCK_HORIZONTAL;
			blockPosition[X] += 1.5;
			blockPosition[Y] -= 0.5;
			blockDimens[X] = 2;
			blockDimens[Y] = 1;
			movementDirection = DIRECTION_X;
		}
		else {
			if(movementDirection == DIRECTION_X){
				blockOrientation = BLOCK_VERTICAL;
				blockPosition[X] += 1.5;
				blockPosition[Y] += 0.5;
				blockDimens[X] = 1;
				blockDimens[Y] = 2;
			} else if(movementDirection == DIRECTION_Z)
				blockPosition[X] += 1;
		}
		break;
	}
	glutPostRedisplay();
}

void timer(int x){
	
}

int main(int argc, char* argv[]) {

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