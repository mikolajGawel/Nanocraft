#include "Chunk.hpp"
#include <geom/BlockMesh.hpp>
#include <iostream>
#include <stdexcept>
namespace Blocks
{
    Chunk::ChunkDirection Chunk::getOpositeChunkDirection(ChunkDirection direction)
    {
        switch (direction)
        {
        case NORTH:
            return SOUTH;
        case SOUTH:
            return NORTH;
        case EAST:
            return WEST;
        case WEST:
            return EAST;
        }
        throw std::runtime_error("Undefined ChunkDirection: " + direction);
        return NORTH;
    }
    //if block is air(empty it returns false) 
    bool Chunk::checkBlock(int x, int y, int z)
    {
        if (y < 0 || y >= CHUNK_HEIGHT)
            return false;
            
        if (x >= 0 && x < CHUNK_SIZE && z >= 0 && z < CHUNK_SIZE)//check on chunk itself
            return (blocks[getIndex(x, y, z)] != BLOCK_AIR);

        if (z == CHUNK_SIZE && x >= 0 && x < CHUNK_SIZE)//check north neighbor
        {
            if (auto north = neighbors[NORTH].lock())
            {
                return (north->blocks[getIndex(x, y, 0)] != BLOCK_AIR);
            }
            return false;
        }
        if (z == -1 && x >= 0 && x < CHUNK_SIZE)//check south neighbord
        {
            if (auto south = neighbors[SOUTH].lock())
            {
                return (south->blocks[getIndex(x, y, CHUNK_SIZE-1)] != BLOCK_AIR);
            }
            return false;
        }
        if (x == CHUNK_SIZE && z >= 0 && z < CHUNK_SIZE)//check east neighbord
        {
            if (auto east = neighbors[EAST].lock())
            {
                return (east->blocks[getIndex(0, y, z)] != BLOCK_AIR);
            }
            return false;
        }
        if (x == -1 && z >= 0 && z < CHUNK_SIZE)//check west neighbord
        {
            if (auto west = neighbors[WEST].lock())
            {
                return (west->blocks[getIndex(CHUNK_SIZE-1, y, z)] != BLOCK_AIR);
            }
            return false;
        }

        return false;
    }

    Geom::SelectedFaces Chunk::getVisibleFaces(int x, int y, int z)
    {
        return Geom::SelectedFaces{
            !checkBlock(x, y, z + 1),//north
            !checkBlock(x, y, z - 1),//south
            !checkBlock(x + 1, y, z),//east
            !checkBlock(x - 1, y, z),//west
            !checkBlock(x, y + 1, z),
            !checkBlock(x, y - 1, z),
        };
    }
    size_t Chunk::getIndex(int x, int y, int z) const
    {
        if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_SIZE)
        {
            std::cerr << "Chunk::getIndex out of bounds: " << x << " " << y << " " << z << std::endl;
            return 0;
        }
        return x + CHUNK_SIZE * (z + CHUNK_SIZE * y);
    }
    Chunk::Chunk(glm::ivec2 chunkPosition) : chunkPosition(chunkPosition)
    {
        blocks.fill(BLOCK_AIR);
    }
    Chunk Chunk::generateFlatChunk(glm::ivec2 chunkPosition, uint8_t height)
    {
        Chunk result(chunkPosition);
        for (int x = 0; x < CHUNK_SIZE; x++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                for (int y = 0; y < height; y++)
                {
                    if (y == height - 1)
                        result.blocks[result.getIndex(x, y, z)] = BLOCK_GRASS;
                    else if (y > height / 2)
                        result.blocks[result.getIndex(x, y, z)] = BLOCK_DIRT;
                    else
                        result.blocks[result.getIndex(x, y, z)] = BLOCK_STONE;
                }
            }
        }
        result.blocks[result.getIndex(0,height,0)] = BLOCK_STONE;
        result.refreshChunk();
        return result;
    }
    void Chunk::refreshChunk()
    {
        auto mesh = getBlocksMesh();
        chunkMesh.setMesh(mesh);
    }
    void Chunk::refreshChunkAndNeighbors(){
        refreshChunk();
        for(int i = 0;i < 4;i++){
            if(auto neighbor = neighbors[i].lock()){
                neighbor->refreshChunk();
            }
        }
    }
    Geom::BasicMesh Chunk::getBlocksMesh()
    {
        std::vector<Geom::BasicMesh> blocksMeshes;
        for (int x = 0; x < CHUNK_SIZE; x++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                for (int y = 0; y < CHUNK_HEIGHT; y++)
                {
                    if (blocks[getIndex(x, y, z)] > 0)
                    {
                        Geom::SelectedFaces visibleFaces = getVisibleFaces(x, y, z);
                        Block block = BLOCKS[blocks[getIndex(x, y, z)]];
                        glm::vec3 worldPos = glm::vec3(
                            x + chunkPosition.x * CHUNK_SIZE,
                            y,
                            z + chunkPosition.y * CHUNK_SIZE);

                        Geom::BasicMesh newBlock = Geom::generateBlockMesh(worldPos, visibleFaces, block.textures);
                        blocksMeshes.push_back(newBlock);
                    }
                }
            }
        }
        return mergeMeshes(blocksMeshes);
    }
    BlockType Chunk::getBlock(int x, int y, int z) const
    {
        if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_SIZE)
        {
            std::cerr << "Chunk::getBlock out of bounds: " << x << " " << y << " " << z << std::endl;
            return BLOCK_AIR;
        }
        return blocks[getIndex(x, y, z)];
    }
    void Chunk::setNeighbor(std::weak_ptr<Chunk> neighbor, ChunkDirection direction)
    {
        neighbors[static_cast<int>(direction)] = neighbor;
    }

} // namespace Blocks