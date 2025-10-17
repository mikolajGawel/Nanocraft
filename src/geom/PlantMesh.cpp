#include "PlantMesh.hpp"
#include "BlockFace.hpp"
#include <algorithm>
namespace Geom
{
    static std::array<BlockFace, 2> plantFaces = {
        BlockFace(
            std::array<Vertex, 4>{
                Vertex{{0,0,0}, {0.0f, 0.0f}},
                Vertex{{1,0,1}, {1.0f, 0.0f}},
                Vertex{{1,1,1}, {1.0f, 1.0f}},
                Vertex{{0,1,0}, {0.0f, 1.0f}}},
            {0, 1, 2,
             2, 3, 0}),
        BlockFace(
            std::array<Vertex, 4>{
                Vertex{{0,0,1}, {0.0f, 0.0f}},
                Vertex{{1,0,0}, {1.0f, 0.0f}},
                Vertex{{1,1,0}, {1.0f, 1.0f}},
                Vertex{{0,1,1}, {0.0f, 1.0f}}},
            {0, 1, 2,
             2, 3, 0}),
     
    };
    BasicMesh generatePlantMesh(glm::vec3 position,int texture){
        
       std::vector<float> vertices = {};
        std::vector<unsigned int> indices = {};


        for (size_t i = 0; i < plantFaces.size(); i++)
        {

            Geom::BlockFace face = plantFaces.at(i);
            int offset = vertices.size() / BlockFace::BlockFaceStride();

            for (uint i : face.indices) 
            {
                indices.push_back(i + offset);
            }
            auto faceVertices = face.getVerticesArray(position, texture,4);
            vertices.insert(vertices.end(), faceVertices.begin(), faceVertices.end());
        }
        std::vector<unsigned int> reversed(indices.rbegin(),indices.rend());
        indices.insert(indices.end(),reversed.begin(),reversed.end());
        return {vertices, indices};
    }
    
} // namespace Geom
