#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <cstdlib>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stb/stb_image.h>
#include <vector>

#include "renderer/camera.h"
#include "renderer/mesh_renderer.h"
#include "utility/file_io.h"
#include "utility/gl_wrapper.h"

GLFWwindow* window;

Mesh load_mesh(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);

    if (!scene) {
        throw std::runtime_error(std::string("Failed to load mesh '") + path + "': " + importer.GetErrorString());
    }

    std::vector<float> vertices;
    std::vector<unsigned> elements;

    for (unsigned i = 0; i < scene->mNumMeshes; i++) {
        const aiMesh* mesh = scene->mMeshes[i];
        for (unsigned j = 0; j < mesh->mNumVertices; j++) {
            aiVector3D vertex = mesh->mVertices[j];
            aiVector3D normal = mesh->mNormals[j];

            vertices.push_back(vertex.x);
            vertices.push_back(vertex.y);
            vertices.push_back(vertex.z);
            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);
        }

        unsigned index_offset = elements.size();

        for (unsigned j = 0; j < mesh->mNumFaces; j++) {
            aiFace face = mesh->mFaces[j];

            for (unsigned k = 0; k < face.mNumIndices; k++) {
                elements.push_back(face.mIndices[k] + index_offset);
            }
        }
    }

    Mesh mesh;
    mesh.count = elements.size();

    glBindVertexArray(mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(unsigned), elements.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    return mesh;
}

void error_callback(int error_code, const char* description)
{
    throw std::runtime_error(description);
}

void init()
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(960, 720, "playtest", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        glfwDestroyWindow(window);
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    stbi_set_flip_vertically_on_load(true);
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void run()
{
    Camera camera;
    MeshRenderer mesh_renderer;
    Mesh mesh = load_mesh("res/models/suzanne.obj");
    
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        // temporary hack to avoid GLM assertion error when aspect ratio = 0
        if (width != 0 && height != 0) {
            glViewport(0, 0, width, height);
            camera.set_aspect_ratio(static_cast<float>(width) / height);
        }

        camera.set_position({0.0f, 12.0f, 12.0f});
        camera.set_orientation(-45.0f, {1.0f, 0.0f, 0.0f});

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
        mesh_renderer.begin();
        mesh_renderer.set_projection_matrix(camera.projection_matrix());
        mesh_renderer.set_view_matrix(camera.view_matrix());

        glm::mat4 model_mat = glm::mat4(1.0f);
        mesh_renderer.set_model_matrix(model_mat);
        mesh_renderer.draw(mesh);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

int main(int argc, char** argv)
{
    try {
        init();
    } catch (std::exception& ex) {
        std::cerr << "[Error] " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    try {
        run();
    } catch (GL::Exception& ex) {
        std::cerr << "[OpenGL] " << ex.what() << std::endl;
    } catch (std::exception& ex) {
        std::cerr << "[Error] " << ex.what() << std::endl;
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}