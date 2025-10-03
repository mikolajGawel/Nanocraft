#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in float aTexIndex;

out vec2 texCoords;
out float texIndex;

uniform mat4 uProjection;
uniform mat4 uModel;
uniform mat4 uView;

void main()
{
    texIndex = aTexIndex;
    texCoords =  aTexCoords;
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}