#include "Chunk.hpp"
#include <geom/BlockMesh.hpp>
Geom::BasicMesh mergeMeshes(const std::vector<Geom::BasicMesh>& meshes){
    Geom::BasicMesh merged;
    uint offset = 0;
    for (auto& mesh: meshes)
    {
        for (uint i : mesh.indices)
        {
            merged.indices.push_back(i + offset);
        }
        merged.vertices.insert(merged.vertices.end(),mesh.vertices.begin(),mesh.vertices.end());
        offset += mesh.vertices.size()/6;
    }
    return merged;
}
namespace Blocks
{
    bool Chunk::checkBlock(int x,int y,int z){
        if(x < 0 || x >= CHUNK_SIZE)   return false;
        if(y < 0 || y >= CHUNK_HEIGHT) return false;
        if(z < 0 || z >= CHUNK_SIZE)   return false;

        return (blocks[getIndex(x,y,z)] != BLOCK_AIR);
    }

    Geom::SelectedFaces Chunk::getVisibleFaces(int x,int y,int z){
        return Geom::SelectedFaces{
            !checkBlock(x+1,y,    z  ),
            !checkBlock(x-1,y,    z  ),
            !checkBlock(x,  y,    z+1),
            !checkBlock(x,  y,    z-1),
            !checkBlock(x,  y+1,  z  ),
            !checkBlock(x,  y-1,  z  ),
        };
    }

    Chunk::Chunk(glm::ivec2 chunkPosition): chunkPosition(chunkPosition){
        blocks.fill(BLOCK_AIR);
        for(int x=0;x<CHUNK_SIZE;x++){
            for(int z=0;z<CHUNK_SIZE;z++){
                for(int y=0;y<CHUNK_SIZE;y++){
                    blocks[getIndex(x,y,z)] = BLOCK_GRASS;
                }
            }
        }
    }
    Geom::BasicMesh Chunk::getChunkMesh(){
        std::vector<Geom::BasicMesh> blocksMeshes;
        for (size_t x = 0; x < CHUNK_SIZE; x++){
            for (size_t z = 0; z < CHUNK_SIZE; z++){
                for (size_t y = 0; y < CHUNK_HEIGHT; y++){
                    if(blocks[getIndex(x,y,z)] > 0){
                        Geom::SelectedFaces visibleFaces = getVisibleFaces(x,y,z);
                        Block block = BLOCKS[blocks[getIndex(x,y,z)]];
                        glm::vec3 worldPos = glm::vec3(
                            x + chunkPosition.x * CHUNK_SIZE,
                            y,
                            z + chunkPosition.y * CHUNK_SIZE
                        );

                        Geom::BasicMesh newBlock = Geom::generateBlockMesh(worldPos,visibleFaces,block.textures);
                        blocksMeshes.push_back(newBlock);
                    }
                }
            }
        }
        return mergeMeshes(blocksMeshes);
    }
} // namespace name
