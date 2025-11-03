#pragma once
#include <blocks/Block.hpp>
#include <glm/glm.hpp>
class Terrain
{
public:
    struct TerrainSettings
    {
        float amplitude;//max height
        float frequency;
        int octaves;

        int waterLevel = 4;
    };
    enum TerrainDecoration: uint8_t{
        NONE = Blocks::BLOCK_AIR,
        GRASS = Blocks::PLANT_GRASS,
        FLOWER1 = Blocks::PLANT_FLOWER1,
        FLOWER2 = Blocks::PLANT_FLOWER2,

        TRUNK = Blocks::BLOCK_WOOD,
        LEAVES = Blocks::BLOCK_LEAVES,
        LEAVES2 = Blocks::BLOCK_LEAVES
    };
private:
    unsigned int seed;
    TerrainSettings terrainSettings;
public:
    Terrain(unsigned int seed, TerrainSettings terrainSettings = {20.0f,0.02f,6,4});
    int getTerrainHeight(int x,int y);
    TerrainDecoration getTerrainDecoration(int x,int y);
    int getWaterLevel();
    bool getTree(int x,int y);
};
