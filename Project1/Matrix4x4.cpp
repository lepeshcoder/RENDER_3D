#include "Matrix4x4.h"

Matrix4x4::Matrix4x4()
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			arr[i][j] = 0;
		}
	}
}

Matrix4x4::Matrix4x4(const float inputArr[4][4])
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			arr[i][j] = inputArr[i][j];
		}
	}
}

Matrix4x4 Matrix4x4::Multiply(Matrix4x4& left, Matrix4x4& right)
{
	Matrix4x4* result = new Matrix4x4();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result->arr[i][j] = left.arr[i][0] * right.arr[0][j] +
				left.arr[i][1] * right.arr[1][j] +
				left.arr[i][2] * right.arr[2][j] +
				left.arr[i][3] * right.arr[3][j];
		}
	}
	return *result;
}

void Matrix4x4::ViewMatrix()
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			std::cout << arr[i][j] << " ";
		}
		std::cout << "\n";
	}
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const
{
	Matrix4x4* result = new Matrix4x4();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result->arr[i][j] = arr[i][0] * other.arr[0][j] +
				arr[i][1] * other.arr[1][j] +
				arr[i][2] * other.arr[2][j] +
				arr[i][3] * other.arr[3][j];
		}
	}
	return *result;
}
