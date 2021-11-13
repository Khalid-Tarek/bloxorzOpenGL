#include "board.cpp"

#define BLOCK_VERTICAL			true
#define BLOCK_HORIZONTAL		false

#define DIRECTION_X				true
#define DIRECTION_Z				false

//The distance between the block's center of mass and the board
#define BLOCK_HAB				1.12

class Dimensions{
public:
	double x;
	double y;
	double z;
	bool orientation;
	bool direction;

	Dimensions() {x = 0; y = 0; z = 0;}
	Dimensions(double x, double y, double z, bool orientation, bool direction){
		this->x = x;
		this->y = y;
		this->z = z;
		this->orientation = orientation;
		this->direction = direction;
	}

	void flipOrientation() { this->orientation = !(this->orientation); }

	void moveZ(){
		double temp = this->y;
		this->y = this->z;
		this->z = temp;
		if(this->orientation == BLOCK_VERTICAL){
			flipOrientation();
			this->direction = DIRECTION_Z;
		}
		else if(this->direction == DIRECTION_Z) flipOrientation();
	}
	void moveX(){
		double temp = this->y;
		this->y = this->x;
		this->x = temp;
		if(this->orientation == BLOCK_VERTICAL){
			flipOrientation();
			this->direction = DIRECTION_X;
		}
		else if(this->direction == DIRECTION_X) flipOrientation();
	}
};

class Block{
public:
	Position	blockPosition;
	Dimensions	blockDimens;
	double		blockColors[3];

	Block() {}
	Block(Position p, Dimensions d, double colors[3]){
		this->blockPosition = p;
		this->blockDimens = d;
		this->blockColors[0] = colors[0];
		this->blockColors[1] = colors[1];
		this->blockColors[2] = colors[2];
	}

	void updateY(){
		if(this->blockDimens.orientation == BLOCK_VERTICAL) 
			this->blockPosition.y = BLOCK_HAB;
		else
			this->blockPosition.y = BLOCK_HAB - 0.5;
	}

	void moveForwards() {
		this->blockPosition.z -= 1.5;
		if(this->blockDimens.orientation == BLOCK_HORIZONTAL &&
			this->blockDimens.direction == DIRECTION_X) this->blockPosition.z += 0.5;
		this->blockDimens.moveZ();
		updateY();
	}

	void moveLeft() {
		this->blockPosition.x -= 1.5;
		if(this->blockDimens.orientation == BLOCK_HORIZONTAL &&
			this->blockDimens.direction == DIRECTION_Z) this->blockPosition.x += 0.5;
		this->blockDimens.moveX();
		updateY();
	}

	void moveBackwards() {
		this->blockPosition.z += 1.5;
		if(this->blockDimens.orientation == BLOCK_HORIZONTAL &&
			this->blockDimens.direction == DIRECTION_X) this->blockPosition.z -= 0.5;
		this->blockDimens.moveZ();
		updateY();
	}

	void moveRight() {
		this->blockPosition.x += 1.5;
		if(this->blockDimens.orientation == BLOCK_HORIZONTAL &&
			this->blockDimens.direction == DIRECTION_Z) this->blockPosition.x -= 0.5;
		this->blockDimens.moveX();
		updateY();
	}
};