#pragma once
#include <SFML/Graphics.hpp>
#include<math.h>
#include "Vector4f.h"

static class Drawer
{
private:
	static void swap(int& a, int& b);

public:
	static void DrawLine(int x1, int y1, int x2, int y2, sf::Image& image);

	static void DrawModel(std::vector<std::vector<sf::Vector3i>>& polygons, std::vector<Vector4f>& vertexes, sf::Image& image);
};

