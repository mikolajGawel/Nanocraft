#include "Chunk.hpp"
#include <geom/BlockMesh.hpp>
#include <iostream>
namespace Blocks
{
    bool Chunk::checkBlock(int x, int y, int z)
    {
        if (x < 0 || x >= CHUNK_SIZE)
            return false;
        if (y < 0 || y >= CHUNK_HEIGHT)
            return false;
        if (z < 0 || z >= CHUNK_SIZE)
            return false;

        return (blocks[getIndex(x, y, z)] != BLOCK_AIR);
    }

    Geom::SelectedFaces Chunk::getVisibleFaces(int x, int y, int z)
    {
        return Geom::SelectedFaces{
            !checkBlock(x + 1, y, z),
            !checkBlock(x - 1, y, z),
            !checkBlock(x, y, z + 1),
            !checkBlock(x, y, z - 1),
            !checkBlock(x, y + 1, z),
            !checkBlock(x, y - 1, z),
        };
    }
    size_t Chunk::getIndex(int x, int y, int z) const
    {
        if(x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_SIZE)
        {
            std::cerr << "Chunk::getIndex out of bounds: " << x << " " << y << " " << z << std::endl;
            return 0;
        }
        return x + CHUNK_SIZE * (z + CHUNK_SIZE * y);
    }
    Chunk::Chunk(glm::ivec2 chunkPosition) : chunkPosition(chunkPosition)
    {
        blocks.fill(BLOCK_AIR);
        for (int x = 0; x < CHUNK_SIZE; x++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                for (int y = 0; y < CHUNK_SIZE; y++)
                {
                    if(y == CHUNK_SIZE - 1)
                        blocks[getIndex(x, y, z)] = BLOCK_GRASS;
                    else if (y > CHUNK_SIZE/2)
                        blocks[getIndex(x, y, z)] = BLOCK_DIRT;
                    else
                        blocks[getIndex(x, y, z)] = BLOCK_STONE;
                }
            }
        }
        refreshChunk();
    }
    void Chunk::refreshChunk()
    {
        auto mesh = getBlocksMesh();
        chunkMesh.setMesh(mesh);
    }

    Geom::BasicMesh Chunk::getBlocksMesh()
    {
        std::vector<Geom::BasicMesh> blocksMeshes;
        for (size_t x = 0; x < CHUNK_SIZE; x++)
        {
            for (size_t z = 0; z < CHUNK_SIZE; z++)
            {
                for (size_t y = 0; y < CHUNK_HEIGHT; y++)
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
    void Chunk::setBlock(int x, int y, int z, BlockType type)
    {
        if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_SIZE)
            return;
        if (blocks[getIndex(x, y, z)] == BLOCK_AIR && type != BLOCK_AIR)
        { // adding only block without refreshing the whole chunk
            Geom::SelectedFaces visibleFaces = getVisibleFaces(x, y, z);
            Block block = BLOCKS[type];
            glm::vec3 worldPos = glm::vec3(
                x + chunkPosition.x * CHUNK_SIZE,
                y,
                z + chunkPosition.y * CHUNK_SIZE);

            Geom::BasicMesh newBlock = Geom::generateBlockMesh(worldPos, visibleFaces, block.textures);
            chunkMesh.addBlock(newBlock);
            return;
        }
        blocks[getIndex(x, y, z)] = type;
        refreshChunk();
    }
} // namespace name
