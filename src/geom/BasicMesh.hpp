#pragma once
#include <vector>
namespace Geom
{
    struct BasicMesh
    {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
    };
    static BasicMesh mergeMeshes(const std::vector<BasicMesh> &meshes)
{
    BasicMesh merged;
    uint offset = 0;
    for (auto &mesh : meshes)
    {
        for (uint i : mesh.indices)
        {
            merged.indices.push_back(i + offset);
        }
        merged.vertices.insert(merged.vertices.end(), mesh.vertices.begin(), mesh.vertices.end());
        offset += mesh.vertices.size() / 6;
    }
    return merged;
}
} // namespace Geom
