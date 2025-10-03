#pragma once 
#include "Texture.hpp"
namespace Graphics
{
    class TextureAtlas : public Texture
    {
    private:
        int cellWidth, cellHeight;
    public:
        TextureAtlas(std::string texturePath, int cellWidth, int cellHeight): 
            Texture(texturePath), cellWidth(cellWidth), cellHeight(cellHeight)
        {}
        float getCellWidthRatio() { return (float)cellWidth/textureWidth; }
        float getCellHeightRatio() { return (float)cellHeight/textureHeight; }
    };
} // namespace Graphics
