#include "ChunkMesh.hpp"
#include <GL/gl3w.h>
#include <iostream>
#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 256
namespace Geom
{
    ChunkMesh::ChunkMesh(): created(false), vao(0), vbo(0), ibo(0)
    {}
    void ChunkMesh::genBuffers(){
        int chunkMaxVerticesSize = (CHUNK_SIZE*CHUNK_SIZE*CHUNK_HEIGHT*sizeof(float)*6*24)/2; // max size of chunk mesh 
        int chunkMaxIndicesSize = (CHUNK_SIZE*CHUNK_SIZE*CHUNK_HEIGHT*sizeof(uint32_t)*36)/2; // max size of chunk mesh
        //(everything is divided by 2 because cull faces can create whole chunk with all faces)

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
glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.vertices.size(), vertices, GL_DYNAMIC_DRAW);

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * mesh.indices.size(), indices, GL_DYNAMIC_DRAW);
 

        glBindVertexArray(vao);
    }

} // namespace Geom
