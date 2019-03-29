#pragma once

#include <SFML/Graphics.hpp>

sf::Vector2f linearInterpolate(sf::Vector2f const &a, sf::Vector2f const &b, float ratio);

sf::Vector2f cosineInterpolate(sf::Vector2f y1,sf::Vector2f y2, float ratio);