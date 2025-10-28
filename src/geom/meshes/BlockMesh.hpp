#pragma once
#include <geom/Quad.hpp>
#include "BasicMesh.hpp"
namespace Geom
{
    struct SelectedFaces
    {
        bool north;  // z+
        bool south;  // z-
        bool east;   // x+
        bool west;   // x-
        bool top;    // y+
        bool bottom; // y-
        static SelectedFaces all() { return {true, true, true, true, true, true}; };
        int count() const
        {
            return static_cast<int>(north) +
                   static_cast<int>(south) +
                   static_cast<int>(east) +
                   static_cast<int>(west) +
                   static_cast<int>(top) +
                   static_cast<int>(bottom);
        }
    };
    struct BlockTextureMap
    {
        int north;  // z+
        int south;  // z-
        int east;   // x+
        int west;   // x-
        int top;    // y+
        int bottom; // y-
        static BlockTextureMap set(int north, int south, int east, int west, int top, int bottom) { return {north, south, east, west, top, bottom}; };
        static BlockTextureMap all(int all) { return {all, all, all, all, all, all}; };
    };

    BasicMesh generateBlockMesh(glm::vec3 position, SelectedFaces faces = SelectedFaces::all(), BlockTextureMap textures = BlockTextureMap::all(0));

} // namespace Blocks
