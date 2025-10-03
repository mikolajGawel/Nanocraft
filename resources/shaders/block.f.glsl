#version 330 core
out vec4 FragColor;

in vec2 texCoords;
in float texIndex;

uniform sampler2D uTex;
uniform float uCellWidth;
uniform float uCellHeight;

void main()
{
    float max_in_row = floor(1.0/uCellWidth); 
    float tex_x = mod(texIndex,max_in_row);
    float tex_y = floor(texIndex/max_in_row);

    vec2 cellSize = vec2(uCellWidth,uCellHeight);
    vec2 offset = vec2(tex_x,tex_y) * cellSize;
    
    //flip verticaly atlas(from top to bottom)
    offset.y = 1.0 - cellSize.y -  offset.y;

    vec2 coords =  offset + texCoords * cellSize;

    FragColor = texture(uTex,coords);
}