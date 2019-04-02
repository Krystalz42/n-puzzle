#include "Grid.hpp"
#include <n_puzzle.hpp>

std::ostream &operator<<(std::ostream &os, const Grid<ValuePuzzle > &grid) {
	for (int index = 0; index < grid.size(); ++index) {
		if (!(index % grid.getX()))
			os << std::endl;
		if (!grid.data_[index])
			os << "\033[31m";
		os << std::setw(3);
		os << grid.data_[index] << ' ';
		os << "\033[0m";
	}
	os << std::endl;
	return os;
}