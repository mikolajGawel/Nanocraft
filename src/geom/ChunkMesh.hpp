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
        bool uploaded = false;
        void uploadValues();//when mesh has changes than this method is invoked before next binding
    public:
        ChunkMesh();
        void genBuffers();
        void setMesh(const BasicMesh& mesh){ 
            uploaded = false;
            this->mesh = mesh; 
        }
        void bind();
        size_t getIndexCount() const { return mesh.indices.size(); }
    };
    
} // namespace Geom
