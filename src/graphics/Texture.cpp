#include "Texture.hpp"
#include <GL/gl3w.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
using namespace Graphics;
Texture::Texture(std::string texturePath): 
    texturePath(texturePath),textureID(0){
    }

Texture::~Texture(){
    if(textureID != 0)
        glDeleteTextures(1,&textureID);
}
void Texture::loadTexture(){
    stbi_set_flip_vertically_on_load(1);
    
    int channels=4, width=1, height=1;
	unsigned char* image = stbi_load(texturePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    textureWidth = width;
	textureHeight = height;
	if (image == nullptr)
	{
        std::cout << "Texture at path :" + std::string(texturePath) + " doesn't exist\n";
		return;
	}
	
    glGenTextures(1,&textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,0);
}
unsigned int Texture::getTexureID(){
    return textureID;
}