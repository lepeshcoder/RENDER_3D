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
            image.setPixel(y, x, sf::Color::Red);
        }
        else {
            image.setPixel(x, y, sf::Color::Red);
        }
        error2 += derror2;

        if (error2 > dx) {
            y += (y2 > y1 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

void Drawer::DrawModel(std::vector<std::vector<sf::Vector3i>>& polygons, std::vector<Vector4f>& vertexes, sf::Image& image)
{
    sf::Vector2u size = image.getSize();
    for (int polygon = 0; polygon < polygons.size(); polygon++)
    {
        for (int vertex = 1; vertex < polygons[polygon].size(); vertex++)
        {
            int indexA = polygons[polygon][vertex - 1].x - 1;
            int indexB = polygons[polygon][vertex].x - 1;
            Vector4f vertexA = vertexes[indexA];
            Vector4f vertexB = vertexes[indexB];
            if(vertexA.x>=0 && vertexA.x<size.x && vertexA.y>=0 && vertexA.y<size.y &&
                vertexB.x>=0 && vertexB.x<size.x && vertexB.y>=0 && vertexB.y<size.y)
            DrawLine(vertexA.x, vertexA.y, vertexB.x, vertexB.y, image);
        }
    }
}



void Drawer::swap(int& a, int& b)
{
    a ^= b;
    b ^= a;
    a ^= b;
}
