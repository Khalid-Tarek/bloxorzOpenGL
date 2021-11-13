#include "block.cpp"
#include <GL\glut.h>

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

void drawCuboid(Block block) {
	double x = block.blockPosition.x;
	double y = block.blockPosition.y;
	double z = block.blockPosition.z;
	
	double hL = block.blockDimens.x / 2.0;
	double hH = block.blockDimens.y / 2.0;
	double hB = block.blockDimens.z / 2.0;

	double faces[6][4][3] = {
		{{x - hL, y + hH, z + hB}, {x + hL, y + hH, z + hB}, {x + hL, y - hH, z + hB}, {x - hL, y - hH, z + hB}},	//front
		{{x + hL, y + hH, z + hB}, {x + hL, y + hH, z - hB}, {x + hL, y - hH, z - hB}, {x + hL, y - hH, z + hB}},	//right
		{{x + hL, y + hH, z - hB}, {x - hL, y + hH, z - hB}, {x - hL, y - hH, z - hB}, {x + hL, y - hH, z - hB}},	//back
		{{x - hL, y + hH, z - hB}, {x - hL, y + hH, z + hB}, {x - hL, y - hH, z + hB}, {x - hL, y - hH, z - hB}},	//left
		{{x - hL, y + hH, z - hB}, {x + hL, y + hH, z - hB}, {x + hL, y + hH, z + hB}, {x - hL, y + hH, z + hB}},	//top
		{{x - hL, y - hH, z + hB}, {x + hL, y - hH, z + hB}, {x + hL, y - hH, z - hB}, {x - hL, y - hH, z - hB}},	//bottom
	};

	glColor3dv(block.blockColors);
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

void drawBoard(Board board){
	Position	p(0, 0, 0);
	Dimensions	d(1, 0.2, 1, false, false);
	double c[3] = {1, 1, 1};
	double gC[3] = {1, 0, 1};

	Block tile(Position(0, 0, 0), d, c);

	for(int i = 0; i < board.dimension; i++){
		tile.blockPosition.x = 0;
		for(int j = 0; j < board.dimension; j++){
			if(board.board[i][j] == 'F') drawCuboid(tile);
			else if(board.board[i][j] == 'W'){
				Block winningTile(tile.blockPosition, d, gC);
				drawCuboid(winningTile);
			}
			tile.blockPosition.x += 1;
		}
		tile.blockPosition.z += 1;
	}
}