#pragma once
#include <geom/BlockMesh.hpp>
#include <map>
namespace Blocks{
    enum BlockType{
        BLOCK_AIR = 0,
        BLOCK_DIRT,
        BLOCK_GRASS,
        BLOCK_STONE,
    };
    struct Block{
        BlockType id;
        Geom::BlockTextureMap textures;
    };
    extern std::map<BlockType,Block> BLOCKS;
    void initBlocks();
}