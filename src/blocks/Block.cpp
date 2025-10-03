#include "Block.hpp"
namespace Blocks{
    std::map<BlockType,Block> BLOCKS = {};
    void initBlocks(){
        BLOCKS[BLOCK_AIR] = {BLOCK_AIR,Geom::BlockTextureMap::all(0)};
        BLOCKS[BLOCK_DIRT] = {BLOCK_DIRT,Geom::BlockTextureMap::all(2)};

        Geom::BlockTextureMap grassTextures = Geom::BlockTextureMap::set(3,3,3,3,0,2);
        BLOCKS[BLOCK_GRASS] = {BLOCK_GRASS,grassTextures};
    }
}