#include "Chunk.hpp"
#include <geom/BlockMesh.hpp>
#include <geom/PlantMesh.hpp>
#include <iostream>
#include <stdexcept>
#include <GL/gl3w.h>
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
        throw std::runtime_error("Undefined ChunkDirection: " + std::to_string(direction));
        return NORTH;
    }
    // if block transparent e.g.: AIR GLASS WATER it returns false
    bool Chunk::checkBlockForFace(int x, int y, int z)
    {
        if (y < 0 || y >= CHUNK_HEIGHT)
            return false;

        if (x >= 0 && x < CHUNK_SIZE && z >= 0 && z < CHUNK_SIZE) // check on chunk itself
            return (!isBlockTransparent(blocks[getIndex(x, y, z)]));

        if (z == CHUNK_SIZE && x >= 0 && x < CHUNK_SIZE) // check north neighbor
        {
            if (auto north = neighbors[NORTH].lock())
            {
                return (!isBlockTransparent(north->blocks[getIndex(x, y, 0)]));
            }
            return false;
        }
        if (z == -1 && x >= 0 && x < CHUNK_SIZE) // check south neighbord
        {
            if (auto south = neighbors[SOUTH].lock())
            {
                return (!isBlockTransparent(south->blocks[getIndex(x, y, CHUNK_SIZE - 1)]));
            }
            return false;
        }
        if (x == CHUNK_SIZE && z >= 0 && z < CHUNK_SIZE) // check east neighbord
        {
            if (auto east = neighbors[EAST].lock())
            {
                return (!isBlockTransparent(east->blocks[getIndex(0, y, z)]));
            }
            return false;
        }
        if (x == -1 && z >= 0 && z < CHUNK_SIZE) // check west neighbord
        {
            if (auto west = neighbors[WEST].lock())
            {
                return (!isBlockTransparent(west->blocks[getIndex(CHUNK_SIZE - 1, y, z)]));
            }
            return false;
        }

        return false;
    }
    //checks if blocks is covered by any other (when it is then returns face as false so it wont be remembered in chunkMesh)
    Geom::SelectedFaces Chunk::getVisibleFaces(int x, int y, int z)
    {
        return Geom::SelectedFaces{
            !checkBlockForFace(x, y, z + 1), // north
            !checkBlockForFace(x, y, z - 1), // south
            !checkBlockForFace(x + 1, y, z), // east
            !checkBlockForFace(x - 1, y, z), // west
            !checkBlockForFace(x, y + 1, z), // top
            !checkBlockForFace(x, y - 1, z), // bottom
        };
    }
    size_t Chunk::getIndex(int x, int y, int z) const
    {
        if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_SIZE)
        {
            throw std::runtime_error("Chunk::getIndex out of bounds");

            return 0;
        }
        return x + CHUNK_SIZE * (z + CHUNK_SIZE * y);
    }
    void Chunk::refreshNeighborIfExists(ChunkDirection direction)
    {
        if (auto neighbor = neighbors[static_cast<int>(direction)].lock())
        {
            neighbor->refreshChunk();
        }
    }

    Chunk::Chunk(glm::ivec2 chunkPosition) : chunkPosition(chunkPosition)
    {
        blocks.fill(BLOCK_AIR);
    }
    std::shared_ptr<Chunk> Chunk::generateChunkFromTerrain(glm::ivec2 chunkPosition,Terrain terrain){
        auto result = std::make_shared<Chunk>(chunkPosition);
        for (int x = 0; x < CHUNK_SIZE; x++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                
                int height = terrain.getTerrainHeight(x + CHUNK_SIZE * chunkPosition.x,z + CHUNK_SIZE * chunkPosition.y);
                for (int y = 0; y < height; y++)
                {
                    if (y == height - 1)
                        result->blocks[result->getIndex(x, y, z)] = BLOCK_GRASS;
                    else if (y > height / 2)
                        result->blocks[result->getIndex(x, y, z)] = BLOCK_DIRT;
                    else
                        result->blocks[result->getIndex(x, y, z)] = BLOCK_STONE;

                }
                Terrain::TerrainDecoration deco = terrain.getTerrainDecoration(x + CHUNK_SIZE * chunkPosition.x,z + CHUNK_SIZE * chunkPosition.y);
                if(deco != Terrain::NONE && result->blocks[result->getIndex(x,height-1,z)] == BLOCK_GRASS){
                    result->blocks[result->getIndex(x,height,z)] = static_cast<BlockType>(deco);
                }
            }
        }
        return result;
    }

    std::shared_ptr<Chunk> Chunk::generateFlatChunk(glm::ivec2 chunkPosition, uint8_t height)
    {
        auto result = std::make_shared<Chunk>(chunkPosition);
        for (int x = 0; x < CHUNK_SIZE; x++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                for (int y = 0; y < height; y++)
                {
                    if (y == height - 1)
                        result->blocks[result->getIndex(x, y, z)] = BLOCK_GRASS;
                    else if (y > height / 2)
                        result->blocks[result->getIndex(x, y, z)] = BLOCK_DIRT;
                    else
                        result->blocks[result->getIndex(x, y, z)] = BLOCK_STONE;
                }
            }
        }
        return result;
    }
    void Chunk::refreshChunk()
    {
        auto mesh = getBlocksMesh();
        chunkMesh.setMesh(mesh);
    }
    void Chunk::refreshChunkAndNeighbors()
    {
        for (int i = 0; i < 4; i++)
        {
            refreshNeighborIfExists(static_cast<ChunkDirection>(i));
        }
        refreshChunk();
    }
    Geom::BasicMesh Chunk::getBlocksMesh()
    {
        blocksIndicesCount = 0;
        //it appends meshes to 2 diffrent lists that are later merged but prioritize blocks so they can be only rendered wihtout details
        std::vector<Geom::BasicMesh> blocksMeshes;
        std::vector<Geom::BasicMesh> otherMeshes;
        for (int x = 0; x < CHUNK_SIZE; x++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                for (int y = 0; y < CHUNK_HEIGHT; y++)
                {
                    Block block = BLOCKS[blocks[getIndex(x, y, z)]];
                    if (block.id != BLOCK_AIR)
                    {
                        glm::vec3 worldPos = glm::vec3(
                            x + chunkPosition.x * CHUNK_SIZE,
                            y,
                            z + chunkPosition.y * CHUNK_SIZE
                        );

                        if(block.blockMeshType == Blocks::BlockMesh::PLANT_MESH){
                            Geom::BasicMesh plant = Geom::generatePlantMesh(worldPos,block.textures.north);
                            otherMeshes.push_back(plant);
                            continue;
                        }
                        
                        Geom::SelectedFaces visibleFaces = getVisibleFaces(x, y, z);
                        blocksIndicesCount += visibleFaces.count()*6;
                        Geom::BasicMesh newBlock = Geom::generateBlockMesh(worldPos, visibleFaces, block.textures);
                        blocksMeshes.push_back(newBlock);
                    }
                }
            }
        }
        
        blocksMeshes.insert(blocksMeshes.end(),otherMeshes.begin(),otherMeshes.end());
        return mergeMeshes(blocksMeshes);
    }
    void Chunk::setBlock(int x, int y, int z, BlockType type)
    {
        blocks[getIndex(x, y, z)] = type;
        if(BLOCKS[blocks[getIndex(x, y+1, z)]].blockMeshType == BlockMesh::PLANT_MESH){
            blocks[getIndex(x, y+1, z)] = BLOCK_AIR;
        }
        refreshChunk();
        if (x == 0)refreshNeighborIfExists(WEST);
        else if (x == CHUNK_SIZE - 1)refreshNeighborIfExists(EAST);

        if (z == 0)refreshNeighborIfExists(SOUTH);
        else if (z == CHUNK_SIZE - 1)refreshNeighborIfExists(NORTH);

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
    void Chunk::drawChunk()
    {
        chunkMesh.bind();
        glDrawElements(GL_TRIANGLES,details ? chunkMesh.getIndexCount() : blocksIndicesCount, GL_UNSIGNED_INT, 0);
    }
    void Chunk::setDetails(bool details){
        this->details = details;
    }

    void Chunk::setNeighbor(std::weak_ptr<Chunk> neighbor, ChunkDirection direction)
    {
        neighbors[static_cast<int>(direction)] = neighbor;
    }

} // namespace Blocks