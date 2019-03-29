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
			iterC->position = interpolateVector2f(iterA->position, moved->position, ratio);
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

sf::VertexArray getQuadTilePixel_(int indexWidthTile, int indexHeightTile, float indexWidthGrid, float indexHeightGrid, unsigned int tileSizeX, unsigned int tileSizeY) {
	sf::VertexArray quad(sf::Quads, 4);

	quad[0].position = sf::Vector2f(((indexWidthGrid * tileSizeX + pixelBorder * indexWidthGrid)), ((indexHeightGrid * tileSizeY + pixelBorder * indexHeightGrid)));
	quad[1].position = sf::Vector2f(tileSizeX, 0.f) + quad[0].position;

	quad[2].position = quad[0].position + sf::Vector2f(tileSizeX, tileSizeY);
	quad[3].position = quad[2].position - sf::Vector2f(tileSizeX, 0.f);

	quad[0].texCoords = sf::Vector2f((tileSizeX) * (indexWidthTile),
									 (tileSizeY) * (indexHeightTile));
	quad[1].texCoords = sf::Vector2f(tileSizeX, 0.f) + quad[0].texCoords;

	quad[2].texCoords = sf::Vector2f((tileSizeX) * (indexWidthTile + 1),
									 (tileSizeY) * (indexHeightTile + 1));
	quad[3].texCoords = quad[2].texCoords - sf::Vector2f(tileSizeX, 0.f);
	return (quad);
}

void updateRenderTextureFromgridContainer(sf::RenderTexture &render, GridContainer &a, GridContainer &b, float ratio, sf::Texture const &texture) {
	GridVisualizer previous_state(gridContainerToGridVisualizer(a));
	GridVisualizer current_state(gridContainerToGridVisualizer(b));
	GridVisualizer interpolated_state = interpolateGridVisualizer(current_state, previous_state, ratio);

	render.clear();
	unsigned int pixelTileSizeX = render.getSize().x / a.getX();
	unsigned int pixelTileSizeY = render.getSize().y / a.getY();

	for (unsigned int y = 0; y < a.getY(); ++y) {
		for (unsigned int x = 0; x < a.getX(); ++x) {
			render.draw(getQuadTilePixel_(interpolated_state(x, y).id % a.getX(), interpolated_state(x, y).id / a.getX(),
										  interpolated_state(x, y).position.x, interpolated_state(x, y).position.y,
										  pixelTileSizeX, pixelTileSizeY),
						&texture);
		}
	}
	render.display();
}