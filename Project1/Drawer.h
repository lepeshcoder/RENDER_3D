#pragma once
#include <SFML/Graphics.hpp>
#include<math.h>
#include "Vector4f.h"
#include"Vector3fExtensions.h"
#include"MatrixTranslations.h"
#include<unordered_map>

static class Drawer
{
private:
	static void swap(int& a, int& b);

public:

	static void DrawModel(std::vector<std::vector<sf::Vector3i>>& polygons, std::vector<Vector4f>& deviceVertexes,
		std::vector<Vector4f>& worldVertexes, std::unordered_map<int, sf::Vector3f>& vertexNormals,
		sf::Texture& texture,sf::Vector3f& camera, sf::Vector3f& light,Matrix4x4& inverse, std::vector<sf::Vector3f>& normals);

	static void FillPixel(int x1, int x2, int x, int y, sf::Vector3f* fPoints, sf::Vector3i* iPoints, double** zBuffer, Matrix4x4& inverse, std::vector<sf::Vector3f>& normals,
		std::vector<std::vector<sf::Vector3i>>& polygons, int polygon, sf::Vector3f& light,double phi,double z,unsigned char* buffer,
		const sf::Vector2u& size);

	
};

