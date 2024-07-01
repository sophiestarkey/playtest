#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/geometric.hpp>
#include <stdexcept>

#include "geometry.h"

Geometry::Geometry(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);

    if (!scene) {
        throw std::runtime_error(std::string("Failed to read '") + path + "': " + importer.GetErrorString());
    }

    for (unsigned i = 0; i < scene->mNumMeshes; i++) {
        const aiMesh* mesh = scene->mMeshes[i];

        for (unsigned j = 0; j < mesh->mNumVertices; j++) {
            aiVector3D position = mesh->mVertices[j];
            aiVector3D normal = mesh->mNormals[j];

            Vertex vertex;
            vertex.m_position = glm::vec3(position.x, position.y, position.z);
            vertex.m_normal = glm::vec3(normal.x, normal.y, normal.z);
            m_vertices.push_back(vertex);
        }

        unsigned indices_offset = static_cast<unsigned>(m_indices.size());

        for (unsigned j = 0; j < mesh->mNumFaces; j++) {
            aiFace face = mesh->mFaces[j];

            for (unsigned k = 0; k < face.mNumIndices; k++) {
                m_indices.push_back(face.mIndices[k] + indices_offset);
            }
        }

        unsigned face_indices_offset = 0;

        for (unsigned j = 0; j < mesh->mNumFaces; j++) {
            aiFace ai_face = mesh->mFaces[j];
            Face face;

            face.m_indices = m_indices.data() + face_indices_offset;
            face.m_num_indices = ai_face.mNumIndices;
            m_faces.push_back(face);

            face_indices_offset += ai_face.mNumIndices;
        }
    }

    for (Face& face : m_faces) {
        glm::vec3 normal(0.0f);

        // calculate cross product for each consecutive pair of edges and
        // use the one with the greatest magnitude as the face normal
        for (unsigned i = 0; i < face.m_num_indices; i++) {
            Vertex v0 = m_vertices.at(face.m_indices[i]);
            Vertex v1 = m_vertices.at(face.m_indices[(i + 1) % face.m_num_indices]);
            Vertex v2 = m_vertices.at(face.m_indices[(i + 2) % face.m_num_indices]);

            glm::vec3 a = v2.m_position - v1.m_position;
            glm::vec3 b = v0.m_position - v1.m_position;

            glm::vec3 cross = glm::cross(a, b);
            if (glm::length(cross) > glm::length(normal)) {
                normal = cross;
            }
        }

        face.m_normal = glm::normalize(normal);
    }
}

const std::vector<Vertex>& Geometry::vertices() const
{
    return m_vertices;
}

const std::vector<Face>& Geometry::faces() const
{
    return m_faces;
}