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

void Drawer::DrawModel(std::vector<std::vector<sf::Vector3i>>& polygons, std::vector<Vector4f>& vertexes, std::vector<sf::Vector3f>& normals, sf::Image& image, sf::Vector3f& camera)
{
    const sf::Vector2u size = image.getSize();
    
    double** zBuffer = new double* [size.y];
    for (int i = 0; i < size.y; i++)
    {
        zBuffer[i] = new double[size.x];
        for (int j = 0; j < size.x; j++)
        {
            zBuffer[i][j] = _I32_MAX;
        }
    }

    for (int polygon = 0; polygon < polygons.size(); polygon++)
    {
        sf::Vector3i points[3] 
        {
            Vector3Extensions::ToVector3i(vertexes[polygons[polygon][0].x - 1]),
            Vector3Extensions::ToVector3i(vertexes[polygons[polygon][1].x - 1]),
            Vector3Extensions::ToVector3i(vertexes[polygons[polygon][2].x - 1])
        };

        sf::Vector3f fPoints[3]
        {
            sf::Vector3f(vertexes[polygons[polygon][0].x - 1].x,vertexes[polygons[polygon][0].x - 1].y,vertexes[polygons[polygon][0].x - 1].z),
            sf::Vector3f(vertexes[polygons[polygon][1].x - 1].x,vertexes[polygons[polygon][1].x - 1].y,vertexes[polygons[polygon][1].x - 1].z),
            sf::Vector3f(vertexes[polygons[polygon][2].x - 1].x,vertexes[polygons[polygon][2].x - 1].y,vertexes[polygons[polygon][2].x - 1].z),
        };

        if (points[0].y > points[1].y) std::swap(points[0], points[1]);
        if (points[0].y > points[2].y) std::swap(points[0], points[2]);
        if (points[1].y > points[2].y) std::swap(points[1], points[2]);

        sf::Vector3f edgeA = fPoints[1] - fPoints[0];
        sf::Vector3f edgeB = fPoints[1] - fPoints[2];

        sf::Vector3f normal = Vector3Extensions::crossProduct(edgeA,edgeB);
        Vector3Extensions::Normalize(normal);
        sf::Vector3f polygonCenter = sf::Vector3f(
            (fPoints[0].x + fPoints[1].x + fPoints[2].x) / 3.0, 
            (fPoints[0].y + fPoints[1].y + fPoints[2].y) / 3.0, 
            (fPoints[0].z + fPoints[1].z + fPoints[2].z) / 3.0);
        sf::Vector3f sight = camera - polygonCenter ;
        if (Vector3Extensions::scalarProduct(normal, sight) < 0) continue;
        
        
        int total_height = points[2].y - points[0].y;
        if (total_height != 0)
        {
            sf::Color color(rand() % 256, rand() % 256, rand() % 256);
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
                    if (y >= 0 && y < size.y )
                    {
                        double z = Vector3Extensions::FindZ(x, y, fPoints);
                        if (z < zBuffer[y][x])
                        {
                            zBuffer[y][x] = z;
                            image.setPixel(x, y, color);
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
                    if (x1 == 0 && x2 == size.x - 1)
                    {
                        int kal = 4;
                    }
                    if (y >= 0 && y < size.y)
                    {
                        double z = Vector3Extensions::FindZ(x, y, fPoints);
                        if (z < zBuffer[y][x])
                        {
                            zBuffer[y][x] = z;
                            image.setPixel(x, y, color);
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

