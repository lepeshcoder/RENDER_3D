#include "Vector3fExtensions.h"

double Vector3Extensions::scalarProduct(sf::Vector3f& left, sf::Vector3f& right)
{
	return (double)left.x * right.x + left.y * right.y + left.z * right.z;
}

float Vector3Extensions::scalarProduct(sf::Vector3i& left, sf::Vector3i& right)
{
	return left.x * right.x + left.y * right.y + left.z * right.z;
}

sf::Vector3f Vector3Extensions::crossProduct(sf::Vector3f& left, sf::Vector3f& right)
{
	return sf::Vector3f(left.y * right.z - left.z * right.y,
		left.z * right.x - left.x * right.z,
		left.x * right.y - left.y * right.x);

}

sf::Vector3f Vector3Extensions::crossProduct(sf::Vector3i& left, sf::Vector3i& right)
{
	return sf::Vector3f(left.y * right.z - left.z * right.y,
		left.z * right.x - left.x * right.z,
		left.x * right.y - left.y * right.x);
}

void Vector3Extensions::Normalize(sf::Vector3f& vector)
{	
	double length = sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	vector.x /= length;
	vector.y /= length;
	vector.z /= length;
}

void Vector3Extensions::Normalize(sf::Vector3i& vector)
{
	float length = sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	vector.x /= length;
	vector.y /= length;
	vector.z /= length;
}

sf::Vector3i Vector3Extensions::ToVector3i(Vector4f& v)
{
	sf::Vector3i result(round(v.x), round(v.y), round(v.z));
	return result;
}

sf::Vector3i Vector3Extensions::Multiply(sf::Vector3i& v, float f)
{
	sf::Vector3i* result = new sf::Vector3i(v.x * f, v.y * f, v.z * f);
	return *result;
}

double Vector3Extensions::FindZ(float x, float y, sf::Vector3f* points)
{
	sf::Vector3f u = points[1] - points[0];
	sf::Vector3f v = points[2] - points[0];
	sf::Vector3f n = crossProduct(u, v);
	Normalize(n);
	double d = -(n.x * points[0].x + n.y * points[0].y + n.z * points[0].z);
	return (double)(-n.x * x - n.y * y - d) / n.z;
}

double Vector3Extensions::len(sf::Vector3f v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

