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

    static void FillPixel(int x1, int x2, int x, int y, sf::Vector3f* fPoints, double** zBuffer, Matrix4x4& inverse, std::vector<sf::Vector3f>& normals,
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
        
            sf::Vector3f barCoords = MatrixTranslations::GetBarCoords(fPoints[0], fPoints[1], fPoints[2], pointO);

            sf::Vector3f normalA = normals[polygons[polygon][0].z - 1];
            Vector3Extensions::Normalize(normalA);

            sf::Vector3f normalB = normals[polygons[polygon][1].z - 1];
            Vector3Extensions::Normalize(normalB);

            sf::Vector3f normalC = normals[polygons[polygon][2].z - 1];
            Vector3Extensions::Normalize(normalC);

            sf::Vector3f pointNormal = MatrixTranslations::GetPointNormal(barCoords, normalA, normalB, normalC);

            sf::Vector3f pointSight = light - pointOF;
            Vector3Extensions::Normalize(pointSight);

            double pointLightIntensity = std::max(0.0, Vector3Extensions::scalarProduct(pointNormal, pointSight));


            double ambientIntensivity = 0;
            sf::Vector3f ambientColor(71 / 255., 71 / 255., 71 / 255.);
            sf::Vector3f ambientComponent(ambientColor.x * ambientIntensivity, ambientColor.y * ambientIntensivity, ambientColor.z * ambientIntensivity);
            
            double diffuseIntensivity = 0;
            sf::Vector3f diffuseColor(179 / 255., 179 / 255., 179 / 255.);
            double diffuseAngle = pointLightIntensity;
            double diffuseCoeff = diffuseIntensivity * diffuseAngle;
            sf::Vector3f diffuseComponent(diffuseColor.x * diffuseCoeff, diffuseColor.y * diffuseCoeff, diffuseColor.z * diffuseCoeff);

            double specularIntensivity = 300;
            double specularCoeff = 1;
            sf::Vector3f specularColor(255 / 255., 255 / 255., 255 / 255.);
            sf::Vector3f V = camera - pointOF;
            Vector3Extensions::Normalize(V);
            sf::Vector3f L = light - pointOF;
            Vector3Extensions::Normalize(L);
            double temp = 2 * Vector3Extensions::scalarProduct(L, pointNormal);
            sf::Vector3f R = L - sf::Vector3f(pointNormal.x * temp, pointNormal.y * temp, pointNormal.z * temp);
            Vector3Extensions::Normalize(R);
            double temp1 = pow(Vector3Extensions::scalarProduct(R, V), specularCoeff) * specularIntensivity;
            sf::Vector3f specularComponent(specularColor.x * temp1, specularColor.y * temp1, specularColor.z * temp1);

                       
            sf::Vector3f generalColor = ambientComponent + diffuseComponent + specularComponent;
            if (generalColor.x < 0) generalColor.x = 0;
            if (generalColor.y < 0) generalColor.y = 0;
            if (generalColor.z < 0) generalColor.z = 0;
            if (generalColor.x > 255) generalColor.x = 255;
            if (generalColor.y > 255) generalColor.y = 255;
            if (generalColor.z > 255) generalColor.z = 255;


            int start = (y * size.x + x) * 4;
            buffer[start] = generalColor.x;
            buffer[start + 1] = generalColor.y;
            buffer[start + 2] = generalColor.z;
            buffer[start + 3] = 255;

            zBuffer[y][x] = z;
        }
    }

	
};

