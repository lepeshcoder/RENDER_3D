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
            std::vector<sf::Vector3i> currentPolygon;
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
                currentPolygon.push_back(sf::Vector3i(vertex, texture, normal));
                vertexNormals[vertex - 1] += normals[normal - 1];

            }
            polygons.push_back(currentPolygon); 
            /*sf::Vector3f A(vertexes[currentPolygon[0].x - 1]);
            sf::Vector3f B(vertexes[currentPolygon[1].x - 1]);
            sf::Vector3f C(vertexes[currentPolygon[2].x - 1]);
            sf::Vector3f AB = A - B;
            sf::Vector3f AC = A - C;
            sf::Vector3f BC = B - C;
            Vector3Extensions::Normalize(AB);
            Vector3Extensions::Normalize(AC);
            Vector3Extensions::Normalize(BC);
            double An = Vector3Extensions::scalarProduct(AB, AC);
            double Bn = Vector3Extensions::scalarProduct(AB, BC);
            double Cn = Vector3Extensions::scalarProduct(BC, AC);
            vertexNormals[currentPolygon[0].x - 1] += sf::Vector3f(An * normals[currentPolygon[0].z - 1].x, An * normals[currentPolygon[0].z - 1].y, An * normals[currentPolygon[0].z - 1].z);
            vertexNormals[currentPolygon[1].x - 1] += sf::Vector3f(Bn * normals[currentPolygon[1].z - 1].x, Bn * normals[currentPolygon[1].z - 1].y, Bn * normals[currentPolygon[1].z - 1].z);
            vertexNormals[currentPolygon[2].x - 1] += sf::Vector3f(Cn * normals[currentPolygon[2].z - 1].x, Cn * normals[currentPolygon[2].z - 1].y, Cn * normals[currentPolygon[2].z - 1].z);*/
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
