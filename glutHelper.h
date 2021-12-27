#include "block.cpp"
#include <GL\glut.h>

#define Z_DISTANCE	20

void renderGUI(int moves);

void renderChar(double x, double y, char c);

void renderCoordinateSystem();

void renderCuboid(Block block);

void renderBoard(Board board);