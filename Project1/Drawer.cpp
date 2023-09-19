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

void Drawer::DrawModel(std::vector<std::vector<sf::Vector3i>>& polygons, std::vector<Vector4f>& vertexes, sf::Image& image)
{
    const sf::Vector2u size = image.getSize();
    
    
    int** zBuffer = new int* [size.y];
    for (int i = 0; i < size.y; i++)
    {
        zBuffer[i] = new int[size.x] {_I32_MAX};
    }

    for (int polygon = 0; polygon < polygons.size(); polygon++)
    {
        Vector4f points[3] = {
            vertexes[polygons[polygon][0].x - 1],
            vertexes[polygons[polygon][1].x - 1],
            vertexes[polygons[polygon][2].x - 1]
        };
        if (points[0].y > points[1].y) std::swap(points[0], points[1]);
        if (points[0].y > points[2].y) std::swap(points[0], points[2]);
        if (points[1].y > points[2].y) std::swap(points[1], points[2]);
        
        
        int total_height = points[2].y - points[0].y + 1;
        if (total_height != 0)
        {
            sf::Color color(rand() % 256, rand() % 256, rand() % 256);
            int segment_height1 = points[1].y - points[0].y + 1;
            if (segment_height1 != 0)
            {
                for (int y = points[0].y; y <= points[1].y; y++) {
                    float alpha = (y - points[0].y) / (float)total_height;
                    float beta = (y - points[0].y) / (float)segment_height1;
                    int x1 = round(points[0].x + alpha * (points[2].y - points[0].y));
                    int x2 = round(points[0].x + beta * (points[1].y - points[0].y));
                    if (x1 > x2) std::swap(x1, x2);
                    for (int x = x1; x <= x2; x++) {
                        SetPixelSecure(image, x, y, color);
                    }
                }
            }
            /*int segment_height2 = points[2].y - points[1].y + 1;
            if (segment_height2 != 0)
            {
                sf::Color color(rand() % 256, rand() % 256, rand() % 256);
                for (int y = points[1].y; y <= points[2].y; y++) {
                    float alpha =(y - points[0].y) / (float)total_height;
                    float beta = (y - points[1].y) / (float)segment_height2;
                    int x1 = round(points[0].x + alpha * (points[2].y - points[0].y));
                    int x2 = round(points[1].x + beta * (points[2].y - points[1].y));
                    if (x1 > x2) std::swap(x1, x2);
                    for (int x = x1; x <= x2; x++) {
                        SetPixelSecure(image, x, y, color);
                    }
                }
            }*/
        }



        /*for (int vertex = 1; vertex < polygons[polygon].size(); vertex++)
        {

            int indexA = polygons[polygon][vertex - 1].x - 1;
            int indexB = polygons[polygon][vertex].x - 1;
            Vector4f vertexA = vertexes[indexA];
            Vector4f vertexB = vertexes[indexB];
            if(vertexA.x>=0 && vertexA.x<size.x && vertexA.y>=0 && vertexA.y<size.y &&
                vertexB.x>=0 && vertexB.x<size.x && vertexB.y>=0 && vertexB.y<size.y)
            DrawLine(vertexA.x, vertexA.y, vertexB.x, vertexB.y, image);
        }*/
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

