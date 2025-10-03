#pragma once
#include "geom/Vertex.hpp"
#include <memory>
namespace Geom
{
    class BlockFace
    {
        public:
        std::array<Vertex,4> vertices;
        std::array<uint,6> indices;
        BlockFace(std::array<Vertex,4> vertices,std::array<uint,6> indices);
        std::vector<float> getVerticesArray(glm::vec3 blockPosition,uint textureID = 0);
    };
    
} // namespace Geom

