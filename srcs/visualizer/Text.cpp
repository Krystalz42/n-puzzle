#include "Text.hpp"

Text::Text(sf::Font const &font, sf::Vector2f const &position) :
font_(font),
position_(position) {
	text_.setFont(font_);
	text_.setStyle(sf::Text::Regular);
	setText("Text");
}
Text::Text(sf::Font const &font, sf::Vector2f const &position, std::string const &text, unsigned int sizeFont) :
font_(font),
position_(position) {
	text_.setFont(font_);
	text_.setStyle(sf::Text::Regular);
	setText(text, sizeFont);
}

sf::Vector2f Text::getPosition() const {
	return position_;
}
void Text::setText(std::string const &text, unsigned int sizeFont) {
	text_.setString(text);
	text_.setCharacterSize(sizeFont);

	centerText_();
}

void Text::render(sf::RenderTarget &render) {
	render.draw(text_);
}

void Text::centerText_() {
	sf::FloatRect textRect = text_.getLocalBounds();
	text_.setOrigin(textRect.left + textRect.width / 2.0f,
					textRect.top  + textRect.height / 2.0f);
	text_.setPosition(position_);
}