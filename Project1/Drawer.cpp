#include "Drawer.h"

void Drawer::DrawLine(int x1, int y1, int x2, int y2, sf::Image& image)
{
    bool steep = false;
    if (std::abs(x1 - x2) < std::abs(y1 - y2)) {
        std::swap(x1, y1);
        std::swap(x2, y2);
        steep = true;
    }
    if (x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    int dx = x2 - x1;
    int dy = y2 - y1;
    int derror2 = std::abs(dy) * 2;
    int error2 = 0;
    int y = y1;
    for (int x = x1; x <= x2; x++) {
        if (steep) {
            SetPixelSecure(image,y, x, sf::Color::Red);
        }
        else {
            SetPixelSecure(image, x, y, sf::Color::Red);
        }
        error2 += derror2;

        if (error2 > dx) {
            y += (y2 > y1 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

void Drawer::DrawModel(std::vector<std::vector<sf::Vector3i>>& polygons, std::vector<Vector4f>& deviceVertexes,
    std::vector<Vector4f>& worldVertexes, std::map<int, sf::Vector3f>& vertexNormals,
    sf::Image& image, sf::Vector3f& camera, sf::Vector3f& light, Matrix4x4& inverse)
{
    const sf::Vector2u size = image.getSize();
    int drawing = 0;
    
    double** zBuffer = new double* [size.y];
    for (int i = 0; i < size.y; i++)
    {
        zBuffer[i] = new double[size.x];
        for (int j = 0; j < size.x; j++)
            zBuffer[i][j] = _I32_MAX;
    }
    
    for (int polygon = 0; polygon < polygons.size(); polygon++)
    {
        sf::Vector3i iPoints[3]  
        {
            Vector3Extensions::ToVector3i(deviceVertexes[polygons[polygon][0].x - 1]),
            Vector3Extensions::ToVector3i(deviceVertexes[polygons[polygon][1].x - 1]),
            Vector3Extensions::ToVector3i(deviceVertexes[polygons[polygon][2].x - 1])
        };

        sf::Vector3f fPoints[3]
        {
            sf::Vector3f(deviceVertexes[polygons[polygon][0].x - 1].x,deviceVertexes[polygons[polygon][0].x - 1].y,deviceVertexes[polygons[polygon][0].x - 1].z),
            sf::Vector3f(deviceVertexes[polygons[polygon][1].x - 1].x,deviceVertexes[polygons[polygon][1].x - 1].y,deviceVertexes[polygons[polygon][1].x - 1].z),
            sf::Vector3f(deviceVertexes[polygons[polygon][2].x - 1].x,deviceVertexes[polygons[polygon][2].x - 1].y,deviceVertexes[polygons[polygon][2].x - 1].z)
        };

        sf::Vector3f worldPoints[3]
        {
           sf::Vector3f(worldVertexes[polygons[polygon][0].x - 1].x, worldVertexes[polygons[polygon][0].x - 1].y, worldVertexes[polygons[polygon][0].x - 1].z),
           sf::Vector3f(worldVertexes[polygons[polygon][1].x - 1].x, worldVertexes[polygons[polygon][1].x - 1].y, worldVertexes[polygons[polygon][1].x - 1].z),
           sf::Vector3f(worldVertexes[polygons[polygon][2].x - 1].x, worldVertexes[polygons[polygon][2].x - 1].y, worldVertexes[polygons[polygon][2].x - 1].z),
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
 
        drawing++;

        int total_height = iPoints[2].y - iPoints[0].y;
        if (total_height != 0)
        {    
            int segmentUp = iPoints[1].y - iPoints[0].y + 1;
          
            for (int y = iPoints[0].y; y <= iPoints[1].y; y++) {
                float alpha = (float)(y - iPoints[0].y) / total_height;
                float beta = (float)(y - iPoints[0].y) / segmentUp;
                int x1 = iPoints[0].x + alpha * (iPoints[2].x - iPoints[0].x);
                int x2 = iPoints[0].x + beta * (iPoints[1].x - iPoints[0].x);
                if (x1 > x2) std::swap(x1, x2);
                if (x1 < 0) x1 = 0;    
                if (x2 > (int)size.x) x2 = size.x - 1;
                for (int x = x1; x <= x2; x++)
                {
                    if (y >= 0 && y < size.y)
                    {
                        double z = Vector3Extensions::FindZ(x, y, fPoints);
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

                            sf::Vector3f normalA = vertexNormals[polygons[polygon][0].x - 1];
                            Vector3Extensions::Normalize(normalA);
                            
                            sf::Vector3f normalB = vertexNormals[polygons[polygon][1].x - 1];
                            Vector3Extensions::Normalize(normalB);

                            sf::Vector3f normalC = vertexNormals[polygons[polygon][2].x - 1];
                            Vector3Extensions::Normalize(normalC);

                            sf::Vector3f pointNormal = MatrixTranslations::GetPointNormal(barCoords, normalA, normalB, normalC);
                            Vector3Extensions::Normalize(pointNormal);
                            
                            sf::Vector3f pointSight = light - pointOF;
                            Vector3Extensions::Normalize(pointSight);

                            double pointLightIntensity = std::max(0.0, Vector3Extensions::scalarProduct(pointNormal, pointSight));
                            sf::Color color(pointLightIntensity * 255, pointLightIntensity * 255, pointLightIntensity * 255);
                            
                            image.setPixel(x, y, color);
                            
                            zBuffer[y][x] = z;
                        }
                    
                    }
                }
            }
            int segmentDown = iPoints[2].y - iPoints[1].y + 1;
            
            for (int y = iPoints[1].y; y <= iPoints[2].y; y++) {
                float alpha = (float)(y - iPoints[0].y) / total_height;
                float beta = (float)(y - iPoints[1].y) / segmentDown;
                int x1 = iPoints[0].x + alpha * (iPoints[2].x - iPoints[0].x);
                int x2 = iPoints[1].x + beta * (iPoints[2].x - iPoints[1].x);
                if (x1 > x2) std::swap(x1, x2);
                if (x1 < 0) x1 = 0;
                if (x2 > (int)size.x) x2 = size.x - 1;
                for (int x = x1; x <= x2; x++)
                {
                    if (y >= 0 && y < size.y)
                    {
                        double z = Vector3Extensions::FindZ(x, y, fPoints);
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

                            sf::Vector3f normalA = vertexNormals[polygons[polygon][0].x - 1];
                            Vector3Extensions::Normalize(normalA);

                            sf::Vector3f normalB = vertexNormals[polygons[polygon][1].x - 1];
                            Vector3Extensions::Normalize(normalB);

                            sf::Vector3f normalC = vertexNormals[polygons[polygon][2].x - 1];
                            Vector3Extensions::Normalize(normalC);

                            sf::Vector3f pointNormal = MatrixTranslations::GetPointNormal(barCoords, normalA, normalB, normalC);
                            Vector3Extensions::Normalize(pointNormal);

                            sf::Vector3f pointSight = light - pointOF;
                            Vector3Extensions::Normalize(pointSight);

                            double pointLightIntensity = std::max(0.0, Vector3Extensions::scalarProduct(pointNormal, pointSight));
                            sf::Color color(pointLightIntensity * 255, pointLightIntensity * 255, pointLightIntensity * 255);

                            image.setPixel(x, y, color);

                            zBuffer[y][x] = z;
                        }
                    }
                }
            }
        }
        
    }
    std::cout << "drawing: " << drawing << "\n";
    for (int i = 0; i < size.y; i++)
    {
        delete[] zBuffer[i];
    }
    delete[] zBuffer;
}

void Drawer::SetPixelSecure(sf::Image& image, int x, int y, sf::Color color)
{
    sf::Vector2u size = image.getSize();
    if (x < size.x && x >= 0 && y < size.y && y >= 0)
    {
        image.setPixel(x, y, color);
    }
}



void Drawer::swap(int& a, int& b)
{
    a ^= b;
    b ^= a;
    a ^= b;
}

