#include "Chunk.hpp"
#include <geom/meshes/BlockMesh.hpp>
#include <geom/meshes/PlantMesh.hpp>
#include <geom/meshes/LiquidMesh.hpp>
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
    // checks blocks on chunks and one that are connected on neighbors
    Blocks::BlockType Chunk::checkBlockAndNeighbors(int x, int y, int z)
    {
        if (y < 0 || y >= CHUNK_HEIGHT)
            return BLOCK_AIR;

        if (x >= 0 && x < CHUNK_SIZE && z >= 0 && z < CHUNK_SIZE) // check on chunk itself
            return (blocks[getIndex(x, y, z)]);

        if (z == CHUNK_SIZE && x >= 0 && x < CHUNK_SIZE) // check north neighbor
        {
            if (auto north = neighbors[NORTH].lock())
            {
                return (north->blocks[getIndex(x, y, 0)]);
            }
            return BLOCK_AIR;
        }
        if (z == -1 && x >= 0 && x < CHUNK_SIZE) // check south neighbord
        {
            if (auto south = neighbors[SOUTH].lock())
            {
                return (south->blocks[getIndex(x, y, CHUNK_SIZE - 1)]);
            }
            return BLOCK_AIR;
        }
        if (x == CHUNK_SIZE && z >= 0 && z < CHUNK_SIZE) // check east neighbord
        {
            if (auto east = neighbors[EAST].lock())
            {
                return (east->blocks[getIndex(0, y, z)]);
            }
            return BLOCK_AIR;
        }
        if (x == -1 && z >= 0 && z < CHUNK_SIZE) // check west neighbord
        {
            if (auto west = neighbors[WEST].lock())
            {
                return (west->blocks[getIndex(CHUNK_SIZE - 1, y, z)]);
            }
            return BLOCK_AIR;
        }

        return BLOCK_AIR;
    }

    // checks if blocks is covered by any other (when it is then returns face as false so it wont be remembered in chunkMesh)
    Geom::SelectedFaces Chunk::getVisibleFaces(int x, int y, int z)
    {
        return Geom::SelectedFaces{
            // if neighbor is transparent then it returns true so face is visible
            Blocks::isBlockTransparent(checkBlockAndNeighbors(x, y, z + 1)), // north
            Blocks::isBlockTransparent(checkBlockAndNeighbors(x, y, z - 1)), // south
            Blocks::isBlockTransparent(checkBlockAndNeighbors(x + 1, y, z)), // east
            Blocks::isBlockTransparent(checkBlockAndNeighbors(x - 1, y, z)), // west
            Blocks::isBlockTransparent(checkBlockAndNeighbors(x, y + 1, z)), // top
            Blocks::isBlockTransparent(checkBlockAndNeighbors(x, y - 1, z)), // bottom
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
    std::shared_ptr<Chunk> Chunk::generateChunkFromTerrain(glm::ivec2 chunkPosition, Terrain terrain)
    {
        auto result = std::make_shared<Chunk>(chunkPosition);
        for (int x = 0; x < CHUNK_SIZE; x++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                const int waterLevel = terrain.getWaterLevel();
                glm::ivec2 blockWorldPos = glm::ivec2(x + CHUNK_SIZE * chunkPosition.x, z + CHUNK_SIZE * chunkPosition.y);
                int height = terrain.getTerrainHeight(blockWorldPos.x, blockWorldPos.y);

                for (int y = 0; y < height; y++) // sets basic height level
                {
                    if (y == height - 1)
                        result->blocks[result->getIndex(x, y, z)] = BLOCK_GRASS;
                    else if (y > height / 2)
                        result->blocks[result->getIndex(x, y, z)] = BLOCK_DIRT;
                    else
                        result->blocks[result->getIndex(x, y, z)] = BLOCK_STONE;
                }

                if (!terrain.getTree(blockWorldPos.x, blockWorldPos.y)) // create leaves for nearby tree
                {
                    // check neighbor blocks for tree
                    for (int i = -2; i <= 2; i++)
                        for (int j = -2; j <= 2; j++)
                        {
                            glm::ivec2 neighborPos = blockWorldPos + glm::ivec2(i, j);
                            if (terrain.getTree(neighborPos.x, neighborPos.y))
                            {
                                int neighborHeight = terrain.getTerrainHeight(neighborPos.x, neighborPos.y);

                                if (neighborHeight <= waterLevel)
                                    break;

                                int leavesHeight = 5;

                                if (abs(i) == 1 && abs(j) == 1)
                                    leavesHeight = 6;
                                if ((i == 0 && abs(j) < 2) || (abs(i) < 2 && j == 0))
                                    leavesHeight = 7;

                                for (int i = neighborHeight + 2; i < neighborHeight + leavesHeight; i++)
                                {
                                    result->blocks[result->getIndex(x, i, z)] = BLOCK_LEAVES;
                                }
                            }
                        }
                }
                
                if (height <= waterLevel) //when terrain is lower than water level
                {
                    // creates water and sand underneath
                    for (int y = waterLevel; y >= 0; y--)
                    {
                        if (result->blocks[result->getIndex(x, y, z)] != BLOCK_AIR)
                        {
                            result->blocks[result->getIndex(x, y, z)] = BLOCK_SAND;
                            break;
                        }
                        result->blocks[result->getIndex(x, y, z)] = LIQUID_WATER;
                    }
                    if (height == waterLevel) // creates sand around water
                    {
                        int waterBlocksCount = 0;
                        waterBlocksCount += (int)(terrain.getTerrainHeight(blockWorldPos.x + 1, blockWorldPos.y) <= 4);
                        waterBlocksCount += (int)(terrain.getTerrainHeight(blockWorldPos.x - 1, blockWorldPos.y) <= 4);
                        waterBlocksCount += (int)(terrain.getTerrainHeight(blockWorldPos.x, blockWorldPos.y + 1) <= 4);
                        waterBlocksCount += (int)(terrain.getTerrainHeight(blockWorldPos.x, blockWorldPos.y - 1) <= 4);
                        if (waterBlocksCount <= 3)
                            result->blocks[result->getIndex(x, height, z)] = BLOCK_SAND;
                    }
                    continue;
                }

                Terrain::TerrainDecoration deco = terrain.getTerrainDecoration(blockWorldPos.x, blockWorldPos.y);
                if (deco != Terrain::NONE && result->blocks[result->getIndex(x, height - 1, z)] == BLOCK_GRASS && height > waterLevel)
                {
                    result->blocks[result->getIndex(x, height, z)] = static_cast<BlockType>(deco);
                }

                // creates trunk of tree and leaves on top
                if (terrain.getTree(blockWorldPos.x, blockWorldPos.y) && height > waterLevel)
                {
                    for (int i = height; i < height + 7; i++)
                    {
                        result->blocks[result->getIndex(x, i, z)] = (height + 5 > i ? BLOCK_WOOD : BLOCK_LEAVES);
                    }
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
        auto [mesh, mesh2] = getBlocksMesh();
        mainBlocksMesh.setMesh(mesh);
        transparentBlocksMesh.setMesh(mesh2);
    }
    void Chunk::refreshChunkAndNeighbors()
    {
        for (int i = 0; i < 4; i++)
        {
            refreshNeighborIfExists(static_cast<ChunkDirection>(i));
        }
        refreshChunk();
    }
    std::tuple<Geom::BasicMesh, Geom::BasicMesh> Chunk::getBlocksMesh()
    {
        blocksIndicesCount = 0;
        // it appends meshes to 2 diffrent lists that are later merged but prioritize blocks so they can be only rendered wihtout details
        std::vector<Geom::BasicMesh> blocksMeshes;
        std::vector<Geom::BasicMesh> transparentMeshes;
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
                            z + chunkPosition.y * CHUNK_SIZE);

                        if (block.blockMeshType == Blocks::BlockMesh::PLANT_MESH)
                        {
                            Geom::BasicMesh plant = Geom::generatePlantMesh(worldPos, block.textures.north);
                            otherMeshes.push_back(plant);
                            continue;
                        }
                        if (block.blockMeshType == Blocks::BlockMesh::LIQUID_MESH)
                        {
                            Geom::SelectedFaces visibleFaces = Geom::SelectedFaces{
                                // makes faces visible only for air and liqiuds with other id
                                (BLOCK_AIR == checkBlockAndNeighbors(x, y, z + 1) || (BLOCKS[checkBlockAndNeighbors(x, y, z + 1)].blockMeshType == BlockMesh::LIQUID_MESH && BLOCKS[checkBlockAndNeighbors(x, y, z + 1)].id != block.id)), // north
                                (BLOCK_AIR == checkBlockAndNeighbors(x, y, z - 1) || (BLOCKS[checkBlockAndNeighbors(x, y, z - 1)].blockMeshType == BlockMesh::LIQUID_MESH && BLOCKS[checkBlockAndNeighbors(x, y, z - 1)].id != block.id)), // south
                                (BLOCK_AIR == checkBlockAndNeighbors(x + 1, y, z) || (BLOCKS[checkBlockAndNeighbors(x + 1, y, z)].blockMeshType == BlockMesh::LIQUID_MESH && BLOCKS[checkBlockAndNeighbors(x + 1, y, z)].id != block.id)), // east
                                (BLOCK_AIR == checkBlockAndNeighbors(x - 1, y, z) || (BLOCKS[checkBlockAndNeighbors(x - 1, y, z)].blockMeshType == BlockMesh::LIQUID_MESH && BLOCKS[checkBlockAndNeighbors(x - 1, y, z)].id != block.id)), // west
                                (BLOCK_AIR == checkBlockAndNeighbors(x, y + 1, z) || (BLOCKS[checkBlockAndNeighbors(x, y + 1, z)].blockMeshType == BlockMesh::LIQUID_MESH && BLOCKS[checkBlockAndNeighbors(x, y + 1, z)].id != block.id)), // top
                                (BLOCK_AIR == checkBlockAndNeighbors(x, y - 1, z) || (BLOCKS[checkBlockAndNeighbors(x, y - 1, z)].blockMeshType == BlockMesh::LIQUID_MESH && BLOCKS[checkBlockAndNeighbors(x, y - 1, z)].id != block.id)), // bottom
                            };
                            Geom::BasicMesh newBlock = Geom::generateLiquidMesh(worldPos, visibleFaces, block.textures);
                            if (block.transparent)
                            {
                                transparentMeshes.push_back(newBlock);
                            }
                            else
                            {
                                blocksMeshes.push_back(newBlock);
                                blocksIndicesCount += visibleFaces.count() * 6;
                            }
                            continue;
                        }

                        Geom::SelectedFaces visibleFaces = getVisibleFaces(x, y, z);
                        if (block.transparent)
                        {
                            visibleFaces = Geom::SelectedFaces{
                                // makes faces visible only for air and liqiuds with other id
                                (BLOCKS[checkBlockAndNeighbors(x, y, z + 1)].id != block.id), // north
                                (BLOCKS[checkBlockAndNeighbors(x, y, z - 1)].id != block.id), // south
                                (BLOCKS[checkBlockAndNeighbors(x + 1, y, z)].id != block.id), // east
                                (BLOCKS[checkBlockAndNeighbors(x - 1, y, z)].id != block.id), // west
                                (BLOCKS[checkBlockAndNeighbors(x, y + 1, z)].id != block.id), // top
                                (BLOCKS[checkBlockAndNeighbors(x, y - 1, z)].id != block.id), // bottom
                            };

                            if (block.id == BLOCK_LEAVES)
                            { // leaves are always visible and its mesh is not connecting
                                visibleFaces = Geom::SelectedFaces::all();
                            }

                            Geom::BasicMesh newBlock = Geom::generateBlockMesh(worldPos, visibleFaces, block.textures);
                            transparentMeshes.push_back(newBlock);
                            continue;
                        }

                        blocksIndicesCount += visibleFaces.count() * 6;
                        Geom::BasicMesh newBlock = Geom::generateBlockMesh(worldPos, visibleFaces, block.textures);
                        blocksMeshes.push_back(newBlock);
                    }
                }
            }
        }

        blocksMeshes.insert(blocksMeshes.end(), otherMeshes.begin(), otherMeshes.end());
        return {mergeMeshes(blocksMeshes), mergeMeshes(transparentMeshes)};
    }
    void Chunk::setBlock(int x, int y, int z, BlockType type)
    {
        blocks[getIndex(x, y, z)] = type;
        if (BLOCKS[blocks[getIndex(x, y + 1, z)]].blockMeshType == BlockMesh::PLANT_MESH)
        {
            blocks[getIndex(x, y + 1, z)] = BLOCK_AIR;
        }
        refreshChunk();
        if (x == 0)
            refreshNeighborIfExists(WEST);
        else if (x == CHUNK_SIZE - 1)
            refreshNeighborIfExists(EAST);

        if (z == 0)
            refreshNeighborIfExists(SOUTH);
        else if (z == CHUNK_SIZE - 1)
            refreshNeighborIfExists(NORTH);
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
        mainBlocksMesh.bind();
        glDrawElements(GL_TRIANGLES, details ? mainBlocksMesh.getIndexCount() : blocksIndicesCount, GL_UNSIGNED_INT, 0);
        if (transparentBlocksMesh.getIndexCount() > 0)
        {
            transparentBlocksMesh.bind();
            glDrawElements(GL_TRIANGLES, transparentBlocksMesh.getIndexCount(), GL_UNSIGNED_INT, 0);
        }
    }
    void Chunk::setDetails(bool details)
    {
        this->details = details;
    }

    void Chunk::setNeighbor(std::weak_ptr<Chunk> neighbor, ChunkDirection direction)
    {
        neighbors[static_cast<int>(direction)] = neighbor;
    }

} // namespace Blocks