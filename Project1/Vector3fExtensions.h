#pragma once
#include <SFML/Graphics.hpp>
static class Vector3fExtensions
{
public:

	static float scalarProduct(sf::Vector3f& left, sf::Vector3f& right);

	static sf::Vector3f crossProduct(sf::Vector3f& left, sf::Vector3f& right);

	static void Normalize(sf::Vector3f& vector);
};

