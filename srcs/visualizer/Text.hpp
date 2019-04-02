#pragma once

#include <SFML/Graphics.hpp>

class Text {
public:
	Text(sf::Font const &font, sf::Vector2f const &position);
	Text(sf::Font const &font, sf::Vector2f const &position, std::string const &text, unsigned int sizeFont = 11);

	sf::Vector2f getPosition() const;
	void setText(std::string const &text, unsigned int sizeFont = 11);
	void render(sf::RenderTarget &render);

private:
	sf::Font const &font_;
	sf::Text text_;
	sf::Vector2f position_;

	void centerText_();
};