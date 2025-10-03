#pragma once
#include "Vertex.hpp"
#include <memory>
namespace Geom
{
    class Quad
    {
        static unsigned int vao,vbo,ibo;
    public:
        static void init();
        static void bind();
        static void draw();
    };
    
} // namespace Geom

