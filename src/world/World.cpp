#include "World.hpp"
#include <GL/gl3w.h>
#include <algorithm>
#include <stdexcept>

World::World(std::shared_ptr<Graphics::Camera> camera,uint8_t renderDistance):
    camera(camera),renderDistance(renderDistance)
{
}
World::~World()
{
    loadedChunks.clear();
}
glm::ivec2 chunkDir[4] = {
    glm::ivec2(0, 1),  // north
    glm::ivec2(0, -1), // south
    glm::ivec2(1, 0),  // east
    glm::ivec2(-1, 0)  // west
};

void World::addChunk(std::shared_ptr<Blocks::Chunk> chunk)
{
    auto chunkPosition = chunk->chunkPosition;
    auto checkChunks = loadedChunks.find(chunkPosition);
    if (checkChunks != loadedChunks.end())
    {
        throw std::invalid_argument("Chunk Loading Error:Chunk at this position allready exists");
    }
    loadedChunks.insert({chunkPosition, chunk});
    for (size_t i = 0; i < 4; i++) // check neighbors for chunk
    {
        glm::ivec2 neighborPos = chunkPosition + chunkDir[i];
        auto neighborOpt = getChunk(neighborPos);
        if (neighborOpt)
        {
            std::shared_ptr<Blocks::Chunk> neighbor = *neighborOpt;
            auto chunkDirection = static_cast<Blocks::Chunk::ChunkDirection>(i);
            chunk->setNeighbor(neighbor, chunkDirection);
            neighbor->setNeighbor(chunk, Blocks::Chunk::getOpositeChunkDirection(chunkDirection));
            // neighbor->refreshChunk();
        }
    }
    // chunk->refreshChunk();
}

std::optional<std::shared_ptr<Blocks::Chunk>> World::getChunk(glm::ivec2 position)
{
    auto it = loadedChunks.find(position);
    if (it != loadedChunks.end())
        return it->second;
    return std::nullopt;
}

#include <iostream>
void World::create(glm::ivec2 origin)
{
 

    for (int i = -renderDistance; i <= renderDistance; i++)
    {
        for (int j = -renderDistance; j <= renderDistance; j++)
        {
            glm::ivec2 offsetPos = origin + glm::ivec2(i, j);
            auto currChunk = loadedChunks.find(offsetPos);

            if (currChunk == loadedChunks.end())
            {
                auto newChunk = std::make_shared<Blocks::Chunk>(Blocks::Chunk::generateFlatChunk(offsetPos, 16));
                addChunk(newChunk);
            }
        }
    }
}
void World::refreshNearby(glm::vec2 position) // refreshes chunk and all its neighbors
{
    auto currChunk = loadedChunks.find(position);

    if (currChunk != loadedChunks.end())
    {
        currChunk->second->refreshChunkAndNeighbors();
    }
}
void World::refresh()
{

    for (auto chunk : loadedChunks)
    {
        chunk.second->refreshChunk();
    }
    std::cout << "Chunks refreshed\n";    

}
void World::update()
{
    glm::ivec2 position = getChunkPosition(camera->position);

    if (position == lastPlayerPosition)
        return; // preventing to rereading chunks that were already loaded
    lastPlayerPosition = position;

    auto currChunk = loadedChunks.find(position);

    if (currChunk == loadedChunks.end())
    {
        auto newChunk = std::make_shared<Blocks::Chunk>(Blocks::Chunk::generateFlatChunk(position, 16));
        addChunk(newChunk);
        refreshNearby(position);
    }else{
        refreshNearby(position);
    }
}
glm::ivec2 World::getChunkPosition(glm::vec3 position)
{
    int chunkX = static_cast<int>(floor(position.x / Blocks::Chunk::CHUNK_SIZE));
    int chunkZ = static_cast<int>(floor(position.z / Blocks::Chunk::CHUNK_SIZE));
    return glm::ivec2(chunkX, chunkZ);
}
void World::render()
{
    for (auto &iter : loadedChunks)
    {
        std::shared_ptr<Blocks::Chunk> chunk = iter.second;
        chunk->getChunkMesh().bind();
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CW);
        glCullFace(GL_BACK);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, chunk->getChunkMesh().getIndexCount(), GL_UNSIGNED_INT, 0);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDisable(GL_CULL_FACE);
    }
}