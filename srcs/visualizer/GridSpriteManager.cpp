#include "GridSpriteManager.hpp"
#include <cassert>
#include "SfVectorInterpolate.hpp"
#include <math.h>
#include <resolver/KStar.hpp>

namespace Visualizer {

	void GridSpriteManager::setSpriteRect_() {


		KStar::Builder builder;
		builder.setSize(tileSize_.x);
		KStar::node_pointer nodePointer = builder.buildGoalGrid();

		std::cout << nodePointer->grid << std::endl;

		for (unsigned int y = 0; y < tileSize_.y; y++) {
			for (unsigned int x = 0; x < tileSize_.x; x++) {
				size_t index = nodePointer->grid[x + y * tileSize_.x];
				sprites_[index].setTextureRect(
						sf::IntRect(x * pixelTileSize_.x, y * pixelTileSize_.y, pixelTileSize_.x, pixelTileSize_.y));
				sprites_[index].setTexture(texture_);
			}
		}
	}

	//void GridSprite::init();

	void GridSpriteManager::updateSpritePositionFromGridContainers(GridContainer &a) {
		Grid<PuzzlePiece> states(gridContainerToGridPuzzle(a));

		for (auto &state : states) {
			sprites_[state.id].setPosition(
					state.position.x * pixelTileSize_.x + GridSpriteManager::pixelBorderSize_ * floor(state.position.x),
					state.position.y * pixelTileSize_.y +
					GridSpriteManager::pixelBorderSize_ * floor(state.position.y));
		}
	}

	void GridSpriteManager::updateSpritePositionFromGridContainers(GridContainer &a, GridContainer &b, float ratio) {
		Grid<PuzzlePiece> previous_state(gridContainerToGridPuzzle(a));
		Grid<PuzzlePiece> current_state(gridContainerToGridPuzzle(b));
		Grid<PuzzlePiece> interpolated_state = interpolateGridPuzzle(current_state, previous_state, ratio);

		for (auto &state : interpolated_state) {
			sprites_[state.id].setPosition(
					state.position.x * pixelTileSize_.x + GridSpriteManager::pixelBorderSize_ * floor(state.position.x),
					state.position.y * pixelTileSize_.y +
					GridSpriteManager::pixelBorderSize_ * floor(state.position.y));
		}
	}

	void GridSpriteManager::renderTarget(sf::RenderTarget &render, sf::Vector2f const &scale) {
		size_t index = 0;

		for (auto const &sprite : sprites_) {
			sf::Sprite copy(sprite);
			copy.setScale(scale);
			copy.setPosition(copy.getPosition().x * scale.x, copy.getPosition().y * scale.y);
			if (!index)
				copy.setColor(sf::Color(copy.getColor().r, copy.getColor().g, copy.getColor().b, 96));
			render.draw(copy);
			index++;
		}
	}

	size_t GridSpriteManager::pixelBorderSize_ = 3;
}