#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in float aTexIndex;
layout (location = 3) in float sideID;


out vec2 texCoords;
out float texIndex;
out vec3 normal;
uniform mat4 uProjection;
uniform mat4 uModel;
uniform mat4 uView;


const vec3 normals[6] = vec3[6](
    vec3(0,0,1),//north
    vec3(0,0,-1),//south
    vec3(1,0,0),//east
    vec3(-1,0,0),//west
    vec3(0,1,0),//top
    vec3(0,-1,0)//bottom
);

void main()
{
    int side_id = clamp(int(sideID), 0, 5);

    normal = normals[side_id];
    texIndex = aTexIndex;
    texCoords =  aTexCoords;
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}