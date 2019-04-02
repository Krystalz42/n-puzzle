#pragma once

#include <SFML/Graphics.hpp>
#include "IDisplay.hpp"
#include <functional>

#ifndef N_PUZZLE_ROOT
	#define N_PUZZLE_ROOT ""
#endif

class DisplaySfml : public IDisplay {
public:
	class SfmlException : public std::exception {
	public:
		SfmlException() noexcept;
		explicit SfmlException(std::string const &s) noexcept;
		const char* what() const noexcept override;
		~SfmlException() noexcept override = default;
		SfmlException(SfmlException const &src) = default;
		SfmlException &operator=(SfmlException const &rhs) = delete;
	private:
		std::string			error_;
	};

	DisplaySfml() = delete;
	DisplaySfml(unsigned int width, unsigned int height, char const *windowName);
	~DisplaySfml() override;
	DisplaySfml &operator=(DisplaySfml const &rhs) = delete;
	DisplaySfml(DisplaySfml const &src) = delete;

	bool exit() const override;

	void updateInput() override;

	void render() override;

	void setCallbackEvent(std::function<void(sf::Event &)> callbackEvent);

	sf::RenderWindow &getWindow();

private:
	bool exit_;
	bool restart_;
	sf::RenderWindow win_;
	sf::Event ev_;
	std::function<void(sf::Event &)> callback_;

	void error_(std::string const &s = std::string("Error"));
};