#pragma once

#include <SFML/Graphics.hpp>
#include <visualizer/GridSpriteManager.hpp>
#include <resolver/KStar.hpp>
#include <chrono>
#include <boost/filesystem.hpp>

namespace Visualizer {

	class TimeLogic {
	public:
		TimeLogic();
		void update();
		bool needLogicUpdate();
		float getRatio() const;

		void setStep(std::chrono::milliseconds milli);
		void setPause(bool b);
		std::chrono::milliseconds getStep() const;
	private:
		std::chrono::nanoseconds lag_;
		std::chrono::steady_clock::time_point update_;
		std::chrono::nanoseconds step_;
		bool wantPause_;
		bool pause_;
	};

	class Text {
	public:
		Text(sf::Font const &font, sf::Vector2f const &position) :
		font_(font),
		position_(position) {
			text_.setFont(font_);
			text_.setStyle(sf::Text::Regular);
			setText("Text");
		}
		Text(sf::Font const &font, sf::Vector2f const &position, std::string const &text, unsigned int sizeFont = 11) :
				font_(font),
				position_(position) {
			text_.setFont(font_);
			text_.setStyle(sf::Text::Regular);
			setText(text, sizeFont);
		}

		sf::Vector2f getPosition() const {
			return position_;
		}
		void setText(std::string const &text, unsigned int sizeFont = 11) {
			text_.setString(text);
			text_.setCharacterSize(sizeFont);

			centerText_();
		}

		void render(sf::RenderTarget &render) {
			render.draw(text_);
		}

	private:
		sf::Font const &font_;
		sf::Text text_;
		sf::Vector2f position_;

		void centerText_() {
			sf::FloatRect textRect = text_.getLocalBounds();
			text_.setOrigin(textRect.left + textRect.width / 2.0f,
							textRect.top  + textRect.height / 2.0f);
			text_.setPosition(position_);
		}
	};

	constexpr unsigned int pixelBorder = 3;
	constexpr unsigned int sizeInfo = 30;
	class Vizualizer {
	public:
		Vizualizer(unsigned int tileSize);
		void loop(KStar::ResolverData &resolver);
	private:
		TimeLogic timeLogic_;
		boost::filesystem::path pathRoot_;
		std::unique_ptr<DisplaySfml> display_;
		sf::Texture texturePuzzle_;
		sf::Font  font_;
		std::unique_ptr<GridSpriteManager> gs_;

		unsigned int tileSize_;
		bool restart_;
		bool pause_;

		std::unique_ptr<Text> complexityInTime_;
		std::unique_ptr<Text> complexityInSize_;
		std::unique_ptr<Text> numberOfMove_;
		std::unique_ptr<Text> speed_;
		std::unique_ptr<Text> nameHeuristique_;

		void initText_(KStar::ResolverData &resolver);
	};
}
