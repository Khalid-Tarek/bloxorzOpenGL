#include "board.cpp"

//The 2 states of a block's orientation
#define BLOCK_VERTICAL			true	//The block
#define BLOCK_HORIZONTAL		false

//The 2 states of a block's direction
#define DIRECTION_X				true
#define DIRECTION_Z				false

//The distance between the block's center of mass and the board
#define BLOCK_HAB				1.12

/**
 *	A class representing the dimensions of a block
 *	
 *	@fields x, y, z:		the width, height, and depth of the block
 *	@field	orientation:	Whether this block is vertical or horizontal (used only for the controllable block)
 *	@field	direction		Whether this block is parrallel to the X-axis or Z-axis (used only for the controllable block when it's horizontal)
 */
class Dimensions{
public:
	double x; double y; double z;
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

	//Switches the current orientation. Horizontal -> Vertical and Vertical -> Horizontal
	void flipOrientation() { this->orientation = !(this->orientation); }

	/**
	 *	Flip the dimensions around the Z-axis
	 *  If the block is vertical, simply call {@link #flipOrientation()} method and set {@link #orientation} to be prallel to the Z-axis
	 *	Otherwise, if the block is horizontal and parallel to the Z-axis, then just call {@link #flipOrientation()}
	 */
	void flipZ(){
		double temp = this->y;
		this->y = this->z;
		this->z = temp;
		if(this->orientation == BLOCK_VERTICAL){
			flipOrientation();
			this->direction = DIRECTION_Z;
		}
		else if(this->direction == DIRECTION_Z) flipOrientation();
	}

	/**
	 *	Flip the dimensions around the X-axis
	 *  If the block is vertical, simply call {@link #flipOrientation()} method and set {@link #orientation} to be prallel to the X-axis
	 *	Otherwise, if the block is horizontal and parallel to the X-axis, then just call {@link #flipOrientation()}
	 */
	void flipX(){
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

/**
 *	A class representation of a 3D block
 *	
 *	@field blockPosition:	The position of the absolute center of the block
 *	@field blockDimens:		The dimensions of the block (width, height, depth)
 *	@field blockColror:		The color of the block in RGB
 */
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

	//This method is called whenever the block's {@link #blockDimens.orientation} is changed
	void updateY(){
		if(this->blockDimens.orientation == BLOCK_VERTICAL) 
			this->blockPosition.y = BLOCK_HAB;
		else
			this->blockPosition.y = BLOCK_HAB - 0.5;
	}

	/**
	 *	Moves the block forward by changing the z coordinate
	 *	Change is originally -1.5
	 *	If the block is horizontal and parallel to X-axis (flipping on it's side), modify that value by +0.5
	 *	Then call {@link #blockDimens.flipZ()}
	 */
	void moveForwards() {
		double c = -1.5;
		if(this->blockDimens.orientation == BLOCK_HORIZONTAL &&
			this->blockDimens.direction == DIRECTION_X) c += 0.5;
		this->blockPosition.z += c;
		this->blockDimens.flipZ();
		updateY();
	}

	/**
	 *	Moves the block to the left by changing the x coordinate
	 *	Change is originally -1.5
	 *	If the block is horizontal and parallel to Z-axis (flipping on it's side), modify that value by +0.5
	 *	Then call {@link #blockDimens.flipX()}
	 */
	void moveLeft() {
		double c = -1.5;
		if(this->blockDimens.orientation == BLOCK_HORIZONTAL &&
			this->blockDimens.direction == DIRECTION_Z) c += 0.5;
		this->blockPosition.x += c;
		this->blockDimens.flipX();
		updateY();
	}

	/**
	 *	Moves the block backwards by changing the z coordinate
	 *	Change is originally +1.5
	 *	If the block is horizontal and parallel to X-axis (flipping on it's side), modify that value by -0.5
	 *	Then call {@link #blockDimens.flipZ()}
	 */
	void moveBackwards() {
		double c = 1.5;
		if(this->blockDimens.orientation == BLOCK_HORIZONTAL &&
			this->blockDimens.direction == DIRECTION_X) c += -0.5;
		this->blockPosition.z += c;
		this->blockDimens.flipZ();
		updateY();
	}

	/**
	 *	Moves the block to the right by changing the x coordinate
	 *	Change is originally +1.5
	 *	If the block is horizontal and parallel to Z-axis (flipping on it's side), modify that value by -0.5
	 *	Then call {@link #blockDimens.flipX()}
	 */
	void moveRight() {
		double c = 1.5;
		if(this->blockDimens.orientation == BLOCK_HORIZONTAL &&
			this->blockDimens.direction == DIRECTION_Z) c += -0.5;
		this->blockPosition.x += c;
		this->blockDimens.flipX();
		updateY();
	}
};