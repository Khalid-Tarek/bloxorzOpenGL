#include <GL\glut.h>

#define WINDOW_WIDTH			1024
#define WINDOW_HEIGHT			720
#define BACKGROUND_COLOR		0.4, 0.15, 0, 1

double rotateX = 0;
double rotateY = 0;

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

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslated(0,0,-10);
	glRotated(rotateX, 1, 0, 0);
	glRotated(rotateY, 0, 1, 0);

	drawCoordinateSystem();

	glColor3d(1,1,1);
	glLineWidth(1);
	glBegin(GL_QUADS);
		glVertex3d(0,0,0);
		glVertex3d(0,1,0);
		glVertex3d(1,1,0);
		glVertex3d(1,0,0);
	glEnd();

	glutSwapBuffers();
}

void lockResizing(int width, int height){
	
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
	switch(key){
	case 27:
		exit(0);
		break;
	case 'w':
		
		break;
	case 's':
		
		break;
	case 'a':
		
		break;
	case 'd':
		
		break;
	}
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