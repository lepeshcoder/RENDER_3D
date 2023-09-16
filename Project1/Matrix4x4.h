#pragma once
#include<iostream>

class Matrix4x4
{


public:

	float arr[4][4];

	Matrix4x4();

	Matrix4x4(const float inputArr[4][4]);

	static Matrix4x4 Multiply(Matrix4x4& left, Matrix4x4& right);

	void ViewMatrix();
	
	Matrix4x4 operator*(const Matrix4x4& other) const;
	
};

