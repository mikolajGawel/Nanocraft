#pragma once
#include <iostream>
namespace Graphics{
    class Texture
    {
    protected:
        unsigned int textureID;
        std::string texturePath;
        int textureWidth, textureHeight;
    public:
        Texture(std::string texturePath);
        ~Texture();
        void loadTexture();
        unsigned int getTexureID();
    };
}