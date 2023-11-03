#include "Drawer.h"
#include <thread>



void Drawer::DrawModel(std::vector<std::vector<sf::Vector3i>>& polygons, std::vector<Vector4f>& deviceVertexes,
    std::vector<Vector4f>& worldVertexes, std::unordered_map<int, sf::Vector3f>& vertexNormals,
    sf::Texture& texture, sf::Vector3f& camera, sf::Vector3f& light, Matrix4x4& inverse, std::vector<sf::Vector3f>& normals,ThreadPool& pool)
{
    const sf::Vector2u size(1920, 1080);
    int drawing = 0;

    double ambientIntensivity = 0.2;
    sf::Color ambientColor = sf::Color::Yellow;
    sf::Color newColor = sf::Color::Blue;
    sf::Color sum = ambientColor + newColor;
    
    double** zBuffer = new double* [size.y];
    for (int i = 0; i < size.y; i++)
    {
        zBuffer[i] = new double[size.x];
        for (int j = 0; j < size.x; j++)
            zBuffer[i][j] = _I32_MAX;
    }
    

    unsigned int bufferSize = size.x * size.y * 4;
    sf::Uint8* buffer = new sf::Uint8[bufferSize] {0};

    for (int polygon = 0; polygon < polygons.size(); polygon++)
    {
        int vertexAIndex = polygons[polygon][0].x - 1;
        int vertexBIndex = polygons[polygon][1].x - 1;
        int vertexCIndex = polygons[polygon][2].x - 1;

        sf::Vector3i iPoints[3]  
        {
            Vector3Extensions::ToVector3i(deviceVertexes[vertexAIndex]),
            Vector3Extensions::ToVector3i(deviceVertexes[vertexBIndex]),
            Vector3Extensions::ToVector3i(deviceVertexes[vertexCIndex])
        };

        sf::Vector3f fPoints[3]
        {
            sf::Vector3f(deviceVertexes[vertexAIndex].x,deviceVertexes[vertexAIndex].y,deviceVertexes[vertexAIndex].z),
            sf::Vector3f(deviceVertexes[vertexBIndex].x,deviceVertexes[vertexBIndex].y,deviceVertexes[vertexBIndex].z),
            sf::Vector3f(deviceVertexes[vertexCIndex].x,deviceVertexes[vertexCIndex].y,deviceVertexes[vertexCIndex].z)
        };

        sf::Vector3f worldPoints[3]
        {
           sf::Vector3f(worldVertexes[vertexAIndex].x, worldVertexes[vertexAIndex].y, worldVertexes[vertexAIndex].z),
           sf::Vector3f(worldVertexes[vertexBIndex].x, worldVertexes[vertexBIndex].y, worldVertexes[vertexBIndex].z),
           sf::Vector3f(worldVertexes[vertexCIndex].x, worldVertexes[vertexCIndex].y, worldVertexes[vertexCIndex].z),
        };

      
        if (iPoints[0].y > iPoints[1].y) std::swap(iPoints[0], iPoints[1]);
        if (iPoints[0].y > iPoints[2].y) std::swap(iPoints[0], iPoints[2]);
        if (iPoints[1].y > iPoints[2].y) std::swap(iPoints[1], iPoints[2]);

        sf::Vector3f edgeA = worldPoints[1] - worldPoints[0];
        sf::Vector3f edgeB = worldPoints[2] - worldPoints[0];

        sf::Vector3f normal = Vector3Extensions::crossProduct(edgeA, edgeB);

        Vector3Extensions::Normalize(normal);

        sf::Vector3f polygonCenter = sf::Vector3f
        (
            (worldPoints[0].x + worldPoints[1].x + worldPoints[2].x) / 3.0,
            (worldPoints[0].y + worldPoints[1].y + worldPoints[2].y) / 3.0,
            (worldPoints[0].z + worldPoints[1].z + worldPoints[2].z) / 3.0          
        );

        sf::Vector3f sight = camera - polygonCenter;
        Vector3Extensions::Normalize(sight);
    
        double visibility = Vector3Extensions::scalarProduct(normal, sight);

        if (visibility < 0) continue;
 
       

        int total_height = iPoints[2].y - iPoints[0].y;
        if (total_height != 0)
        {    
            int segmentUp = iPoints[1].y - iPoints[0].y + 1;

            int from = (iPoints[0].y < 0) ? 0 : iPoints[0].y;
            int to = (iPoints[1].y >= (int)size.y) ? size.y - 1 : iPoints[1].y;
            
            for (int y = from; y <= to; y++) {
                float alpha = (float)(y - iPoints[0].y) / total_height;
                float beta = (float)(y - iPoints[0].y) / segmentUp;
                int x1 =iPoints[0].x + alpha * (iPoints[2].x - iPoints[0].x);
                int x2 =iPoints[0].x + beta * (iPoints[1].x - iPoints[0].x);
                if (x1 > x2) std::swap(x1, x2);
                if (x1 < 0) x1 = 0;    
                if (x2 > (int)size.x) x2 = size.x - 1;
                for (int x = x1; x <= x2; x++)
                {
                    float phi = x1 == x2 ? 1. : (float)(x - x1) / (float)(x2 - x1);
                    double z = fPoints[0].z + phi * (fPoints[1].z - fPoints[0].z);
                    FillPixel(x1, x2, x, y, fPoints, zBuffer, inverse, normals, polygons, polygon, light,phi,z,buffer,size,camera);
                }
            }

            

            int segmentDown = iPoints[2].y - iPoints[1].y + 1;
            
            int from1 = (iPoints[1].y < 0) ? 0 : iPoints[1].y;
            int to1 = (iPoints[2].y >= (int)size.y) ? size.y - 1 : iPoints[2].y;

            for (int y = from1; y <= to1; y++) {
                float alpha = (float)(y - iPoints[0].y) / total_height;
                float beta = (float)(y - iPoints[1].y) / segmentDown;
                int x1 = iPoints[0].x + alpha * (iPoints[2].x - iPoints[0].x);
                int x2 = iPoints[1].x + beta * (iPoints[2].x - iPoints[1].x);
                if (x1 > x2) std::swap(x1, x2);
                if (x1 < 0) x1 = 0;
                if (x2 > (int)size.x) x2 = size.x - 1;
                for (int x = x1; x <= x2; x++)
                {
                    float phi = x1 == x2 ? 1. : (float)(x - x1) / (float)(x2 - x1);
                    double z = fPoints[1].z + phi * (fPoints[2].z - fPoints[1].z);
                    FillPixel(x1, x2, x, y, fPoints, zBuffer, inverse, normals, polygons, polygon, light, phi, z,buffer,size, camera);
                }
            }
        }
       
       
        
    }
    texture.update(buffer);
    delete[] buffer;

    for (int i = 0; i < size.y; i++)
    {
        delete[] zBuffer[i];
    }
    delete[] zBuffer;  
}

void Drawer::swap(int& a, int& b)
{
    a ^= b;
    b ^= a;
    a ^= b;
}

