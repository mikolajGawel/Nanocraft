#include "BlockMesh.hpp"
#include "BasicMesh.hpp"
#include "BlockFace.hpp"
#include <memory>

std::array<bool, 6> faceToArray(Geom::SelectedFaces faces)
{
    return {faces.north, faces.south, faces.east, faces.west, faces.top, faces.bottom};
}
std::array<int, 6> textureMapToArray(Geom::BlockTextureMap textures)
{
    return {textures.north, textures.south, textures.east, textures.west, textures.top, textures.bottom};
}
namespace Geom
{
    static std::array<BlockFace, 6> blockFaces = {
        // North face (x+)
        BlockFace(
            std::array<Vertex, 4>{
                Vertex{{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
                Vertex{{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
                Vertex{{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},
                Vertex{{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}}},
            {0, 1, 2,
             2, 3, 0}),
        // South face (x-)
        BlockFace(
            std::array<Vertex, 4>{
                Vertex{{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},
                Vertex{{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},
                Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
                Vertex{{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}}},
            {0, 1, 2,
             2, 3, 0}),
        // East face (z+)
        BlockFace(
            std::array<Vertex, 4>{
                Vertex{{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},
                Vertex{{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},
                Vertex{{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},
                Vertex{{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}}},
            {0, 1, 2,
             2, 3, 0}),
        // West face (z-)
        BlockFace(
            std::array<Vertex, 4>{
                Vertex{{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
                Vertex{{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
                Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
                Vertex{{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}}},
            {0, 1, 2,
             2, 3, 0}),
        // Top face (y+)
        BlockFace(
            std::array<Vertex, 4>{
                Vertex{{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},
                Vertex{{-0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},
                Vertex{{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f}},
                Vertex{{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}}},
            {0, 1, 2,
             2, 3, 0}),
        // Bottom face (y-)
        BlockFace(
            std::array<Vertex, 4>{
                Vertex{{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},
                Vertex{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
                Vertex{{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},
                Vertex{{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f}}},
            {0, 1, 2,
             2, 3, 0})};

    BasicMesh generateBlockMesh(glm::vec3 position, SelectedFaces faces, BlockTextureMap textures)
    {
        std::vector<float> vertices = {};
        std::vector<unsigned int> indices = {};
        std::array<bool, 6> faceArray = faceToArray(faces);
        std::array<int, 6> textureArray = textureMapToArray(textures);

        for (size_t i = 0; i < faceArray.size(); i++)
        {
            if (!faceArray[i])
                continue;

            Geom::BlockFace face = blockFaces.at(i);
            int offset = vertices.size() / 6;

            for (uint i : face.indices)
            {
                indices.push_back(i + offset);
            }
            auto faceVertices = face.getVerticesArray(position, textureArray.at(i));
            vertices.insert(vertices.end(), faceVertices.begin(), faceVertices.end());
        }
        return {vertices, indices};
    }
}