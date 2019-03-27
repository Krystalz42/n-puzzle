#ifndef N_PUZZLE_POSITION_HPP
#define N_PUZZLE_POSITION_HPP

#include <cstdlib>
#include <ostream>

struct Position {
	Position() : x(0), y(0) { }

	Position(int x, int y) : x(x), y(y) { }

	Position operator+(const Position &position) {
		return Position(x + position.x, y + position.y);
	}

	Position operator-(const Position &position) {
		return Position(x - position.x, y - position.y);
	}

	bool operator==(const Position &rhs) const {
		return x == rhs.x && y == rhs.y;
	}

	bool operator!=(const Position &rhs) const {
		return !(rhs == *this);
	}

	friend std::ostream &
	operator<<(std::ostream &os, const Position &position) {
		os << "x: " << position.x << " y: " << position.y;
		return os;
	}

	int x;
	int y;
};

#endif //N_PUZZLE_POSITION_HPP
