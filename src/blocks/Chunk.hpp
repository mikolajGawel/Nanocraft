#pragma once
#include "Block.hpp"
#include <memory>
namespace Blocks
{
    class Chunk
    {
    private:
        bool checkBlock(int x,int y,int z);
        Geom::SelectedFaces getVisibleFaces(int x,int y,int z);
    public:
        glm::ivec2 chunkPosition; // chunk position in chunk coordinates
        static const int CHUNK_SIZE = 16;
        static const int CHUNK_HEIGHT = 256;
        
        std::array<BlockType, CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE> blocks;
        size_t getIndex(int x, int y, int z) const {
            return x + CHUNK_SIZE * (z + CHUNK_SIZE * y);
        }
        Chunk(glm::ivec2 chunkPosition);
        Geom::BasicMesh getChunkMesh();
    };
} // namespace Blocks
