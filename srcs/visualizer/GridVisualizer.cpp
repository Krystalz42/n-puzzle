#include "GridVisualizer.hpp"
#include <cassert>
#include "SfVectorInterpolate.hpp"

GridVisualizer interpolateGridVisualizer(GridVisualizer &a, GridVisualizer &b, float ratio) {
	assert(a.size() == b.size());

	GridVisualizer c(a);
	auto iterA = a.begin();
	auto iterB = b.begin();
	auto iterC = c.begin();

	while (iterA != a.end()) {
		if (iterA->id != iterB->id) {
			auto moved = b.begin();
			for ( ; moved->id != iterA->id && moved != b.end() ; moved++)
				;
			assert(moved != a.end());
			iterC->position = cosineInterpolate(moved->position, iterA->position, ratio);
		}
		++iterA;
		++iterB;
		++iterC;
	}
	return c;
}

GridVisualizer gridContainerToGridVisualizer(GridContainer &grid) {
	GridVisualizer visu(grid.getX(), grid.getY());

	auto iterGrid = grid.begin();
	auto iterVisu = visu.begin();
	size_t i = 0;

	while (iterGrid != grid.end()) {

		(*iterVisu).position = sf::Vector2f(i % grid.getX(), i / grid.getX());
		(*iterVisu).id = (*iterGrid);
		++iterGrid;
		++iterVisu;
		i++;
	}
	return visu;
}

std::ostream &operator<<(std::ostream &os, const GridVisualizer &grid) {
	os << "Grid[" << grid.getX() << "][" << grid.getY() << "]" << std::endl;
	for (unsigned int y = 0; y < grid.getY(); y++) {
		for (unsigned int x = 0; x < grid.getX(); x++) {
			os << "[" << grid(x, y).id << "] ";
		}
		os << "  |  ";
		for (unsigned int x = 0; x < grid.getX(); x++) {
			os << "[" << grid(x, y).position.x << "|" << grid(x, y).position.y << "] ";
		}
		os << std::endl;
	}
	os << std::endl;
	return os;
}