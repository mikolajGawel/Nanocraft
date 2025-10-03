#pragma once
#include <geom/Quad.hpp>
namespace Blocks
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
    struct BlockMesh
    {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
    };
    class Block
    {
        
    public:
        SelectedFaces faces;
        BlockTextureMap textures;
        glm::vec3 position;
    
        Block(glm::vec3 pos,BlockTextureMap texMap = BlockTextureMap::all(0)): position(pos),faces(SelectedFaces::all()),textures(texMap) {};
        BlockMesh generateMesh();
    };
} // namespace Blocks    

