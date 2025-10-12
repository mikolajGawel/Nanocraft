#pragma once
#include "world/World.hpp"
#include <glm/glm.hpp>
#include <optional>
namespace Raycast
{
    enum BlockSide
    {
        NORTH,
        SOUTH,
        EAST,
        WEST,
        TOP,
        BOTTOM
    };
    struct RaycastHit
    {
        glm::ivec3 blockPos;
        Blocks::BlockType blockType;
        BlockSide blockSide;
    };
    std::optional<RaycastHit> raycast(glm::vec3 direction, glm::vec3 position, float distance, World &world);
    std::string blockSideToString(BlockSide side);
} // namespace Raycast
