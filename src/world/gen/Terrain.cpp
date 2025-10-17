#include "Terrain.hpp"
#include "Noise.hpp"
Terrain::Terrain(unsigned int seed,TerrainSettings terrainSettings):
    seed(seed),terrainSettings(terrainSettings)
{
}

int Terrain::getTerrainHeight(int x,int y){
    
    float amplitude = terrainSettings.amplitude;
    float frequency = terrainSettings.frequency;
    float result = 0.0f;
    for (int i = 0; i < terrainSettings.octaves; i++) { 
        result += amplitude *  Noise::perlin(seed,x*frequency, y * frequency);
        amplitude *= 0.5;
        frequency *= 2.0;
    }

    return static_cast<int>(round(result) + 1);
}
Terrain::TerrainDecoration Terrain::getTerrainDecoration(int x,int y){
    float result = Noise::perlin(seed,x, y);
    int i = floor(abs(result*3));
    return (i == 2 ) ? GRASS : NONE;
}