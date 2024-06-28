#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stdexcept>

#include "mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& elements)
{
    init(vertices, elements);
}

Mesh::Mesh(const Geometry& geometry, bool use_face_normals)
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> elements;

    if (use_face_normals) {
        for (const Face& face : geometry.faces()) {
            Vertex v0 = geometry.vertices().at(face.m_indices[0]);
            v0.m_normal = face.m_normal;

            for (unsigned i = 2; i < face.m_num_indices; i++) {
                Vertex v1 = geometry.vertices().at(face.m_indices[i - 1]);
                Vertex v2 = geometry.vertices().at(face.m_indices[i]);
                v1.m_normal = face.m_normal;
                v2.m_normal = face.m_normal;

                elements.push_back(static_cast<GLuint>(vertices.size()));
                elements.push_back(static_cast<GLuint>(vertices.size() + 1));
                elements.push_back(static_cast<GLuint>(vertices.size() + 2));

                vertices.push_back(v0);
                vertices.push_back(v1);
                vertices.push_back(v2);
            }
        }
    }
    else {
        for (const Vertex& vertex : geometry.vertices()) {
            vertices.push_back(vertex);
        }

        for (const Face& face : geometry.faces()) {
            unsigned v0 = face.m_indices[0];

            for (unsigned i = 2; i < face.m_num_indices; i++) {
                unsigned v1 = face.m_indices[i - 1];
                unsigned v2 = face.m_indices[i];

                elements.push_back(v0);
                elements.push_back(v1);
                elements.push_back(v2);
            }
        }
    }

    init(vertices, elements);
}

Mesh::Mesh(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);

    if (!scene) {
        throw std::runtime_error(std::string("Failed to read '") + path + "': " + importer.GetErrorString());
    }

    std::vector<Vertex> vertices;
    std::vector<GLuint> elements;

    for (unsigned i = 0; i < scene->mNumMeshes; i++) {
        const aiMesh* mesh = scene->mMeshes[i];

        for (unsigned j = 0; j < mesh->mNumVertices; j++) {
            aiVector3D position = mesh->mVertices[j];
            aiVector3D normal = mesh->mNormals[j];

            Vertex vertex;
            vertex.m_position = glm::vec3(position.x, position.y, position.z);
            vertex.m_normal = glm::vec3(normal.x, normal.y, normal.z);
            vertices.push_back(vertex);
        }

        unsigned element_offset = static_cast<unsigned>(elements.size());

        for (unsigned j = 0; j < mesh->mNumFaces; j++) {
            aiFace face = mesh->mFaces[j];

            for (unsigned k = 0; k < face.mNumIndices; k++) {
                elements.push_back(face.mIndices[k] + element_offset);
            }
        }
    }

    init(vertices, elements);
}

void Mesh::init(const std::vector<Vertex>& vertices, const std::vector<GLuint>& elements)
{
    m_num_elements = static_cast<GLuint>(elements.size());

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * elements.size(), elements.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_position)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_normal)));
}

void Mesh::draw()
{
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_num_elements, GL_UNSIGNED_INT, static_cast<void*>(0));
}