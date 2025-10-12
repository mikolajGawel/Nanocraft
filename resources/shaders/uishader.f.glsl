#version 330 core
out vec4 FragColor;

in vec2 texCoords;
uniform sampler2D uTex;

void main()
{
    FragColor = vec4(1,1,1,0.3);
}