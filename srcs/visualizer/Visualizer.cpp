#include "Visualizer.hpp"

namespace Vizualizer {

	Map::Map() :
		position_(0, 0),
		zoom_(100.f)
	{}

	float Map::getZoom() const {
		return zoom_;
	}

	sf::Vector2i Map::getPosition() const {
		return position_;
	}

	float Map::zoomMax_ = 500;
	float Map::zoomMin_ = 30;

	Node::Node(Map const &map, sf::Vector2i const &position, sf::Texture const &texture, sf::Vector2u const &tileSize, sf::Font const &font) :
		map_(map),
		font_(font),
		gs_(texture, tileSize),
		position_(position) {
	}

	void Node::update(float ratio) {

		sizePuzzle_ = Node::percentSizePuzzle_ * map_.getZoom();
		sizeData_ = Node::percentSizeData_ * map_.getZoom();

		int i = 0;
		line_[i++] = sf::Vertex(sf::Vector2f(map_.getPosition().x + position_.x, map_.getPosition().y + position_.y));
		line_[i++] = sf::Vertex(sf::Vector2f(map_.getPosition().x + position_.x, map_.getPosition().y + position_.y + sizePuzzle_ + sizeData_));
		line_[i++] = sf::Vertex(sf::Vector2f(map_.getPosition().x + position_.x + sizePuzzle_, map_.getPosition().y + position_.y + sizePuzzle_ + sizeData_));
		line_[i++] = sf::Vertex(sf::Vector2f(map_.getPosition().x + position_.x + sizePuzzle_, map_.getPosition().y + position_.y));
		line_[i++] = sf::Vertex(sf::Vector2f(map_.getPosition().x + position_.x, map_.getPosition().y + position_.y));
		line_[i++] = sf::Vertex(sf::Vector2f(map_.getPosition().x + position_.x + sizePuzzle_, map_.getPosition().y + position_.y));
		line_[i++] = sf::Vertex(sf::Vector2f(map_.getPosition().x + position_.x, map_.getPosition().y + position_.y + sizePuzzle_ + sizeData_));
		line_[i++] = sf::Vertex(sf::Vector2f(map_.getPosition().x + position_.x + sizePuzzle_, map_.getPosition().y + position_.y + sizePuzzle_ + sizeData_));
		line_[i++] = sf::Vertex(sf::Vector2f(map_.getPosition().x + position_.x, map_.getPosition().y + position_.y + sizePuzzle_));
		line_[i++] = sf::Vertex(sf::Vector2f(map_.getPosition().x + position_.x + sizePuzzle_, map_.getPosition().y + position_.y + sizePuzzle_));

		//gs_.updateSpritePositionFromGridContainers()
	}

	void Node::render(sf::RenderTarget &render) {

		//if (line_[2].position.x < 0 && line_[2].position.y < 0)
		//	return ;
		render.draw(line_, 10, sf::Lines);

		float ratioRender = render.getSize().y;
		if (render.getSize().x > render.getSize().y)
			ratioRender = render.getSize().x;
		gs_.renderTarget(render, sf::Vector2f(sizePuzzle_ / ratioRender, sizePuzzle_ / ratioRender));



		sf::Text text1;
		text1.setFont(font_);
		text1.setCharacterSize(sizeData_ * 0.8);
		text1.setString("OOOKE");
		text1.setStyle(sf::Text::Regular);

		sf::FloatRect textRect = text1.getLocalBounds();
		text1.setOrigin(textRect.left + textRect.width / 2.0f,
						textRect.top  + textRect.height / 2.0f);
		text1.setPosition(map_.getPosition().x + 10 + sizePuzzle_ / 2, map_.getPosition().y + 10 + sizePuzzle_ + sizeData_ / 2);
		render.draw(text1);
	}

	float	Node::percentSizePuzzle_ = 0.7f;
	float	Node::percentSizeData_ = 0.3f;
}