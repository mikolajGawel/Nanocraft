#pragma once
#include "geom/Vertex.hpp"
#include <memory>
namespace Blocks
{
    class BlockFace
    {
        public:
        std::array<Geom::Vertex,4> vertices;
        std::array<uint,6> indices;
        BlockFace(std::array<Geom::Vertex,4> vertices,std::array<uint,6> indices);
        std::vector<float> getVerticesArray(glm::vec3 blockPosition,uint textureID = 0);
    };
    
} // namespace Geom

