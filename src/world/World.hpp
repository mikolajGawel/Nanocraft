#pragma once
#include <blocks/Chunk.hpp>
#include <memory>
#include <unordered_map>
#include <future>
#include "gen/Terrain.hpp"
class World
{
private:
    struct ivec2_hash
    {
        std::size_t operator()(const glm::ivec2 &v) const
        {
            return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1);
        }
    };
    uint8_t renderDistance,
        detailsRenderDistance;
    std::vector<glm::ivec2> renderingOrder;//for chunk to render in correct order (from farthest to nearest)


    std::unordered_map<glm::ivec2,std::shared_ptr<Blocks::Chunk>,ivec2_hash> loadedChunks;
    std::unordered_map<glm::ivec2,std::shared_ptr<Blocks::Chunk>,ivec2_hash> savedChunks;
    std::unique_ptr<Terrain> terrain;
    glm::ivec2 lastPlayerPosition;//stores last chunk position of player to avoid loading chunks every frame
    
    //returns chunk and nullptr if chunk not found
    std::optional<std::shared_ptr<Blocks::Chunk>> getChunk(glm::ivec2 position);
    void addChunk(std::shared_ptr<Blocks::Chunk> chunk);
    void checkChunkNeighbors(glm::ivec2 chunkPosition,bool appendToNeighbor = false);
    
    
        glm::ivec2 getChunkPosition(glm::vec3 position);
    public:
    World(std::unique_ptr<Terrain> terrain,uint8_t renderDistance,uint8_t detailsRenderDistance);
    ~World();
    std::vector<glm::ivec2> loadChunks(glm::vec3 positon);//loads chunks at location and remove unnecessary one, returns chunks to refresh
    void refreshNearby(glm::ivec2 position);//refreshes chunk and all its neighbors
    void refreshChunks(std::vector<glm::ivec2> chunksToRefresh);
    void refreshRenderingOrder(glm::ivec2 center);
    Blocks::BlockType getBlock(glm::ivec3 coordinates);
    void setBlock(glm::ivec3 position, Blocks::BlockType);
    void update(glm::vec3 position);
    void render();
};
