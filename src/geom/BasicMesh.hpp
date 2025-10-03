#pragma once
#include <vector>
namespace Geom
{
    struct BasicMesh
    {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
    };    
} // namespace Geom
