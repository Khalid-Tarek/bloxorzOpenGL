#include "block.cpp"
#include <GL\glut.h>

/**
 *	Renders 3 simple lines signifying the 3 positive axis. Only used for debugging purposes
 */
void renderCoordinateSystem(){
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

/**
 *	Renders a cuboid represented by a Block object
 *
 *	A cuboid (in the graphical sense) is a closed, 3D, 6 rectangular-faced object that is rendered from it's absolute center
 */
void renderCuboid(Block block) {
	//If the block is completely under the board (+ a small bias for animation reasons), don't draw it
	if(block.blockPosition.y < (-block.blockDimens.y) - 0.5) return;

	//Get the 3D coordinate of the center of the block
	double x = block.blockPosition.x;
	double y = block.blockPosition.y;
	double z = block.blockPosition.z;

	//Get half the dimensions of the block
	double hL = block.blockDimens.x / 2.0;
	double hH = block.blockDimens.y / 2.0;
	double hB = block.blockDimens.z / 2.0;

	//Create an array of all the faces, each face represented by 4 points, and each point having 3 coordinates xyz
	//All faces go from top left -> top right -> bottom right -> bottom left (To make the front of the face on the outside of the cuboid)
	double faces[6][4][3] = {
		{{x - hL, y + hH, z + hB}, {x + hL, y + hH, z + hB}, {x + hL, y - hH, z + hB}, {x - hL, y - hH, z + hB}},	//front
		{{x + hL, y + hH, z + hB}, {x + hL, y + hH, z - hB}, {x + hL, y - hH, z - hB}, {x + hL, y - hH, z + hB}},	//right
		{{x + hL, y + hH, z - hB}, {x - hL, y + hH, z - hB}, {x - hL, y - hH, z - hB}, {x + hL, y - hH, z - hB}},	//back
		{{x - hL, y + hH, z - hB}, {x - hL, y + hH, z + hB}, {x - hL, y - hH, z + hB}, {x - hL, y - hH, z - hB}},	//left
		{{x - hL, y + hH, z - hB}, {x + hL, y + hH, z - hB}, {x + hL, y + hH, z + hB}, {x - hL, y + hH, z + hB}},	//top
		{{x - hL, y - hH, z + hB}, {x + hL, y - hH, z + hB}, {x + hL, y - hH, z - hB}, {x - hL, y - hH, z - hB}},	//bottom
	};

	//render the cuboid
	glColor3dv(block.blockColors);	//Set the current rendering color to the block's color
	glLineWidth(1);					//Set the line width to the default: 1
	glBegin(GL_QUADS);
		for(int i = 0; i < 6; i++){
			for(int j = 0; j < 4; j++){
				glVertex3dv(faces[i][j]);
			}
		}
	glEnd();

	//render an outline line loop around the cuboid to distinguish faces (MIGHT REMOVE THIS IF WE IMPLEMENT SHADOWS)
	glColor4d(0, 0, 0, 1);			//Set the current rendering color to black
	glLineWidth(1.5);				//Set the line width to: 1.5
	for(int i = 0; i < 6; i++){
		glBegin(GL_LINE_LOOP);
			for(int j = 0; j < 4; j++){
				glVertex3dv(faces[i][j]);
			}
		glEnd();
	}
}

/**
 *	Renders a board represented by a Board object
 *  
 *	A board (in the graphical sense) is a group of tiles (or low height cuboids) that are arranged in an array
 *	All the tiles are rendered in one color, except for the winning tile, which is different to grab attention
 */
void renderBoard(Board board){
	
	//Define the properties of the regular tile
	Position	p(0, 0, 0);						//Starting position
	Dimensions	d(1, 0.2, 1, false, false);		//Fixed dimension for all tiles
	double c[3] = {1, 1, 1};					//Color of all regular tiles
	double wC[3] = {1, 0, 1};					//Color of winning tile

	Block tile(Position(0, 0, 0), d, c);		//Instatiate a block object that keeps getting rendered after changing its position gradually

	for(int i = 0; i < board.dimension; i++){
		
		//Reset line at x = 0
		tile.blockPosition.x = 0;

		for(int j = 0; j < board.dimension; j++){
			if(board.board[i][j] == 'F') renderCuboid(tile);
			else if(board.board[i][j] == 'W'){
				Block winningTile(tile.blockPosition, d, wC); //Create a new block object with the current tile's position, but with the winning tile Color
				renderCuboid(winningTile);
			}
			tile.blockPosition.x += 1;
		}

		//Go to next "line" of the board
		tile.blockPosition.z += 1;
	}
}