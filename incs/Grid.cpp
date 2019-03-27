#include "Grid.hpp"

std::ostream &operator<<(std::ostream &os, const Grid<uint16_t> &grid) {
	for (int index = 0; index < grid.size(); ++index) {
		if ((index % grid.x_) == 0)
			os << std::setw(4) << std::endl;
		os << grid.data_[index] << ' ';
	}
	os << std::endl;
	return os;
}
