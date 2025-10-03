#include "Block.hpp"
#include "BlockFace.hpp"
#include <memory>

using namespace Blocks;

std::array<bool,6> faceToArray(SelectedFaces faces){
    return {faces.north, faces.south, faces.east, faces.west, faces.top, faces.bottom};
}
std::array<int,6> textureMapToArray(BlockTextureMap textures){
    return {textures.north, textures.south, textures.east, textures.west, textures.top, textures.bottom};
}

static std::array<Blocks::BlockFace,6> blockFaces = {
    // North face (x+)
    Blocks::BlockFace(
        std::array<Geom::Vertex,4>{
            Geom::Vertex{{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
            Geom::Vertex{{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
            Geom::Vertex{{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},
            Geom::Vertex{{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}}
        },{
            0, 1, 2,
            2, 3, 0
        }
    ),
    // South face (x-)
    Blocks::BlockFace(
        std::array<Geom::Vertex,4>{
            Geom::Vertex{{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},
            Geom::Vertex{{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},
            Geom::Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
            Geom::Vertex{{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}}
        },{
            0, 1, 2,
            2, 3, 0
        }
    ),
    // East face (z+)
    Blocks::BlockFace(
        std::array<Geom::Vertex,4>{
            Geom::Vertex{{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},
            Geom::Vertex{{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},
            Geom::Vertex{{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},
            Geom::Vertex{{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}}
        },{
            0, 1, 2,
            2, 3, 0
        }),
        // West face (z-)
    Blocks::BlockFace(
        std::array<Geom::Vertex,4>{
            Geom::Vertex{{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
            Geom::Vertex{{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
            Geom::Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f   , 0.0f}},
            Geom::Vertex{{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}}
        },{
            0, 1, 2,
            2, 3, 0
        }),
        // Top face (y+)
    Blocks::BlockFace(
        std::array<Geom::Vertex,4>{
            Geom::Vertex{{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},
            Geom::Vertex{{-0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},
            Geom::Vertex{{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f}},
            Geom::Vertex{{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}}
        },{
            0, 1, 2,
            2, 3, 0
        }),
        // Bottom face (y-)
    Blocks::BlockFace(
        std::array<Geom::Vertex,4>{ 
            Geom::Vertex{{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},
            Geom::Vertex{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
            Geom::Vertex{{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},
            Geom::Vertex{{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f}}
        },{
            0, 1, 2,
            2, 3, 0
        })
};



BlockMesh Block::generateMesh(){
    
    std::vector<float> vertices = {};
    std::vector<unsigned int> indices = {};
    std::array<bool,6> faceArray = faceToArray(this->faces);
    std::array<int,6> textureArray = textureMapToArray(this->textures);
    
    for(size_t i = 0; i < faceArray.size();i++){
        if(!faceArray[i])
            continue;
        
        Blocks::BlockFace face = blockFaces.at(i); 
        int offset = vertices.size()/6;
        
        for(uint i : face.indices){
            indices.push_back(i + offset);
        }
        auto faceVertices = face.getVerticesArray(position,textureArray.at(i));
        vertices.insert(vertices.end(), faceVertices.begin(),faceVertices.end());
    }
    return BlockMesh{vertices,indices};
}