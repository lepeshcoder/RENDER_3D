#include <SFML/Graphics.hpp>
#include"Drawer.h"
#include<iostream>
#include"ObjReader.h"
#include"Matrix4x4.h"
#include"Vector4f.h"
#include "MatrixTranslations.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML works!");
    
    sf::Image image;
    image.create(1920, 1080,sf::Color::White);

 

    auto data = ObjReader::ReadFile("C:\\Users\\LepeshCoder\\Desktop\\Cube.obj"); 

    float angleX, angleY, angleZ, scale,dx,dy,dz;
    angleX = angleY = angleZ = dx = dy = dz = 0.;
    scale = 2.;
    float r = 100, a = 45, b = 0;

    Matrix4x4 ModelMatrix = MatrixTranslations::SetScale(1,1,1);

    sf::Vector3f sourceCamera = MatrixTranslations::GetCameraPositionFromSpheric(r,a,b);
    std::cout << sourceCamera.x << " : " << sourceCamera.y << " : " << sourceCamera.z << "\n";
    // ������� �������� � ����������� ������
    Matrix4x4 ViewMatrix = MatrixTranslations::CreateLookAt(sourceCamera, sf::Vector3f(0, 0, 0), sf::Vector3f(0, 1, 0));
    ViewMatrix.ViewMatrix();

    // ������� �������� � ����������� �����������
    Matrix4x4 ProjectionMatrix = MatrixTranslations::CreatePerspectiveFieldOfView(45*3.14/180.,1920/1080.,0.1,1000);

    // ������� �������� � ����������� ����������
    Matrix4x4 ViewPortMatrix = MatrixTranslations::CreateDeviceCoordinates(1920, 1080, 0, 0);

    // ������ ������� �������������
    Matrix4x4 totalMatrix = ViewPortMatrix * ProjectionMatrix * ViewMatrix * ModelMatrix;

    std::vector<Vector4f> sourceVertexes(data.vertexes.size());
    for (int i = 0; i < sourceVertexes.size(); i++)
    {
        Vector4f* v = new Vector4f(data.vertexes[i]);
        sourceVertexes[i] = *v;
    }

    std::vector<Vector4f> currVertexes(sourceVertexes);

    // �������������� ��������� � �������
    MatrixTranslations::TransformVertex(currVertexes, totalMatrix);

    Drawer::DrawModel(data.polygons, currVertexes, image);
      
    sf::Texture texture;
    texture.loadFromImage(image);
    sf::Sprite sprite(texture);

    sf::Clock clock;
    
    bool isMoving = false;
    float rotSpeed = 0.03f;
    float scaleSpeed = 0.1f;
    float movementSpeed = 0.1f;
    int frameCounter = 0;
    float time = 0;
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    srand(std::time(NULL));
    while (window.isOpen())
    {  
        sf::Time deltaTime = clock.getElapsedTime(); 
        clock.restart();

        time += deltaTime.asMicroseconds();
        if (time > 1000000)
        {
            time = 0;                                                                                                                                                                                                                   
            std::cout << "FPS: " << frameCounter << "\n";
            frameCounter = 0;
        }
        else
        {
            frameCounter++;
        }
       
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.key.code == sf::Mouse::Left)
                    isMoving = true;
            }
            if (event.type == sf::Event::MouseMoved)
            {
                if (isMoving)
                {
                    sf::Vector2i newMousePosition = sf::Mouse::getPosition(window);
                    sf::Vector2i delta = mousePosition - newMousePosition;
                    mousePosition = newMousePosition;
                    a += delta.y * deltaTime.asMilliseconds() * rotSpeed;
                    if (a > 179.9) a = 179.9;
                    if (a < 0.1) a = 0.1;
                    b += delta.x * deltaTime.asMilliseconds() * rotSpeed;
                }
                else
                {
                    mousePosition = sf::Mouse::getPosition(window);
                }
            }
            if (event.type == sf::Event::MouseButtonReleased)
            {
                isMoving = false;
            }
            if (event.type == sf::Event::MouseWheelMoved)
            {
                isMoving = true;
                r += event.mouseWheel.delta * deltaTime.asMilliseconds() * scaleSpeed;
                if (r < 1) r = 1;
            }
            if (event.type == sf::Event::KeyReleased)
            {
                isMoving = false;
            }

            
        }
        if (isMoving)
        {
            
            std::vector<Vector4f> currVertexes(sourceVertexes);
            
            sf::Vector3f camera = MatrixTranslations::GetCameraPositionFromSpheric(r, a, b);
            
            Matrix4x4 newViewMatrix = MatrixTranslations::CreateLookAt(camera, sf::Vector3f(0, 0, 0), sf::Vector3f(0,1,0));

            Matrix4x4 newTotalMatrix = ViewPortMatrix * ProjectionMatrix * newViewMatrix * ModelMatrix;
            
            MatrixTranslations::TransformVertex(currVertexes, newTotalMatrix);
            
            image.create(1920, 1080,sf::Color::White);
            
            Drawer::DrawModel(data.polygons, currVertexes, image);
            
            texture.loadFromImage(image);
            sprite.setTexture(texture);
        }
       
        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}