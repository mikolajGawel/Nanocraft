#include "Block.hpp"
namespace Blocks{
    std::map<BlockType,Block> BLOCKS = {};
    void initBlocks(){
        BLOCKS[BLOCK_AIR] = {BLOCK_AIR,Geom::BlockTextureMap::all(0)};
        BLOCKS[BLOCK_DIRT] = {BLOCK_DIRT,Geom::BlockTextureMap::all(0)};

        Geom::BlockTextureMap grassTextures = Geom::BlockTextureMap::set(1,1,1,1,2,0);
        BLOCKS[BLOCK_GRASS] = {BLOCK_GRASS,grassTextures};

        BLOCKS[BLOCK_STONE] = {BLOCK_STONE,Geom::BlockTextureMap::all(3)};
    }
}