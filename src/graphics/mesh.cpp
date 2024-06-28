#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stdexcept>

#include "mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& elements)
{
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

        unsigned element_offset = elements.size();

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
    m_num_elements = elements.size();

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