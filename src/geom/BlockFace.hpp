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
        //side id in order north,south,east,west,top,bottom
        std::vector<float> getVerticesArray(glm::vec3 blockPosition,uint textureID = 0,unsigned int sideID = 0);
        static unsigned int BlockFaceStride(){return 7;}
    };
    
} // namespace Geom

