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

	std::map<int,sf::Vector3f> vertexNormals;

	ObjData(std::vector<sf::Vector3f> vertexes, std::vector<sf::Vector3f> normals,
		std::vector<sf::Vector3f> textures, std::vector<std::vector<sf::Vector3i>> polygons,
		std::map<int, sf::Vector3f> vertexNormals)
	{
		this->vertexes = vertexes;
		this->normals = normals;
		this->textures = textures;
		this->polygons = polygons;
		this->vertexNormals = vertexNormals;
	}
};