#version 330 core
out vec4 FragColor;

in vec2 texCoords;
in float texIndex;
in vec3 normal;
in vec3 position;

uniform sampler2D uTex;
uniform float uCellWidth;
uniform float uCellHeight;
uniform vec3 cameraPos;
uniform float fadeStart;

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
    vec4 texture_color = texture(uTex,coords);
    if(texture_color.a < 0.1) discard;


    //calculating distance and fog
    float dist = sqrt(pow(cameraPos.x-position.x,2) + pow(cameraPos.z-position.z,2));
    if(dist > fadeStart){
        float a = 1.0 - (dist-fadeStart)/16;
        texture_color.a *= a;
    }


    //added small level of static lights for better blocks visibility
    vec3 lightPos = vec3(1,1,1);
    float ambient = 0.5;
    vec3 lightDir = normalize(lightPos - vec3(0.0));
    float diff= max(dot(normal,lightDir),0.0);

    vec4 color = vec4(min(ambient+diff,1.0));
    color.a = 1.0;
    FragColor = texture_color * color;
}