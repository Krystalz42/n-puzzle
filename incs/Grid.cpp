#include "Grid.hpp"
#include <n_puzzle.hpp>

std::ostream &operator<<(std::ostream &os, const Grid<ValuePuzzle > &grid) {
	for (int index = 0; index < grid.size(); ++index) {
		if (!(index % grid.getX()))
			os << std::endl;
		os << grid.data_[index] << ' ';
	}
	os << std::endl;
	return os;
}