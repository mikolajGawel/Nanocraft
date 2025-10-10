#pragma once
#include <blocks/Chunk.hpp>
#include <memory>
#include <unordered_map>
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
public:
    World();
    ~World();
    void refreshNearby(glm::vec2 position);//refreshes chunk and all its neighbors
    void refresh();
    glm::ivec2 getChunkPosition(glm::vec3 position);
    void update(glm::vec3 playerPosition);
    void create(glm::vec3 playerPosition);
    void render();
};
