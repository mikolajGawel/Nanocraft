#include "Raycast.hpp"
namespace Raycast
{
    std::optional<RaycastHit> raycast(glm::vec3 direction, glm::vec3 position, float distance, World &world)
    {
        direction = glm::normalize(direction);
        glm::ivec3 currMapPos = glm::floor(position);

        glm::vec3 delta = {
            (direction.x == 0) ? 1e30 : std::abs(1 / direction.x),
            (direction.y == 0) ? 1e30 : std::abs(1 / direction.y),
            (direction.z == 0) ? 1e30 : std::abs(1 / direction.z)};
        glm::vec3 sideDist;

        glm::ivec3 step;
        BlockSide side;
        for (int i = 0; i < 3; i++)
        {
            if (direction[i] < 0)
            {
                step[i] = -1;
                sideDist[i] = (position[i] - currMapPos[i]) * delta[i];
            }
            else
            {
                step[i] = 1;
                sideDist[i] = (currMapPos[i] + 1.0 - position[i]) * delta[i];
            }
        }

        int safe = 30;
        while (safe > 0)
        {
            BlockSide lastSide = side;
            if (sideDist.x <= sideDist.y && sideDist.x <= sideDist.z)
            {
                sideDist.x += delta.x;
                currMapPos.x += step.x;
                side = step.x > 0 ? EAST : WEST;
            }
            else if (sideDist.y <= sideDist.z)
            {
                sideDist.y += delta.y;
                currMapPos.y += step.y;
                side = step.y > 0 ? TOP : BOTTOM;
            }
            else
            {
                sideDist.z += delta.z;
                currMapPos.z += step.z;
                side = step.z > 0 ? NORTH : SOUTH;
            }

            Blocks::BlockType hit = world.getBlock(currMapPos);
            if (hit != Blocks::BLOCK_AIR)
            {
                return (RaycastHit){
                    currMapPos,
                    hit,
                    lastSide};
            }
            safe--;
        }
        return std::nullopt;
    }
    std::string blockSideToString(BlockSide side)
    {
        switch (side)
        {
        case NORTH:
            return "NORTH";
        case SOUTH:
            return "SOUTH";
        case EAST:
            return "EAST";
        case WEST:
            return "WEST";
        case TOP:
            return "TOP";
        case BOTTOM:
            return "BOTTOM";
        default:
            return "UNKNOWN";
        }
    }
} // namespace Raycast
