#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/geometric.hpp>
#include <stdexcept>

#include "collision_mesh.h"

CollisionMesh::CollisionMesh(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

    if (!scene) {
        throw std::runtime_error(std::string("Failed to read '") + path + "': " + importer.GetErrorString());
    }

    for (unsigned i = 0; i < scene->mNumMeshes; i++) {
        const aiMesh* mesh = scene->mMeshes[i];

        for (unsigned j = 0; j < mesh->mNumFaces; j++) {
            aiFace face = mesh->mFaces[j];

            aiVector3D v0 = mesh->mVertices[face.mIndices[0]];
            aiVector3D v1 = mesh->mVertices[face.mIndices[1]];
            aiVector3D v2 = mesh->mVertices[face.mIndices[2]];

            Triangle triangle({ v0.x, v0.y, v0.z }, { v1.x, v1.y, v1.z }, {v2.x, v2.y, v2.z });
            m_triangles.push_back(triangle);
        }
    }
}

const std::vector<Triangle>& CollisionMesh::triangles() const
{
    return m_triangles;
}