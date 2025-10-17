#pragma once
#include <geom/BlockMesh.hpp>
#include <map>
namespace Blocks{
    enum BlockType : uint8_t{
        BLOCK_AIR = 0,
        BLOCK_DIRT,
        BLOCK_GRASS,
        BLOCK_STONE,
        PLANT_GRASS,
        PLANT_FLOWER1,
        PLANT_FLOWER2,
    };
    enum BlockMesh{
        BLOCK_MESH = 0,
        PLANT_MESH
    };
    struct Block{
        BlockType id;
        bool transparent;//means that you can see other blocks thru this e.g.: water,glass,air
        Geom::BlockTextureMap textures;
        BlockMesh blockMeshType = BLOCK_MESH;
    };
    extern std::map<BlockType,Block> BLOCKS;
    bool isBlockTransparent(BlockType blockType);
    void initBlocks();
}