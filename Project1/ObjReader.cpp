#include "ObjReader.h"

ObjData ObjReader::ReadFile(std::string Path)
{
    std::ifstream file(Path);

    if (!file.is_open()) {
        std::cout << "Не удалось открыть файл " << Path << std::endl;
    }

    std::vector<sf::Vector3f> vertexes;
    std::vector<sf::Vector3f> normals;
    std::vector<sf::Vector3f> textures;
    std::vector<std::vector<sf::Vector3i>> polygons;
    std::map<int, sf::Vector3f> vertexNormals;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);

        std::string lineType;
        iss >> lineType;

        if (lineType == "v") {
            float x, y, z;
            iss >> x >> y >> z;
            vertexes.push_back(sf::Vector3f(x, y, z));
        }
        else if (lineType == "vt") {
            float x, y, z;
            iss >> x >> y >> z;
            textures.push_back(sf::Vector3f(x, y, z));
        }
        else if (lineType == "vn") {
            float x, y, z;
            iss >> x >> y >> z;
            normals.push_back(sf::Vector3f(x, y, z));
        }
        else if (lineType == "f") {
            std::string triplet;
            std::vector<sf::Vector3i> currentVertex;
            while (iss >> triplet) {
                std::istringstream tripletStream(triplet);

                int vertex = 0, texture = 0, normal = 0;
                tripletStream >> vertex;

                if (tripletStream.peek() == '/') {
                    tripletStream.ignore();
                    if (tripletStream.peek() != '/') {
                        tripletStream >> texture;
                    }
                    if (tripletStream.peek() == '/') {
                        tripletStream.ignore();
                        tripletStream >> normal;
                    }
                }
                currentVertex.push_back(sf::Vector3i(vertex, texture, normal));
                vertexNormals[vertex - 1] += normals[normal - 1];
                
            }
            polygons.push_back(currentVertex);
        }
    }

    file.close();
    ObjData* data = new ObjData(vertexes, normals, textures, polygons, vertexNormals);
    return *data;
}

std::vector<std::vector<sf::Vector2i>> ObjReader::GetDeviceCoordinates(ObjData& data, Matrix4x4& totalTranslationMatrix)
{
    std::vector<std::vector<sf::Vector2i>> coords;
    for (int polygonIndex = 0; polygonIndex < data.polygons.size(); polygonIndex++)
    {
        std::vector<sf::Vector2i> currentPolygon;
        for (int vertexIndex = 0; vertexIndex < data.polygons[polygonIndex].size(); vertexIndex++)
        {
            Vector4f vertex(data.vertexes[vertexIndex]);
            Vector4f translateVertex = Vector4f::Multiply(vertex, totalTranslationMatrix);
            sf::Vector2i* deviceFlatCoords = new sf::Vector2i(int(round(translateVertex.x)), int(round(translateVertex.y)));
            currentPolygon.push_back(*deviceFlatCoords);
        }
        coords.push_back(currentPolygon);
    }
    return coords;
}
