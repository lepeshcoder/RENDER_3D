#pragma once
#include"Vector4f.h"
#include"Vector3fExtensions.h"
static class MatrixTranslations
{

public:

	static Matrix4x4 SetScale(float scaleX, float scaleY, float scaleZ);

	static Matrix4x4 SetPosition(float dx, float dy, float dz);

	static Matrix4x4 SetRotationX(float angle);
	
	static Matrix4x4 SetRotationY(float angle);

	static Matrix4x4 SetRotationZ(float angle);

	static Matrix4x4 CreateLookAt(sf::Vector3f cameraPosition, sf::Vector3f cameraTarget,
		sf::Vector3f cameraUpVector);

	static Matrix4x4 CreatePerspectiveFieldOfView(float fieldOfView, float aspectRatio,
		float newarPlaneDistance, float farPlaneDistance);
	
	static Matrix4x4 CreateDeviceCoordinates(int screenWidth, int screenHeight, int xMin, int Ymin);

	static void TransformVertex(std::vector<Vector4f>& vertexes, Matrix4x4& transformMatrix);

	static Matrix4x4 Kal(sf::Vector3f xAxis, sf::Vector3f yAxis, sf::Vector3f zAxis, sf::Vector3f position);

	static sf::Vector3f GetCameraPositionFromSpheric(float r, float a, float b);

	static sf::Vector3f GetBarCoords(sf::Vector3i& pointA, sf::Vector3i& pointB, sf::Vector3i& pointC, sf::Vector3i& pointO);

	static sf::Vector3f GetPointNormal(sf::Vector3f& barCoords, sf::Vector3f& normalA, sf::Vector3f& normalB, sf::Vector3f& normalC);

	static Matrix4x4 InverseMatrix(Matrix4x4& m);
};

