#pragma once
#include "world/World.hpp"
#include <glm/glm.hpp>
#include <optional>
namespace Raycast
{
    enum BlockSide
    {
        NORTH  = 0,
        SOUTH  = 1,
        EAST   = 2,
        WEST   = 3,
        TOP    = 4,
        BOTTOM = 5
    };
    struct RaycastHit
    {
        glm::ivec3 blockPos;
        Blocks::BlockType blockType;
        BlockSide blockSide;
    };
    std::optional<RaycastHit> raycast(glm::vec3 direction, glm::vec3 position, float distance, World &world);
    std::string blockSideToString(BlockSide side);
    glm::ivec3 sideToCoordinates(BlockSide side);
} // namespace Raycast
