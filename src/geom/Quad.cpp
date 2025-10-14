#include "Quad.hpp"
#include <GL/gl3w.h>

namespace Geom{
    static bool quadInitialized = false;
    unsigned int Quad::vao = 0;
    unsigned int Quad::vbo = 0;
    unsigned int Quad::ibo = 0;

    void Quad::init(){
        float vertices[16] = {
            -1,-1,0,0,
             1,-1,1,0,
             1, 1,1,1,
            -1, 1,0,1
        };
        uint indices[6] = {
            0,1,2,
            2,3,0
        };
        
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void *)(0));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void *)(sizeof(float) * 2));

        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        quadInitialized = true;
    }
    void Quad::bind(){
        if(!quadInitialized)
            init();
        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
    }
    void Quad::draw(){
        bind();
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
    }
}