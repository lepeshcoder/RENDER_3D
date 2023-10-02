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
    std::vector<Vector4f>& worldVertexes, std::map<int, sf::Vector3f> vertexNormals,
    sf::Image& image, sf::Vector3f& camera, sf::Vector3f& light)
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
        sf::Vector3i points[3]  
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

        sf::Vector3f normals[3]
        {
            vertexNormals[polygons[polygon][0].x],
            vertexNormals[polygons[polygon][1].x],
            vertexNormals[polygons[polygon][2].x]
        };

        if (points[0].y > points[1].y) std::swap(points[0], points[1]);
        if (points[0].y > points[2].y) std::swap(points[0], points[2]);
        if (points[1].y > points[2].y) std::swap(points[1], points[2]);

        sf::Vector3f edgeA = worldPoints[1] - worldPoints[0];
        sf::Vector3f edgeB = worldPoints[2] - worldPoints[0];

        
        

        sf::Vector3f normal = Vector3Extensions::crossProduct(edgeA, edgeB);

        double area = Vector3Extensions::len(normal) * 0.5;

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
        
        int total_height = points[2].y - points[0].y;
        if (total_height != 0)
        {    
            int segmentUp = points[1].y - points[0].y + 1;
          
            for (int y = points[0].y; y <= points[1].y; y++) {
                float alpha = (float)(y - points[0].y) / total_height;
                float beta = (float)(y - points[0].y) / segmentUp;
                int x1 = points[0].x + alpha * (points[2].x - points[0].x);
                int x2 = points[0].x + beta * (points[1].x - points[0].x);
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
                            sf::Vector3f barCoords = MatrixTranslations::GetBarCoords(fPoints[0], fPoints[1], fPoints[2], pointO);
                            sf::Vector3f normalA = normals[polygons[polygon][0].x - 1];
                            sf::Vector3f normalB = normals[polygons[polygon][1].x - 1];
                            sf::Vector3f normalC = normals[polygons[polygon][2].x - 1];

                            sf::Vector3f pointNormal = MatrixTranslations::GetPointNormal(barCoords, normalA, normalB, normalC);
                            Vector3Extensions::Normalize(pointNormal);
                        
                            sf::Vector3f pointSight = light - sf::Vector3f(x, y, z);
                            Vector3Extensions::Normalize(pointSight);

                            double pointLightIntensity = std::max(0.0, Vector3Extensions::scalarProduct(pointNormal, pointSight));
                            sf::Color color(pointLightIntensity * 255, pointLightIntensity * 255, pointLightIntensity * 255);
                            
                            image.setPixel(x, y, color);
                            
                            zBuffer[y][x] = z;
                        }
                    
                    }
                }
            }
            int segmentDown = points[2].y - points[1].y + 1;
            
            for (int y = points[1].y; y <= points[2].y; y++) {
                float alpha = (float)(y - points[0].y) / total_height;
                float beta = (float)(y - points[1].y) / segmentDown;
                int x1 = points[0].x + alpha * (points[2].x - points[0].x);
                int x2 = points[1].x + beta * (points[2].x - points[1].x);
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
                            sf::Vector3f barCoords = MatrixTranslations::GetBarCoords(fPoints[0], fPoints[1], fPoints[2], pointO);
                            sf::Vector3f normalA = normals[polygons[polygon][0].x - 1];
                            sf::Vector3f normalB = normals[polygons[polygon][1].x - 1];
                            sf::Vector3f normalC = normals[polygons[polygon][2].x - 1];

                            sf::Vector3f pointNormal = MatrixTranslations::GetPointNormal(barCoords, normalA, normalB, normalC);
                            Vector3Extensions::Normalize(pointNormal);

                            sf::Vector3f pointSight = light - sf::Vector3f(x, y, z);
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

