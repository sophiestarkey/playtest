#include <cstdlib>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "graphics/camera.h"
#include "graphics/mesh.h"
#include "graphics/mesh_shader.h"
#include "graphics/transform.h"
#include "utility/file_io.h"
#include "utility/gl_wrapper.h"

GLFWwindow* window;

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

    glfwSwapInterval(1);
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void run()
{
    Camera camera;
    MeshShader mesh_shader;
    Mesh terrain("res/models/terrain.obj");
    Mesh player("res/models/suzanne.obj");
    Transform player_transform;

    camera.get_transform().set_origin({0.0f, 0.0f, -12.0f});
    camera.get_transform().set_orientation(-10.0f, {1.0f, 0.0f, 0.0f});

    double last_frame = glfwGetTime();
    glm::dvec2 last_cursor_pos;
    glfwGetCursorPos(window, &last_cursor_pos.x, &last_cursor_pos.y);

    while (!glfwWindowShouldClose(window)) {
        double current_frame = glfwGetTime();
        float dt = static_cast<float>(current_frame - last_frame);
        last_frame = current_frame;

        glm::dvec2 current_cursor_pos;
        glfwGetCursorPos(window, &current_cursor_pos.x, &current_cursor_pos.y);
        glm::dvec2 cursor_delta = current_cursor_pos - last_cursor_pos;
        last_cursor_pos = current_cursor_pos;

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            const float sensitivity = 0.1f;
            camera.get_transform().rotate(-cursor_delta.x * sensitivity, { 0.0f, 1.0f, 0.0f });
            camera.get_transform().rotate(-cursor_delta.y * sensitivity, camera.get_transform().right());
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        glm::ivec2 window_size;
        glfwGetFramebufferSize(window, &window_size.x, &window_size.y);

        if (window_size.x > 0 && window_size.y > 0) {
            glViewport(0, 0, window_size.x, window_size.y);
            camera.set_aspect_ratio(static_cast<float>(window_size.x) / window_size.y);
        }

        float movement_speed = 4.0f * dt;
        float rotation_speed = 180.0f * dt;
        glm::vec3 player_velocity = { 0.0f, 0.0f, 0.0f };

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) player_velocity += player_transform.forward() * movement_speed;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) player_velocity -= player_transform.right() * movement_speed;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) player_velocity -= player_transform.forward() * movement_speed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) player_velocity += player_transform.right() * movement_speed;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) player_velocity += player_transform.up() * movement_speed;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) player_velocity -= player_transform.up() * movement_speed;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) player_transform.rotate(rotation_speed, { 0.0f, 1.0f, 0.0f });
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) player_transform.rotate(-rotation_speed, { 0.0f, 1.0f, 0.0f });
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) player_transform.rotate(rotation_speed, player_transform.right());
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) player_transform.rotate(-rotation_speed, player_transform.right());

        player_transform.translate(player_velocity);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
        mesh_shader.use();
        mesh_shader.set_projection_matrix(camera.get_projection_matrix());
        mesh_shader.set_view_matrix(camera.get_transform().get_inverse_matrix());

        mesh_shader.set_color({ 1.0f, 0.5f, 0.5f });
        mesh_shader.set_model_matrix(player_transform.get_matrix());
        player.draw();

        mesh_shader.set_color({ 1.0f, 1.0f, 1.0f });
        mesh_shader.set_model_matrix(glm::mat4(1.0f));
        terrain.draw();

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