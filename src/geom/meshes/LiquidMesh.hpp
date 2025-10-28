#pragma once
#include <geom/Quad.hpp>
#include "BasicMesh.hpp"
#include "BlockMesh.hpp"
namespace Geom
{
    BasicMesh generateLiquidMesh(glm::vec3 position, SelectedFaces faces = SelectedFaces::all(), BlockTextureMap textures = BlockTextureMap::all(0));
} // namespace Blocks
