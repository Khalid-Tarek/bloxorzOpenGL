#include "glutHelper.cpp"
#include <queue>

//Window Properties
#define WINDOW_WIDTH		1024
#define WINDOW_HEIGHT		1024
#define FOV					60
#define BACKGROUND_COLOR	0.6, 0.2, 0, 1

//Just None
#define NONE				0

//Game States
#define PLAYING				10
#define WON					20
#define LOST				30

//Representation of the axis
#define X					100
#define Y					200
#define Z					300

//Representations of axis positions
#define BASE				1000
#define MIDDLE				2000

//Representations of the movements
#define FORWARDS			10000
#define LEFT				20000
#define BACKWARDS			30000
#define RIGHT				40000
#define DOWNWARDS			50000

//The steps of animating (whether rotation, or later on falling)
#define ANIMATION_STEP		5

void checkGameState();

void animateMovement(int rotateAround, int axisPosition);

void actuateMovement();

void setBlockColor();

void display();

void lockResizing(int width, int height);

void arrows(int key, int x, int y);

void keyboard(unsigned char key, int x, int y);

void timer(int x);

int main(int argc, char* argv[]);