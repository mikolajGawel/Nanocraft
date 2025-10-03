#pragma once
#include "BasicMesh.hpp"
namespace Geom
{
    class ChunkMesh
    {
    private:
        unsigned int vao, vbo, ibo;
        BasicMesh mesh;
        bool created = false;
    public:
        ChunkMesh();
        void genBuffers();
        void setMesh(const BasicMesh& mesh){ this->mesh = mesh; }
        void bind();
        void addBlock(BasicMesh blockMesh);//instead of refreshing the whole chunk mesh
        size_t getIndexCount() const { return mesh.indices.size(); }
    };
    
} // namespace Geom
