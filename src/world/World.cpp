#include "World.hpp"
#include <GL/gl3w.h>
#include <algorithm>

World::World()
{
}
World::~World()
{
    loadedChunks.clear();
}
void World::addChunk(std::shared_ptr<Blocks::Chunk> chunk)
{
    auto chunkPosition = chunk->chunkPosition;
    auto checkChunks = loadedChunks.find(chunkPosition);
    if (checkChunks != loadedChunks.end())
    {
        throw std::invalid_argument("Chunk Loading Error:Chunk at this position allready exists");
    }
    chunk->refreshChunk();

    loadedChunks.insert({chunkPosition, chunk});
}

std::shared_ptr<Blocks::Chunk> World::getChunk(glm::ivec2 position)
{
    auto it = loadedChunks.find(position);
    if (it != loadedChunks.end())
        return it->second;
    return nullptr;
}

#include <iostream>
void World::update(glm::vec3 playerPosition)
{
    glm::ivec2 position = getChunkPosition(playerPosition);

    if (position == lastPlayerPosition)
        return; // preventing to rereading chunks that were already loaded

    lastPlayerPosition = position;
    for(int i = -2;i <= 2;i++){
        for(int j = -2;j <= 2;j++){
            glm::ivec2 offsetPos = position + glm::ivec2(i,j);
            auto currChunk = loadedChunks.find(offsetPos);
        
            if (currChunk == loadedChunks.end())
            { 
                auto newChunk = std::make_shared<Blocks::Chunk>(Blocks::Chunk::generateFlatChunk(offsetPos, 16));
                addChunk(newChunk);
                newChunk->refreshChunk();
            }
        }
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
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, chunk->getChunkMesh().getIndexCount(), GL_UNSIGNED_INT, 0);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}