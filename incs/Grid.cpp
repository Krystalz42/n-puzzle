#include "Grid.hpp"

std::ostream &operator<<(std::ostream &os, const Grid<uint16_t> &grid) {
	for (int index = 0; index < grid.size(); ++index) {
		if (!(index % grid.getX()))
			os << std::endl;
		os << grid.data_[index] << ' ';
	}
	os << std::endl;
	return os;
}
