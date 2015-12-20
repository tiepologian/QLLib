/*
 * Grid.h
 *
 *  Created on: Dec 14, 2015
 *      Author: Gianluca Tiepolo <tiepolo.gian@gmail.com>
 */

#ifndef GRID_H_
#define GRID_H_

#include <iomanip>
#include <vector>

/*
 * Grid
 */
class Grid {
public:
	Grid(int width, int height) : _width(width), _height(height) {};
	virtual ~Grid() {};
	int _iterations = 1;
	void setPosition(int x, int y) {
		_x = x;
		_y = y;
	}
	void setDangerPosition(int x, int y) {
	    Position pos;
	    pos.x = x;
	    pos.y = y;
	    _dangerPositions.push_back(pos);
	}
	bool checkIfDangerPosition(int x, int y) {
	    for(auto i:_dangerPositions) {
		if((i.x == x) && (i.y == y)) return true;
	    }
	    return false;
	}
	void update(int sx, int sy, std::string action, int steps) {
		_sx = sx;
		_sy = sy;
		_action = action;
		_steps = steps;
		//for(int i=0;i<15;i++) std::cout << "\033[F";
		print();
	}
	void print() {
#ifdef __linux__
		int r = system("clear");
#endif
		std::cout << "   +";
		for (int i = 0; i < _width * 2; i++)
			std::cout << "---";
		std::cout << "\b";
		std::cout << "+" << std::endl;
		// columns - j
		for (int j = 0; j < _height; j++) {
			std::cout << _height - j;
			if (_height - j < 10)
				std::cout << std::setw(3);
			else
				std::cout << std::setw(2);
			std::cout << " |";

			// rows - i
			for (int i = 1; i < _width+1; i++) {
				bool isUserPosition = ((i == _x) && (_height-j == _y));
				if((checkIfDangerPosition(i, _height-j)) && (!isUserPosition)) {
				    // draw red danger square
				    std::cout << " ";
				    std::string red = "\033[0;31m";
				    std::cout << red << "\u2588" << "\u2588" << "\u2588";
				    std::string white = "\033[0;0m";
				    std::cout << white << " ";
                                    std::cout << "|";
				} else if(i == _x && _height-j == _y) {
					// draw position square
					std::cout << " ";
					std::cout << "\u2588" << "\u2588" << "\u2588";
					std::cout << " ";
					std::cout << "|";
				} else if(i == 8 && _height-j == 2) {
					// draw green goal square
					std::cout << " ";
					std::string color = "\033[0;32m";
					std::cout << color << "\u2588" << "\u2588" << "\u2588";
					std::string white = "\033[0;0m";
					std::cout << white << " ";
					std::cout << "|";
				} else std::cout << "     |";
			}
			std::cout << std::endl;
			std::cout << "   |";
			for (int i = 1; i < _width+1; i++) {
				bool isUserPosition = ((i == _x) && (_height-j == _y));
				if((checkIfDangerPosition(i, _height-j)) && (!isUserPosition)) {
					// draw red square
                    std::cout << " ";
                    std::string red = "\033[0;31m";
                    std::cout << red << "\u2588" << "\u2588" << "\u2588";
                    std::string white = "\033[0;0m";
                    std::cout << white << " ";
                    std::cout << "|";
				} else if(i == _x && _height-j == _y) {
					std::cout << " ";
					std::cout << "\u2588" << "\u2588" << "\u2588";
					std::cout << " ";
					std::cout << "|";
				} else if(i == 8 && _height-j == 2) {
					std::cout << " ";
					std::string color = "\033[0;32m";
					std::cout << color << "\u2588" << "\u2588" << "\u2588";
					std::string black = "\033[0;0m";
					std::cout << black << " ";
					std::cout << "|";
				} else std::cout << "     |";
			}

			std::cout << std::endl;
			std::cout << "    ";
			if (j + 1 == _height)
				continue;
			for (int k = 0; k < _width * 2; k++) {
				std::cout << "---";
			}
			std::cout << std::endl;
		}
		std::cout << "\b+";
		for (int i = 0; i < _width * 2; i++)
			std::cout << "---";
		std::cout << "\b";
		std::cout << "+" << std::endl;
		std::cout << " ";
		for (int i = 0; i < _width; i++)
			std::cout << "     " << i + 1;
		printFooter();
	}
	void printFooter() {
		std::cout << std::endl << std::endl;
		std::cout << "CURRENT STATE: " << _sx << "," << _sy << "          " << std::endl;
		std::cout << "ACTION: " << _action << "     " << std::endl;
		std::cout << "STEPS: " << _steps << "     " << std::endl;
		std::cout << "ITERATIONS: " << _iterations << "     " << std::endl;
	}
private:
	int _width;
	int _height;
	int _x = 0;
	int _y = 0;
	int _sx = 0;
	int _sy = 0;
	std::string _action;
	int _steps = 0;
	struct Position {
	    int x;
	    int y;
	};
	std::vector<Position> _dangerPositions;
};

#endif /* GRID_H_ */
