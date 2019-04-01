#pragma once

#include <SFML/Graphics.hpp>
#include <visualizer/GridSprite.hpp>

namespace Vizualizer {

	class Map {
	public:
		Map();

		float getZoom() const;
		sf::Vector2i getPosition() const;

	private:
		sf::Vector2i position_;
		float	zoom_;

		static float zoomMax_;
		static float zoomMin_;
	};

	class Node {
	public:
		Node(Map const &map, sf::Vector2i const &position, sf::Texture const &texture, sf::Vector2u const &tileSize, sf::Font const &font);
		void update(float ratio);
		void render(sf::RenderTarget &render);

	private:
		//sf::IntRect		bounds;
		Map				const &map_;
		sf::Font		const &font_;
		GridSprite		gs_;
		sf::Vector2i	position_;
		sf::Vertex		line_[10];
		float			sizePuzzle_;
		float			sizeData_;

		static float	percentSizePuzzle_;
		static float	percentSizeData_;
	};

}
