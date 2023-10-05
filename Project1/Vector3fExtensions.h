#pragma once
#include <SFML/Graphics.hpp>
#include "Vector4f.h"
static class Vector3Extensions
{
public:

	static double scalarProduct(sf::Vector3f& left, sf::Vector3f& right);

	static float scalarProduct(sf::Vector3i& left, sf::Vector3i& right);

	static sf::Vector3f crossProduct(sf::Vector3f& left, sf::Vector3f& right);

	static sf::Vector3f crossProduct(sf::Vector3i& left, sf::Vector3i& right);

	static void Normalize(sf::Vector3f& vector);

	static void Normalize(sf::Vector3i& vector);

	static sf::Vector3i ToVector3i(Vector4f& v);

	static sf::Vector3i Multiply(sf::Vector3i& v, float f);

	static double FindZ(float x, float y, sf::Vector3f* points);

	static double len(sf::Vector3f v);
};

