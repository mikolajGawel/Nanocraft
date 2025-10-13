#version 330 core
out vec4 FragColor;

in vec2 texCoords;
in float texIndex;
in vec3 normal;

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

    //added small level of static lights for better blocks visibility
    vec3 lightPos = vec3(1,1,1);
    float ambient = 0.5;
    vec3 lightDir = normalize(lightPos - vec3(0.0));
    float diff= max(dot(normal,lightDir),0.0);

    vec4 color = vec4(min(ambient+diff,1.0));
    FragColor = texture(uTex,coords) * color;
}