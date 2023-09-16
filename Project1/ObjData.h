#pragma once
#include<vector>
#include <SFML/Graphics.hpp>

struct ObjData
{
public:
	std::vector<sf::Vector3f> vertexes;

	std::vector<sf::Vector3f> normals;

	std::vector<sf::Vector3f> textures;

	std::vector<std::vector<sf::Vector3i>> polygons;

	ObjData(std::vector<sf::Vector3f> vertexes, std::vector<sf::Vector3f> normals,
		std::vector<sf::Vector3f> textures, std::vector<std::vector<sf::Vector3i>> polygons)
	{
		this->vertexes = vertexes;
		this->normals = normals;
		this->textures = textures;
		this->polygons = polygons;
	}
};