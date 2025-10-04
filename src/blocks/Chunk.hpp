#pragma once
#include "Block.hpp"
#include <geom/BasicMesh.hpp>
#include <geom/ChunkMesh.hpp>
#include <memory>
#include <optional>
namespace Blocks
{
    class Chunk
    {
    public:
        static const int CHUNK_SIZE = 16;
        static const int CHUNK_HEIGHT = 256;
    private:
        bool checkBlock(int x,int y,int z);
        Geom::SelectedFaces getVisibleFaces(int x,int y,int z);
        Geom::BasicMesh getBlocksMesh();
        
        Geom::ChunkMesh chunkMesh;
        std::array<BlockType, CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE> blocks;
        size_t getIndex(int x, int y, int z) const;
    public:
        glm::ivec2 chunkPosition; // chunk position in chunk coordinates
        Chunk(glm::ivec2 chunkPosition);
        static Chunk generateFlatChunk(glm::ivec2 chunkPosition,uint8_t height);

        BlockType getBlock(int x, int y, int z) const;
        void setBlock(int x, int y, int z, BlockType type);
        void refreshChunk();
        Geom::ChunkMesh& getChunkMesh(){ return chunkMesh; }
    };
} // namespace Blocks
