#include "World.hpp"
#include <GL/gl3w.h>
#include <algorithm>
#include <stdexcept>
#include <future>

World::World(std::unique_ptr<Terrain> terrain,uint8_t renderDistance,uint8_t decorationRenderDistance):
    renderDistance(renderDistance),detailsRenderDistance(decorationRenderDistance),terrain(std::move(terrain))
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
        throw std::invalid_argument("Chunk Loading Error:Chunk at this position already exists");
    }
    loadedChunks.insert({chunkPosition, chunk});
}
void World::checkChunkNeighbors(glm::ivec2 chunkPosition,bool appendToNeighbor){
    auto checkChunks = loadedChunks.find(chunkPosition);
    if(checkChunks == loadedChunks.end()){
        throw std::invalid_argument("Trying to check not existing chunk");
    }
    auto chunk_map = *checkChunks;
    auto chunk = chunk_map.second;
    for (size_t i = 0; i < 4; i++) // check neighbors for chunk
    {
        glm::ivec2 neighborPos = chunkPosition + chunkDir[i];
        auto neighborOpt = getChunk(neighborPos);
        if (neighborOpt)
        {
            std::shared_ptr<Blocks::Chunk> neighbor = *neighborOpt;
            auto chunkDirection = static_cast<Blocks::Chunk::ChunkDirection>(i);
            chunk->setNeighbor(neighbor, chunkDirection);
            if(appendToNeighbor)
                neighbor->setNeighbor(chunk, Blocks::Chunk::getOpositeChunkDirection(chunkDirection));
        }
    }
}

std::optional<std::shared_ptr<Blocks::Chunk>> World::getChunk(glm::ivec2 position)
{
    auto it = loadedChunks.find(position);
    if (it != loadedChunks.end())
        return it->second;
    return std::nullopt;
}

#include <iostream>
void World::refreshNearby(glm::ivec2 position) // refreshes chunk and all its neighbors
{
    auto currChunk = loadedChunks.find(position);

    if (currChunk != loadedChunks.end())
    {
        currChunk->second->refreshChunkAndNeighbors();
    }
}
void refresh_chunk(std::shared_ptr<Blocks::Chunk> chunk){
    chunk->refreshChunk();
}
void World::refresh(std::vector<glm::ivec2> chunksToRefresh)
{
    std::vector<std::future<void>> future = {};
    if(!chunksToRefresh.empty()){
        for (glm::ivec2 chunkPosition : chunksToRefresh)
        {
        auto chunk = loadedChunks.at(chunkPosition);
        future.push_back(std::async(std::launch::async, refresh_chunk, chunk));
        }
    }else{
        for (auto chunk : loadedChunks)
    {
        future.push_back(std::async(std::launch::async, refresh_chunk, chunk.second));
    }
    }

    for(auto& f : future)
        f.get();  
}
Blocks::BlockType World::getBlock(glm::ivec3 coordinates){
    glm::ivec2 chunkPos = getChunkPosition(coordinates);
    glm::ivec3 inChunkPos = glm::mod(glm::vec3(coordinates),glm::vec3(Blocks::Chunk::CHUNK_SIZE,256,Blocks::Chunk::CHUNK_SIZE));
    auto chunk = getChunk(chunkPos);
    if(chunk){
        return chunk.value()->getBlock(inChunkPos.x,inChunkPos.y,inChunkPos.z);
    }
    return Blocks::BLOCK_AIR;
}
void World::setBlock(glm::ivec3 position, Blocks::BlockType blockType){
    glm::ivec2 chunkPos = getChunkPosition(position);
    glm::ivec3 inChunkPos = glm::mod(glm::vec3(position),glm::vec3(Blocks::Chunk::CHUNK_SIZE,256,Blocks::Chunk::CHUNK_SIZE));
    auto chunk = getChunk(chunkPos);
    if(chunk){
        chunk.value()->setBlock(inChunkPos.x,inChunkPos.y,inChunkPos.z,blockType);
        if (savedChunks.find(chunkPos) == savedChunks.end()){
            savedChunks.insert({chunkPos,chunk.value()});
        }
    }
}

static bool isChunkInRange(glm::ivec2 center,int renderDistance,glm::ivec2 chunkPos){
    auto offset = glm::abs(center - chunkPos);
    return (offset.x <= renderDistance && offset.y <= renderDistance);
}

void World::loadChunks(glm::vec3 position)
{
    glm::ivec2 center = getChunkPosition(position);
    for(auto it = loadedChunks.begin(); it != loadedChunks.end(); ){
        if(!isChunkInRange(center,renderDistance,it->first)){
            it = loadedChunks.erase(it);
        }else{
            it++;
        }
    }
    std::vector<std::future<std::shared_ptr<Blocks::Chunk>>> futureChunks = {};
    for (int i = -renderDistance; i <= renderDistance; i++)
    {
        for (int j = -renderDistance; j <= renderDistance; j++)
        {
            glm::ivec2 offsetPos = center + glm::ivec2(i, j);
            if (loadedChunks.find(offsetPos) != loadedChunks.end()){continue;}//if found chunk already on this pos then skip
            
            //checking is in saved chunks
            auto currChunk = savedChunks.find(offsetPos);
            if(currChunk != savedChunks.end())//load chunk back from memory
            {
                loadedChunks.insert({currChunk->first,currChunk->second});
                continue;
            }

            futureChunks.push_back(std::async(std::launch::async,Blocks::Chunk::generateChunkFromTerrain,offsetPos,*terrain));    
        }
    }
    std::vector<glm::ivec2> chunkPositions = {};
    for(auto& f_chunk: futureChunks){
        auto chunk = f_chunk.get();

        addChunk(chunk);
        chunkPositions.push_back(chunk->chunkPosition);
    }
    
    std::vector<glm::ivec2> chunksRefreshQueue = {};
    for (glm::ivec2 pos : chunkPositions)//updating neighbors in chunks
    {
        checkChunkNeighbors(pos,true);
        chunksRefreshQueue.push_back(pos);
        for(int i = 0;i < 4;i++){//appends neighbor to refresh queue
            glm::ivec2 neihgborPos = pos + chunkDir[i];
            if(getChunk(neihgborPos) && std::find(chunkPositions.begin(),chunkPositions.end(),neihgborPos) == chunkPositions.end()){
                chunksRefreshQueue.push_back(neihgborPos);
            }
        }
    }
    for(auto& chunk : loadedChunks){   
        if(chunk.second == nullptr)continue;
        glm::ivec2 distance = glm::abs(center - chunk.second->chunkPosition);
        bool inDetailsDistance = (distance.x <= detailsRenderDistance && distance.y <= detailsRenderDistance);
        chunk.second->setDetails(inDetailsDistance);
    }
    refresh(chunksRefreshQueue);
    //zrefreshuj nowe chunki
}
void World::update(glm::vec3 position)
{
    glm::ivec2 _position = getChunkPosition(position);

    if (_position == lastPlayerPosition)
        return; // preventing to rereading chunks that were already loaded
    lastPlayerPosition = _position;
    loadChunks(position);
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
        std::shared_ptr<Blocks::Chunk>& chunk = iter.second;
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CW);
        glCullFace(GL_BACK);
        chunk->drawChunk();
        glDisable(GL_CULL_FACE);
    }
}