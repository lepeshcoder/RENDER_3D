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

    auto data = ObjReader::ReadFile("C:\\Users\\LepeshCoder\\Desktop\\newSkull.obj"); 

    float angleX, angleY, angleZ, scale,dx,dy,dz;
    angleX = angleY = angleZ = dx = dy = dz = 0.;
    scale = 2.;
    float r = 100, a = 0, b = 0;

    Matrix4x4 ModelMatrix = MatrixTranslations::SetRotationX(angleX * 3.14 / 180) * MatrixTranslations::SetRotationY(angleY * 3.14/180) *
        MatrixTranslations::SetRotationZ(angleZ *3.14/180) * MatrixTranslations::SetScale(scale, scale, scale);

    sf::Vector3f sourceCamera(-100, 0, 0);
    // ћатрица перехода к координатам камеры
    Matrix4x4 ViewMatrix = MatrixTranslations::CreateLookAt(sourceCamera, sf::Vector3f(0, 0, 0), sf::Vector3f(0, 1, 0));

    // ћатрица перехода к координатам перспективы
    Matrix4x4 ProjectionMatrix = MatrixTranslations::CreatePerspectiveFieldOfView(45*3.14/180.,1920/1080.,0.1,1000);

    // ћатрица перехода к координатам устройства
    Matrix4x4 ViewPortMatrix = MatrixTranslations::CreateDeviceCoordinates(1920, 1080, 0, 0);

    // ѕолна€ матрица трансформации
    Matrix4x4 totalMatrix = ViewPortMatrix * ProjectionMatrix * ViewMatrix * ModelMatrix;

    std::vector<Vector4f> sourceVertexes(data.vertexes.size());
    for (int i = 0; i < sourceVertexes.size(); i++)
    {
        Vector4f* v = new Vector4f(data.vertexes[i]);
        sourceVertexes[i] = *v;
    }

    std::vector<Vector4f> currVertexes(sourceVertexes);

    // преобразование координат и деление
    MatrixTranslations::TransformVertex(currVertexes, totalMatrix);

    Drawer::DrawModel(data.polygons, currVertexes, image);
      
    sf::Texture texture;
    texture.loadFromImage(image);
    sf::Sprite sprite(texture);

    sf::Clock clock;
    
    bool isMoving = false;
    float rotSpeed = 0.5f;
    float scaleSpeed = 0.01f;
    float movementSpeed = 0.1f;
    int frameCounter = 0;
    float time = 0;
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
            if (event.type == sf::Event::KeyPressed)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                {
                    isMoving = true;
                    angleX += deltaTime.asMilliseconds() * rotSpeed * 3.14 / 180;
                    /*isMoving = true;
                    a += deltaTime.asMilliseconds() * rotSpeed;*/
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                {
                    isMoving = true;
                    angleX -= deltaTime.asMilliseconds() * rotSpeed * 3.14 / 180;       
                    /*isMoving = true;
                    a -= deltaTime.asMilliseconds() * rotSpeed;*/
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
                {
                    isMoving = true;
                    angleY += deltaTime.asMilliseconds() * rotSpeed * 3.14 / 180;
                    /*isMoving = true;
                    b += deltaTime.asMilliseconds() * rotSpeed;*/
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
                {
                    isMoving = true;
                    angleY -= deltaTime.asMilliseconds() * rotSpeed * 3.14 / 180;
                    /*isMoving = true;
                    b -= deltaTime.asMilliseconds() * rotSpeed;*/
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
                {
                    isMoving = true;
                    angleZ += deltaTime.asMilliseconds() * rotSpeed * 3.14 / 180;
                   /* isMoving = true;
                    r += deltaTime.asMilliseconds() * movementSpeed;*/
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
                {
                    isMoving = true;
                    angleZ -= deltaTime.asMilliseconds() * rotSpeed * 3.14 / 180;
                    /*isMoving = true;
                    r -= deltaTime.asMilliseconds() * movementSpeed;*/
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                {
                    isMoving = true;
                    scale += deltaTime.asMilliseconds() * scaleSpeed;
                    if (scale > 5000) scale = 5000;                   
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                {
                    isMoving = true;
                    scale -= deltaTime.asMilliseconds() * scaleSpeed;
                    if (scale < 0.001) scale = 0.001;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                {
                    isMoving = true;
                    dx += deltaTime.asMilliseconds() * movementSpeed;
                   
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                {
                    isMoving = true;
                    dx -= deltaTime.asMilliseconds() * movementSpeed;
                    
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                {
                    isMoving = true;
                    dy += deltaTime.asMilliseconds() * movementSpeed;

                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                {
                    isMoving = true;
                    dy -= deltaTime.asMilliseconds() * movementSpeed;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
                {
                    isMoving = true;
                    dz += deltaTime.asMilliseconds() * movementSpeed;

                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
                {
                    isMoving = true;
                    dz -= deltaTime.asMilliseconds() * movementSpeed;

                }
            }
            if (event.type == sf::Event::KeyReleased)
            {
                isMoving = false;
            }

            
        }
        if (isMoving)
        {
            std::vector<Vector4f> currVertexes(sourceVertexes);
            
            Matrix4x4 newModelMatrix = MatrixTranslations::SetRotationX(angleX) * MatrixTranslations::SetRotationY(angleY)
                * MatrixTranslations::SetRotationZ(angleZ) * MatrixTranslations::SetScale(scale, scale, scale);

            sf::Vector3f camera = sourceCamera + sf::Vector3f(dx,dy,dz);

            Matrix4x4 newViewMatrix = MatrixTranslations::CreateLookAt(camera, sf::Vector3f(0, 0, 0), sf::Vector3f(0, 1, 0));

            Matrix4x4 newTotalMatrix = ViewPortMatrix * ProjectionMatrix * newViewMatrix * newModelMatrix;
            
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