#pragma once
#include <glm/glm.hpp>
class Terrain
{
public:
    struct TerrainSettings
    {
        float amplitude;//max height
        float frequency;
        int octaves;
    };

private:
    unsigned int seed;
    TerrainSettings terrainSettings;
public:
    Terrain(unsigned int seed, TerrainSettings terrainSettings = {20.0f,0.02f,6});
    int getTerrainHeight(int x,int y);
};
