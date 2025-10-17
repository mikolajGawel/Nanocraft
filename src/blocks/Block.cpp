#include "Block.hpp"
namespace Blocks{
    std::map<BlockType,Block> BLOCKS = {};
    void initBlocks(){
        BLOCKS[BLOCK_AIR] = {BLOCK_AIR,true,Geom::BlockTextureMap::all(0)};
        BLOCKS[BLOCK_DIRT] = {BLOCK_DIRT,false,Geom::BlockTextureMap::all(0)};

        Geom::BlockTextureMap grassTextures = Geom::BlockTextureMap::set(1,1,1,1,2,0);
        BLOCKS[BLOCK_GRASS] = {BLOCK_GRASS,false,grassTextures};

        BLOCKS[BLOCK_STONE] = {BLOCK_STONE,false,Geom::BlockTextureMap::all(3)};
        BLOCKS[PLANT_GRASS] = {PLANT_GRASS,true,Geom::BlockTextureMap::all(16),BlockMesh::PLANT_MESH};
    }
    bool isBlockTransparent(BlockType blockType){
        return BLOCKS[blockType].transparent;
    }

}