#pragma once
#include <SFML/Graphics.hpp>
#include "Vector4f.h"
static class Vector3Extensions
{
public:

	static float scalarProduct(sf::Vector3f& left, sf::Vector3f& right);

	static sf::Vector3f crossProduct(sf::Vector3f& left, sf::Vector3f& right);

	static void Normalize(sf::Vector3f& vector);

	static sf::Vector3i* ToVector3i(Vector4f& v);

	static sf::Vector3i* Multiply(sf::Vector3i& v, float f);

	static int FindZ(float x, float y, sf::Vector3f* points);
};

