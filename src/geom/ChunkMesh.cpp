#include "ChunkMesh.hpp"
#include <GL/gl3w.h>
#include <iostream>

namespace Geom
{
    ChunkMesh::ChunkMesh(): created(false), vao(0), vbo(0), ibo(0)
    {}
    void ChunkMesh::genBuffers(){
        int chunkMaxVerticesSize = 16*16*256*sizeof(float)*6*24; // max size of chunk mesh
        int chunkMaxIndicesSize = 16*16*256*sizeof(uint32_t)*36; // max size of chunk mesh
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER,  chunkMaxVerticesSize, nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(0));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(sizeof(float) * 3));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(sizeof(float) * 5));

        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunkMaxIndicesSize,nullptr, GL_DYNAMIC_DRAW);   
        created = true;
    }
    void ChunkMesh::bind(){
        if(!created) genBuffers();
        float* vertices = mesh.vertices.data();
        uint32_t* indices = mesh.indices.data();

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0,sizeof(float)*mesh.vertices.size(),vertices); 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,sizeof(uint32_t)*mesh.indices.size(),indices); 
        glBindVertexArray(vao);
    }
    void ChunkMesh::addBlock(BasicMesh blockMesh){
        this->mesh = mergeMeshes({mesh,blockMesh});
    }

} // namespace Geom
