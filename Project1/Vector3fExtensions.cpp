#include "Vector3fExtensions.h"

float Vector3fExtensions::scalarProduct(sf::Vector3f& left, sf::Vector3f& right)
{
	return left.x * right.x + left.y * right.y + left.z * right.z;
}

sf::Vector3f Vector3fExtensions::crossProduct(sf::Vector3f& left, sf::Vector3f& right)
{
	sf::Vector3f* result = new sf::Vector3f(left.y * right.z - left.z * right.y,
		left.z * right.x - left.x * right.z,
		left.x * right.y - left.y * right.x);
	return *result;
}

void Vector3fExtensions::Normalize(sf::Vector3f& vector)
{	
	float length = sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	vector.x /= length;
	vector.y /= length;
	vector.z /= length;
}
