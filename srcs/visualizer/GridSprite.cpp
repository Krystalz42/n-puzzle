#include "GridSprite.hpp"
#include <cassert>
#include "SfVectorInterpolate.hpp"
#include <math.h>

void GridSprite::setSpriteRect_() {
	size_t index = 0;
	for (auto & sprite : sprites_) {
		sprite.setTexture(texture_);
		sprite.setTextureRect(sf::IntRect((index % 3) * pixelTileSize_.x, (index / tileSize_.x) * pixelTileSize_.y, pixelTileSize_.x, pixelTileSize_.y));
		index++;
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

void GridSprite::renderTarget(sf::RenderTarget &render) {
	size_t index = 0;
	for (auto const &sprite : sprites_) {
		if (index)
			render.draw(sprite);
		index++;
	}
}

size_t GridSprite::pixelBorderSize_ = 3;