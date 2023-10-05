#pragma once
#include <SFML/Graphics.hpp>
#include<math.h>
#include "Vector4f.h"
#include"Vector3fExtensions.h"
#include"MatrixTranslations.h"

static class Drawer
{
private:
	static void swap(int& a, int& b);

public:
	static void DrawLine(int x1, int y1, int x2, int y2, sf::Image& image);

	static void DrawModel(std::vector<std::vector<sf::Vector3i>>& polygons, std::vector<Vector4f>& deviceVertexes,
		std::vector<Vector4f>& worldVertexes, std::map<int, sf::Vector3f>& vertexNormals,
		std::vector<sf::Vector3f>& normals,
		sf::Image& image,sf::Vector3f& camera, sf::Vector3f& light,Matrix4x4& inverse);

	static void SetPixelSecure(sf::Image& image, int x, int y,sf::Color color);
	
};

