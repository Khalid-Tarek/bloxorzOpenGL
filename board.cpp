#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Position{
public:
	double x;
	double y;
	double z;

	Position() {x = 0; y= 0; z = 0;}
	Position(double x, double y, double z){
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

/**
 * F -> Filled Tile
 * X -> Empty Tile
 * W -> Win Tile
 * T -> Trap
 */
class Board{
public:
	Position startingPos;
	Position winningPos;
	int dimension;
	vector<string> board;

	Board(){}
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

		in.close();
	}
};