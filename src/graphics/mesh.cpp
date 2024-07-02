#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/geometric.hpp>
#include <stdexcept>

#include "mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& elements)
{
    init(vertices, elements);
}

Mesh::Mesh(const CollisionMesh& geometry)
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> elements;

    for (const Triangle& triangle : geometry.triangles()) {
        Vertex v0, v1, v2;

        v0.m_position = triangle.m_vertices[0];
        v1.m_position = triangle.m_vertices[1];
        v2.m_position = triangle.m_vertices[2];

        glm::vec3 N = glm::normalize(glm::cross(v1.m_position - v0.m_position, v2.m_position - v0.m_position));
        v0.m_normal = v1.m_normal = v2.m_normal = N;
        
        vertices.push_back(v0);
        vertices.push_back(v1);
        vertices.push_back(v2);

        elements.push_back(vertices.size() - 3);
        elements.push_back(vertices.size() - 2);
        elements.push_back(vertices.size() - 1);
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