#pragma once
#include <SFML/Graphics.hpp>
#include<math.h>
#include "Vector4f.h"
#include"Vector3fExtensions.h"
#include"MatrixTranslations.h"
#include<unordered_map>
#include "ThreadPool.h"

static class Drawer
{
private:
	static void swap(int& a, int& b);

public:

	static void DrawModel(std::vector<std::vector<sf::Vector3i>>& polygons, std::vector<Vector4f>& deviceVertexes,
		std::vector<Vector4f>& worldVertexes, std::unordered_map<int, sf::Vector3f>& vertexNormals,
		sf::Texture& texture,sf::Vector3f& camera, sf::Vector3f& light,Matrix4x4& inverse, std::vector<sf::Vector3f>& normals,ThreadPool& pool);

    static void DrawLine1(int y, sf::Vector3i* iPoints, sf::Vector3f* fPoints, int total_height, int segmentUp, sf::Vector2u size, double** zBuffer,
        Matrix4x4& inverse, std::vector<sf::Vector3f>& normals, std::vector<std::vector<sf::Vector3i>>& polygons, 
        int polygon, sf::Vector3f& light, sf::Uint8* buffer, sf::Vector3f& camera);

    static void DrawLine2(int y, sf::Vector3i* iPoints, sf::Vector3f* fPoints, int total_height, int segmentDown, sf::Vector2u size, double** zBuffer, Matrix4x4& inverse,
        std::vector<sf::Vector3f>& normals, std::vector<std::vector<sf::Vector3i>>& polygons, int polygon, sf::Vector3f& light, sf::Uint8* buffer, sf::Vector3f& camera);

    static void FillPixel(int x1, int x2, int x, int y, sf::Vector3f* fPoints, sf::Vector3i* iPoints, double** zBuffer, Matrix4x4& inverse, std::vector<sf::Vector3f>& normals,
        std::vector<std::vector<sf::Vector3i>>& polygons, int polygon, sf::Vector3f& light, double phi, double z, unsigned char* buffer,
        const sf::Vector2u& size, sf::Vector3f& camera)
    {
        if (z < zBuffer[y][x])
        {
            sf::Vector3f pointO(x, y, z);
            Vector4f pointOWorld(x, y, z, 1);

            pointOWorld.Transform(inverse);

            double w = pointOWorld.w;
            pointOWorld.x /= w;
            pointOWorld.y /= w;
            pointOWorld.z /= w;
            sf::Vector3f pointOF(pointOWorld.x, pointOWorld.y, pointOWorld.z);

            sf::Vector3i pointI(round(pointO.x), round(pointO.y), round(pointO.z));

            sf::Vector3f barCoords = MatrixTranslations::GetBarCoords(iPoints[0], iPoints[1], iPoints[2], pointI);

            sf::Vector3f normalA = normals[polygons[polygon][0].z - 1];
            Vector3Extensions::Normalize(normalA);

            sf::Vector3f normalB = normals[polygons[polygon][1].z - 1];
            Vector3Extensions::Normalize(normalB);

            sf::Vector3f normalC = normals[polygons[polygon][2].z - 1];
            Vector3Extensions::Normalize(normalC);

            sf::Vector3f pointNormal = MatrixTranslations::GetPointNormal(barCoords, normalA, normalB, normalC);
            Vector3Extensions::Normalize(pointNormal);

            sf::Vector3f pointSight = light - pointOF;
            Vector3Extensions::Normalize(pointSight);

            double pointLightIntensity = std::max(0.0, Vector3Extensions::scalarProduct(pointNormal, pointSight));


            double ambientIntensivity = 0.2;
            sf::Color ambientColor = sf::Color::Yellow;
            sf::Color ambientComponent(ambientColor.r * ambientIntensivity, ambientColor.g * ambientIntensivity, ambientColor.b * ambientIntensivity);
            
            double diffuseIntensivity = 0.5;
            sf::Color diffuseColor = sf::Color::Yellow;
            double diffuseAngle = pointLightIntensity;
            double diffuseCoeff = diffuseIntensivity * diffuseAngle;
            sf::Color diffuseComponent(diffuseColor.r * diffuseCoeff, diffuseColor.g * diffuseCoeff, diffuseColor.b * diffuseCoeff);

            double specularIntensivity = 0.2;
            sf::Color specularColor = sf::Color::Yellow;
            double specularCoeff = 0.5;
            sf::Vector3f L = pointSight;
            sf::Vector3f temp(pointNormal.x * pointLightIntensity * 2, pointNormal.y * pointLightIntensity * 2, pointNormal.z * pointLightIntensity * 2);
            sf::Vector3f R = L - temp;
            Vector3Extensions::Normalize(R);
            sf::Vector3f V = camera - pointOF;
            Vector3Extensions::Normalize(V);
            double mul = Vector3Extensions::scalarProduct(R, V);
            double specularI = pow(mul, specularCoeff) * specularIntensivity;
            sf::Color specularComponent(specularColor.r * specularI, specularColor.g * specularI, specularColor.b * specularI);

            sf::Color generalColor = ambientComponent + diffuseComponent+ specularComponent;


            int start = (y * size.x + x) * 4;
            sf::Uint8 color = pointLightIntensity * 255;
            buffer[start] = generalColor.r;
            buffer[start + 1] = generalColor.g;
            buffer[start + 2] = generalColor.b;
            buffer[start + 3] = 255;

            zBuffer[y][x] = z;
        }
    }

	
};

