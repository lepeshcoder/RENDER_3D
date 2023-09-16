#include "Vector4f.h"

Vector4f::Vector4f()
{
	this->x = this->y = this->z = this->w = 0;
}

Vector4f::Vector4f(sf::Vector3f vec)
{
	this->x = vec.x;
	this->y = vec.y;
	this->z = vec.z;
	this->w = 1;
}

Vector4f::Vector4f(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vector4f Vector4f::Multiply(Vector4f& v, Matrix4x4& m)
{
	Vector4f* result = new Vector4f();
	result->x = m.arr[0][0] * v.x + m.arr[0][1] * v.y + m.arr[0][2] * v.z + m.arr[0][3] * v.w;
	result->y = m.arr[1][0] * v.x + m.arr[1][1] * v.y + m.arr[1][2] * v.z + m.arr[1][3] * v.w;
	result->z = m.arr[2][0] * v.x + m.arr[2][1] * v.y + m.arr[2][2] * v.z + m.arr[2][3] * v.w;
	result->w = m.arr[3][0] * v.x + m.arr[3][1] * v.y + m.arr[3][2] * v.z + m.arr[3][3] * v.w;
	return *result;
}

void Vector4f::Normalize()
{
	float length = sqrt(x * x + y * y + z * z + w * w);
	x /= length;
	y /= length;
	z /= length;
	w /= length;
}

void Vector4f::Transform(Matrix4x4& m)
{
	float x = m.arr[0][0] * this->x + m.arr[0][1] * this->y + m.arr[0][2] * this->z + m.arr[0][3] * this->w;
	float y = m.arr[1][0] * this->x + m.arr[1][1] * this->y + m.arr[1][2] * this->z + m.arr[1][3] * this->w;
	float z = m.arr[2][0] * this->x + m.arr[2][1] * this->y + m.arr[2][2] * this->z + m.arr[2][3] * this->w;
	float w = m.arr[3][0] * this->x + m.arr[3][1] * this->y + m.arr[3][2] * this->z + m.arr[3][3] * this->w;
	this->x = x;															
	this->y = y;
	this->z = z;
	this->w = w;
}
