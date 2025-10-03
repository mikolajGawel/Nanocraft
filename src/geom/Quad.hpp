#pragma once
#include "Vertex.hpp"
#include <memory>
namespace Geom
{
    class Quad
    {
        public:
        std::array<Vertex,4> vertices;
        std::array<uint,6> indices;
        Quad(std::array<Vertex,4> vertices,std::array<uint,6> indices)
            : vertices(vertices),indices(indices)
        {
            
        }
        std::vector<float> getVerticesArray(uint textureID = 0){
            std::vector<float> result = {};
            for(Vertex vertex : vertices){
                result.push_back(vertex.position.x);
                result.push_back(vertex.position.y);
                result.push_back(vertex.position.z);
                result.push_back(vertex.textureCoordinates.x);
                result.push_back(vertex.textureCoordinates.y);
                result.push_back(textureID);
            }
            return result;
        }
    };
    
} // namespace Geom

