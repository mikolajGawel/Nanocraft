#pragma once
#include <vector>
#include <cstdint>
namespace Geom
{
    struct BasicMesh
    {
        std::vector<float> vertices;
        std::vector<uint32_t> indices;
    };
    static BasicMesh mergeMeshes(const std::vector<BasicMesh> &meshes)
{
    BasicMesh merged;
    uint32_t offset = 0;
    for (auto &mesh : meshes)
    {
        for (uint32_t i : mesh.indices)
        {
            merged.indices.push_back(i + offset);
        }
        merged.vertices.insert(merged.vertices.end(), mesh.vertices.begin(), mesh.vertices.end());
        offset += mesh.vertices.size() / 6;
    }
    return merged;
}
} // namespace Geom
