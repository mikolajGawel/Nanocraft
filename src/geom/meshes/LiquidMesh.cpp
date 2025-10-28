#include "LiquidMesh.hpp"
#include "../BlockFace.hpp"

namespace Geom
{
    BasicMesh generateLiquidMesh(glm::vec3 position, SelectedFaces faces, BlockTextureMap textures)
    {
        //makes a version of block mesh that  has less height (0.9 instead of 1)
        BasicMesh mesh = generateBlockMesh(glm::vec3(0,0,0),faces,textures);
        for (int i = 0;i < mesh.vertices.size()/Geom::BlockFace::BlockFaceStride();i++)
        {   
            mesh.vertices[Geom::BlockFace::BlockFaceStride()*i + 0] += position.x; 
            mesh.vertices[Geom::BlockFace::BlockFaceStride()*i + 1] = 0.9 + position.y; 
            mesh.vertices[Geom::BlockFace::BlockFaceStride()*i + 2] +=position.z; 
        }   
        return {mesh};
    }
}