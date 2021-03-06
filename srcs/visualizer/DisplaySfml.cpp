#include "DisplaySfml.hpp"
#include <boost/filesystem.hpp>

DisplaySfml::DisplaySfml(unsigned int width, unsigned int height, char const *windowName) :
		exit_(false),
		win_(sf::VideoMode(width, height), windowName) {


	boost::filesystem::path pathRoot(N_PUZZLE_ROOT);

	win_.setFramerateLimit(60);
	win_.clear();
	win_.display();
}

DisplaySfml::~DisplaySfml() {
	win_.close();
}

void DisplaySfml::error_(std::string const &s) {
	throw (DisplaySfml::SfmlException(s));
}


bool DisplaySfml::exit() const {
	return exit_;
}

void DisplaySfml::updateInput() {
	while (win_.pollEvent(ev_)) {
		if (ev_.type == sf::Event::Closed)
			exit_ = true;
		if (ev_.type == sf::Event::KeyPressed) {

			switch (ev_.key.code) {
				case sf::Keyboard::Escape:
					exit_ = true;
					break;
				default :
					break;
			}
		}
		if (callback_)
			callback_(ev_);
	}
}

void DisplaySfml::render() {
	win_.display();
}

void DisplaySfml::setCallbackEvent(std::function<void(sf::Event &)> callbackEvent) {
	callback_ = callbackEvent;
}

sf::RenderWindow &DisplaySfml::getWindow() {
	return win_;
}

DisplaySfml::SfmlException::SfmlException() noexcept :
		error_("Error on DisplaySfml constructor") {}
DisplaySfml::SfmlException::SfmlException(std::string const &s) noexcept :
		error_(s) {}
const char	*DisplaySfml::SfmlException::what() const noexcept
{ return (error_.c_str()); }