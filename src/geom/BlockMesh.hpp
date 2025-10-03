#pragma once
#include <geom/Quad.hpp>
#include <geom/BasicMesh.hpp>
namespace Geom
{
    struct SelectedFaces
    {
        bool north; // x+
        bool south; // x-
        bool east; // z+
        bool west; // z-
        bool top; // y+
        bool bottom; // y-
        static SelectedFaces all(){return {true,true,true,true,true,true};};
    };
    struct BlockTextureMap{
        int north; // x+
        int south; // x-
        int east; // z+
        int west; // z-
        int top; // y+
        int bottom; // y-
        static BlockTextureMap set(int north, int south, int east, int west, int top, int bottom){return {north,south,east,west,top,bottom};};
        static BlockTextureMap all(int all){return {all,all,all,all,all,all};};
    };

    BasicMesh generateBlockMesh(glm::vec3 position,SelectedFaces faces = SelectedFaces::all(),BlockTextureMap textures = BlockTextureMap::all(0));

} // namespace Blocks    

