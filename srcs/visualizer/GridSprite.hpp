#pragma once
#include "GridVisualizer.hpp"

class GridSprite {
public:
	GridSprite(sf::Texture const &texture, sf::Vector2u const &tileSize) :
	texture_(texture),
	tileSize_(tileSize),
	pixelTileSize_(texture_.getSize().x / tileSize_.x, texture_.getSize().y / tileSize_.y),
	sprites_(tileSize_.x * tileSize_.y) {
		setSpriteRect_();
	};
	~GridSprite() = default;
	GridSprite &operator=(GridSprite const &rhs) = delete;
	GridSprite(GridSprite const &src) = delete;

	void updateSpritePositionFromGridContainers(GridContainer &a, GridContainer &b, float ratio);
	void renderTarget(sf::RenderTarget &render, sf::Vector2f const &scale);
	void updateSpritePositionFromGridContainers(GridContainer &a);

private:
	sf::Texture texture_;
	sf::Vector2u tileSize_;
	sf::Vector2u pixelTileSize_;
	std::vector< sf::Sprite > sprites_;

	static size_t pixelBorderSize_;

	void setSpriteRect_();
};
