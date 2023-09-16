#pragma once
#include"Matrix4x4.h"
#include <SFML/Graphics.hpp>
class Vector4f
{

	

public:

	float x, y, z, w;

	Vector4f();

	Vector4f(sf::Vector3f vec);

	Vector4f(float x, float y, float z, float w);
	
	static Vector4f Multiply(Vector4f& vector, Matrix4x4& matrix);

	void Normalize();

	void Transform(Matrix4x4& transfromMatrix);
};

