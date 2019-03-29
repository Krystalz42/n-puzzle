#include "SfVectorInterpolate.hpp"
#include <math.h>

sf::Vector2f linearInterpolate(sf::Vector2f const &b, sf::Vector2f const &a, float ratio) {
	return a * ratio + b * (1.f - ratio);
}

sf::Vector2f cosineInterpolate(sf::Vector2f b, sf::Vector2f a, float ratio) {
	float ratio2;

	ratio2 = (1 - cos(ratio * M_PI)) / 2;
	return sf::Vector2f(b.x * (1 - ratio2) + a.x * ratio2, b.y * (1 - ratio2) + a.y * ratio2);
}