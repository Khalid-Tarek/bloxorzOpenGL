#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

/**
 *	A simple class representing a coordinate on a 3D plane
 */
class Position{
public:
	double x;
	double y;
	double z;

	Position() {} //Default constructor
	Position(double x, double y, double z){
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

/**
 *	A class representing a 2D board (in our case, the y is fixed at 0)
 *	
 *	A board object is istantiated by passing the level name to the constructor
 *	
 *  @field startingPos: A {@link:Position} that represents where the controllable block starts on this board
 *	@field winningPos:	A {@link:Position} that represents the winning tile on the board. Standing upright on this tile, wins the levels
 *  @field dimension:	The dimension of the board. the bord is dimension x dimension large
 *	@field board:		A vector of strings, where a single character represents one tile
 *						The types of the tiles are as follows:
 *						* F -> Filled Tile
 *						* X -> Empty Tile
 *						* W -> Win Tile
 *						* T -> Trap
 *	@field moves:		The number of moves the player has taken so far
 *	@field optimal:		The lowest number of moves this level can be solved in
 *
 *	A level file is of (txt) extension, in the folder "levels/" in the same directory as main.cpp. It is in this form:
 *		* n
 *		* n lines of n long strings
 *		* x1 z1
 *		* x2 z2
 *		* d
 *	Where n is the dimension of the board, (x1, z1) are the coords of the start position, 
 *	(x2, z2) are the coords of the winning position, and d is the number of optimal moves to win the game
 */
class Board{
public:
	Position startingPos;
	Position winningPos;
	int dimension;
	vector<string> board;
	int optimal;
	int moves;

	Board(){} //Default Constructor
	Board(int level){
		ifstream in("levels/" + to_string((long double)level) + ".txt");
		
		string s;
		getline(in, s);
		this->dimension = stoi(s);

		for(int i = 0; i < this->dimension; i++){
			getline(in, s);
			board.push_back(s);
		}

		int x, z;
		x = (int) (in.get() - '0'); in.get();
		z = (int) (in.get() - '0'); in.get();
		this->startingPos = Position(x, 0, z);

		x = (int) (in.get() - '0'); in.get();
		z = (int) (in.get() - '0'); in.get();
		this->winningPos = Position(x, 0, z);

		optimal =  (int)(in.get() - '0') * 100;
		optimal += (int)(in.get() - '0') * 10;
		optimal += (int)(in.get() - '0'); in.get();

		moves = 0;

		in.close();
	}
};