#include "BlockFace.hpp"
Geom::BlockFace::BlockFace(std::array<Geom::Vertex, 4> vertices, std::array<uint, 6> indices)
    : vertices(vertices), indices(indices)
{
}
std::vector<float> Geom::BlockFace::getVerticesArray(glm::vec3 blockPosition, uint textureID,unsigned int sideID)
{
    std::vector<float> result = {};
    for (Geom::Vertex vertex : vertices)
    {
        glm::vec3 translatedPos = vertex.position + blockPosition;
        result.push_back(translatedPos.x);
        result.push_back(translatedPos.y);
        result.push_back(translatedPos.z);
        result.push_back(vertex.textureCoordinates.x);
        result.push_back(vertex.textureCoordinates.y);
        result.push_back(textureID);
        result.push_back(sideID);
    }
    return result;
}