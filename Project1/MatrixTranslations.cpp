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
	sf::Vector3f zAxis(cameraPosition.x - cameraTarget.x,cameraPosition.y - cameraTarget.y,cameraPosition.z - cameraTarget.z);
	sf::Vector3f zAxisNorm = Vector3fExtensions::Normalize(zAxis);
	sf::Vector3f xAxis = Vector3fExtensions::crossProduct(cameraUpVector, zAxisNorm);
	sf::Vector3f xAxisNorm = Vector3fExtensions::Normalize(xAxis);
	
	result->arr[0][0] = xAxisNorm.x; result->arr[0][1] = xAxisNorm.y; result->arr[0][2] = xAxisNorm.z; result->arr[0][3] = -Vector3fExtensions::scalarProduct(xAxisNorm, cameraPosition);
	result->arr[1][0] = cameraUpVector.x, result->arr[1][1] = cameraUpVector.y, result->arr[1][2] = cameraUpVector.z; result->arr[1][3] = -Vector3fExtensions::scalarProduct(cameraUpVector, cameraPosition);
	result->arr[2][0] = zAxisNorm.x; result->arr[2][1] = zAxisNorm.y; result->arr[2][2] = zAxisNorm.z; result->arr[2][3] = -Vector3fExtensions::scalarProduct(zAxisNorm, cameraPosition);
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
	camera->x = r * sin(a * 3.14 / 180) * cos(b * 3.14 / 180);
	camera->y = r * sin(a * 3.14 / 180) * sin(b * 3.14 / 180);
	camera->z = r * cos(a * 3.14 / 180);
	return *camera;
}


