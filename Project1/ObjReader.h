#pragma once
#include"ObjData.h"
#include<fstream>
#include<iostream>
#include<sstream>
#include "Matrix4x4.h"
#include "Vector4f.h"

static class ObjReader
{

public:
	static ObjData ReadFile(std::string Path);

	static std::vector<std::vector<sf::Vector2i>> GetDeviceCoordinates(ObjData& data, Matrix4x4& totalTranslationMatrix);
};

