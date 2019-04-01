#include "GridSprite.hpp"
#include <cassert>
#include "SfVectorInterpolate.hpp"
#include <math.h>
#include <resolver/KStar.hpp>

void GridSprite::setSpriteRect_() {


	KStar::Builder builder;
	builder.setSize(tileSize_.x);
	KStar::node_pointer nodePointer = builder.buildGoalGrid();

	std::cout << nodePointer->grid << std::endl;

	for (unsigned int y = 0; y < tileSize_.y; y++) {
		for (unsigned int x = 0; x < tileSize_.x; x++) {
			size_t index = nodePointer->grid[x + y * tileSize_.x];
			sprites_[index].setTextureRect(sf::IntRect(x * pixelTileSize_.x, y * pixelTileSize_.y, pixelTileSize_.x, pixelTileSize_.y));
			sprites_[index].setTexture(texture_);
		}
	}
}

//void GridSprite::init();

void GridSprite::updateSpritePositionFromGridContainers(GridContainer &a) {
	GridVisualizer states(gridContainerToGridVisualizer(a));

	for (auto &state : states) {
		sprites_[state.id].setPosition(state.position.x * pixelTileSize_.x + GridSprite::pixelBorderSize_ * floor(state.position.x),
									   state.position.y * pixelTileSize_.y + GridSprite::pixelBorderSize_ * floor(state.position.y));
	}
}

void GridSprite::updateSpritePositionFromGridContainers(GridContainer &a, GridContainer &b, float ratio) {
	GridVisualizer previous_state(gridContainerToGridVisualizer(a));
	GridVisualizer current_state(gridContainerToGridVisualizer(b));
	GridVisualizer interpolated_state = interpolateGridVisualizer(current_state, previous_state, ratio);

	for (auto &state : interpolated_state) {
		sprites_[state.id].setPosition(state.position.x * pixelTileSize_.x + GridSprite::pixelBorderSize_ * floor(state.position.x),
									  state.position.y * pixelTileSize_.y + GridSprite::pixelBorderSize_ * floor(state.position.y));
	}
}

void GridSprite::renderTarget(sf::RenderTarget &render, sf::Vector2f const &scale) {
	size_t index = 0;

	for (auto const &sprite : sprites_) {
		if (index) {
			sf::Sprite copy(sprite);
			copy.setScale(scale);
			copy.setPosition(copy.getPosition().x * scale.x, copy.getPosition().y * scale.y);
			render.draw(copy);
		}
		index++;
	}
}

size_t GridSprite::pixelBorderSize_ = 3;