#include "MatrixTranslations.h"

Matrix4x4 MatrixTranslations::SetScale(float scaleX, float scaleY, float scaleZ)
{
	Matrix4x4* result = new Matrix4x4();
	result->arr[0][0] = scaleX;
	result->arr[1][1] = scaleY;
	result->arr[2][2] = scaleZ;
	result->arr[3][3] = 1;
	return *result;
}

Matrix4x4 MatrixTranslations::SetPosition(float dx, float dy, float dz)
{
	Matrix4x4* result = new Matrix4x4();
	result->arr[0][0] = result->arr[1][1] = result->arr[2][2] = result->arr[3][3] = 1;
	result->arr[3][0] = dx;
	result->arr[3][1] = dy;
	result->arr[3][2] = dz;
	return *result;
}

Matrix4x4 MatrixTranslations::SetRotationX(float angle)
{
	Matrix4x4* result = new Matrix4x4();
	result->arr[0][0] = result->arr[3][3] = 1;
	result->arr[1][1] = result->arr[2][2] = cos(angle);
	result->arr[1][2] = -sin(angle);
	result->arr[2][1] = sin(angle);
	return *result;
}

Matrix4x4 MatrixTranslations::SetRotationY(float angle)
{
	Matrix4x4* result = new Matrix4x4();
	result->arr[1][1] = result->arr[3][3] = 1;
	result->arr[0][0] = result->arr[2][2] = cos(angle);
	result->arr[0][2] = sin(angle);
	result->arr[2][0] = -sin(angle);
	return *result;
}

Matrix4x4 MatrixTranslations::SetRotationZ(float angle)
{
	Matrix4x4* result = new Matrix4x4();
	result->arr[2][2] = result->arr[3][3] = 1;
	result->arr[0][0] = result->arr[1][1] = cos(angle);
	result->arr[0][1] = -sin(angle);
	result->arr[1][0] = sin(angle);
	return *result;
}

Matrix4x4 MatrixTranslations::CreateLookAt(sf::Vector3f cameraPosition, sf::Vector3f cameraTarget,
	sf::Vector3f cameraUpVector)
{
	Matrix4x4* result = new Matrix4x4();
	sf::Vector3f zAxis = cameraPosition - cameraTarget;
	Vector3Extensions::Normalize(zAxis);

	sf::Vector3f xAxis = Vector3Extensions::crossProduct(cameraUpVector, zAxis);
	Vector3Extensions::Normalize(xAxis);

	sf::Vector3f yAxis = Vector3Extensions::crossProduct(zAxis, xAxis);
	
	result->arr[0][0] = xAxis.x; result->arr[0][1] = xAxis.y; result->arr[0][2] = xAxis.z; result->arr[0][3] = -Vector3Extensions::scalarProduct(xAxis, cameraPosition);
	result->arr[1][0] = yAxis.x, result->arr[1][1] = yAxis.y, result->arr[1][2] = yAxis.z; result->arr[1][3] = -Vector3Extensions::scalarProduct(yAxis, cameraPosition);
	result->arr[2][0] = zAxis.x; result->arr[2][1] = zAxis.y; result->arr[2][2] = zAxis.z; result->arr[2][3] = -Vector3Extensions::scalarProduct(zAxis, cameraPosition);
	result->arr[3][0] = result->arr[3][1] = result->arr[3][2] = 0; result->arr[3][3] = 1;

	return *result;
}

Matrix4x4 MatrixTranslations::CreatePerspectiveFieldOfView(float fieldOfView, float aspectRatio,
	float nearPlaneDistance, float farPlaneDistance)
{
	Matrix4x4* result = new Matrix4x4();
	result->arr[0][0] = 1. / (aspectRatio * tan(fieldOfView / 2.));
	result->arr[1][1] = 1. / tan(fieldOfView / 2);
	result->arr[2][2] = farPlaneDistance / (nearPlaneDistance - farPlaneDistance);
	result->arr[2][3] = nearPlaneDistance * farPlaneDistance / (nearPlaneDistance - farPlaneDistance);
	result->arr[3][2] = -1;
	return *result;
}

Matrix4x4 MatrixTranslations::CreateDeviceCoordinates(int screenWidth, int screenHeight, int xMin, int Ymin)
{
	Matrix4x4* result = new Matrix4x4();
	result->arr[0][0] = screenWidth / 2.;
	result->arr[0][3] = xMin + screenWidth / 2.;
	result->arr[1][1] = -screenHeight / 2.;
	result->arr[1][3] = Ymin + screenHeight / 2.;
	result->arr[2][2] = result->arr[3][3] = 1;
	return *result;
}

void MatrixTranslations::TransformVertex(std::vector<Vector4f>& vertexes, Matrix4x4& transformMatrix)
{
	for (int i = 0; i < vertexes.size(); i++)
	{
		vertexes[i].Transform(transformMatrix);
		float w = vertexes[i].w;
		vertexes[i].x /= w;
		vertexes[i].y /= w;
		vertexes[i].z /= w;
	}
}

