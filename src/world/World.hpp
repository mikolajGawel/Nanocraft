#pragma once
#include <blocks/Chunk.hpp>
#include <memory>
#include <unordered_map>
#include <graphics/Camera.hpp>
#include <future>
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
    std::unordered_map<glm::ivec2,std::shared_ptr<Blocks::Chunk>,ivec2_hash> loadedChunks;
    glm::ivec2 lastPlayerPosition = glm::ivec2(-1000);//stores last chunk position of player to avoid loading chunks every frame
    
    //returns chunk and nullptr if chunk not found
    std::optional<std::shared_ptr<Blocks::Chunk>> getChunk(glm::ivec2 position);
    void addChunk(std::shared_ptr<Blocks::Chunk> chunk);
    void checkChunkNeighbors(glm::ivec2 chunkPosition,bool appendToNeighbor = false);
    std::shared_ptr<Graphics::Camera> camera;//used for knowing position and generating new chunks
    uint8_t renderDistance = 4;
    std::mutex chunkRefreshingMutex;
public:
    World(std::shared_ptr<Graphics::Camera> camera,uint8_t renderDistance);
    ~World();
    void loadChunks(glm::vec3 positon);//loads chunks at location and remove unnecessary chunks
    void refreshNearby(glm::ivec2 position);//refreshes chunk and all its neighbors
    void refresh(std::vector<glm::ivec2> chunksToRefresh);
    glm::ivec2 getChunkPosition(glm::vec3 position);
    void update();
    void render();
};
