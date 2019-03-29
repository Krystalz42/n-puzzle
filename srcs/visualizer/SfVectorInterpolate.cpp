#include "SfVectorInterpolate.hpp"

sf::Vector2f interpolateVector2f(sf::Vector2f const &a, sf::Vector2f const &b, float ratio) {
	return a * ratio + b * (1.f - ratio);
}