Matrix4x4 MatrixTranslations::Kal(sf::Vector3f xAxis, sf::Vector3f yAxis, sf::Vector3f zAxis, sf::Vector3f position)
{
	Matrix4x4* matrix = new Matrix4x4();
	matrix->arr[0][0] = xAxis.x;
	matrix->arr[1][0] = xAxis.y;
	matrix->arr[2][0] = xAxis.z;
	matrix->arr[3][0] = 0;

	matrix->arr[0][1] = yAxis.x;
	matrix->arr[1][1] = yAxis.y;
	matrix->arr[2][1] = yAxis.z;
	matrix->arr[3][1] = 0;

	matrix->arr[0][2] = zAxis.x;
	matrix->arr[1][2] = zAxis.y;
	matrix->arr[2][2] = zAxis.z;
	matrix->arr[3][2] = 0;

	matrix->arr[0][3] = position.x;
	matrix->arr[1][3] = position.y;
	matrix->arr[2][3] = position.z;
	matrix->arr[3][3] = 1;

	return *matrix;
}

sf::Vector3f MatrixTranslations::GetCameraPositionFromSpheric(float r, float a, float b)
{
	sf::Vector3f* camera = new sf::Vector3f();
	camera->z = r * sin(a * 3.14 / 180) * cos(b * 3.14 / 180);
	camera->x = r * sin(a * 3.14 / 180) * sin(b * 3.14 / 180);
	camera->y = r * cos(a * 3.14 / 180);
	return *camera;
}

sf::Vector3f MatrixTranslations::GetBarCoords(sf::Vector3f& pointA, sf::Vector3f& pointB, sf::Vector3f& pointC, sf::Vector3f& pointO)
{
	sf::Vector3f AO = pointA - pointO;
	sf::Vector3f BO = pointB - pointO;
	sf::Vector3f CO = pointC - pointO;

	sf::Vector3f edgeABC1 = pointB - pointA;
	sf::Vector3f edgeABC2 = pointC - pointA;

	sf::Vector3f crossABC = Vector3Extensions::crossProduct(edgeABC1, edgeABC2);
	double areaABC = 0.5 * Vector3Extensions::len(crossABC);

	sf::Vector3f crossAOB = Vector3Extensions::crossProduct(AO, BO);
	double areaAOB = 0.5 * Vector3Extensions::len(crossAOB);
	
	sf::Vector3f crossAOC = Vector3Extensions::crossProduct(AO, CO);
	double areaAOC= 0.5 * Vector3Extensions::len(crossAOC);

	sf::Vector3f crossCOB = Vector3Extensions::crossProduct(CO, BO);
	double areaCOB = 0.5 * Vector3Extensions::len(crossCOB);

	return sf::Vector3f(areaCOB / areaABC, areaAOC / areaABC, areaAOB / areaABC);
}

sf::Vector3f MatrixTranslations::GetPointNormal(sf::Vector3f& barCoords, sf::Vector3f& normalA, sf::Vector3f& normalB, sf::Vector3f& normalC)
{
	return sf::Vector3f(
		barCoords.x * normalA.x + barCoords.x * normalB.x + barCoords.x * normalC.x,
		barCoords.y * normalA.y + barCoords.y * normalB.y + barCoords.y * normalC.y,
		barCoords.z * normalA.z + barCoords.z * normalB.z + barCoords.z * normalC.z);
}

Matrix4x4 MatrixTranslations::InverseMatrix(Matrix4x4& m)
{
	int n = 4;// Размерность матрицы (предполагается, что матрица квадратная)

	// Создаем расширенную матрицу, которая содержит исходную матрицу и единичную матрицу справа
	std::vector<std::vector<double>> augmentedMatrix(n, std::vector<double>(2 * n, 0.0));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			augmentedMatrix[i][j] = m.arr[i][j];
		}
		augmentedMatrix[i][i + n] = 1.0;
	}

	// Прямой ход метода Гаусса-Джордана
	for (int i = 0; i < n; ++i) {
		// Делаем главный элемент строки равным 1
		double pivot = augmentedMatrix[i][i];
		for (int j = 0; j < 2 * n; ++j) {
			augmentedMatrix[i][j] /= pivot;
		}

		// Обнуляем все элементы в столбце, кроме главного элемента
		for (int k = 0; k < n; ++k) {
			if (k != i) {
				double factor = augmentedMatrix[k][i];
				for (int j = 0; j < 2 * n; ++j) {
					augmentedMatrix[k][j] -= factor * augmentedMatrix[i][j];
				}
			}
		}
	}

	// Извлекаем инверсию матрицы из правой части расширенной матрицы
	std::vector<std::vector<double>> inverse(n, std::vector<double>(n, 0.0));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			inverse[i][j] = augmentedMatrix[i][j + n];
		}
	}

	Matrix4x4 result;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.arr[i][j] = inverse[i][j];
		}
	}

	return result;
}


