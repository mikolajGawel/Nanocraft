#pragma once
#include <glm/glm.hpp>
class Terrain
{
public:
    struct TerrainSettings
    {
        unsigned int regionSize; // space between
        unsigned int peakHeight; // highest level of terrain
        // unsigned int miningHeigh;//
    };

private:
    unsigned int seed;
    TerrainSettings terrainSettings;
public:
    Terrain(unsigned int seed, TerrainSettings terrainSettings = {10, 16});
    int getTerrainHeight(glm::ivec2 position_xz);
};
