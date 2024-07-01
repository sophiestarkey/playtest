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

// adapted from https://stackoverflow.com/a/42752998
bool intersect_triangle(const glm::vec3& O, const glm::vec3& D, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
{
    glm::vec3 E1 = b - a;
    glm::vec3 E2 = c - a;
    glm::vec3 N = glm::cross(E1, E2);
    float det = -glm::dot(D, N);
    float invdet = 1.0f / det;
    glm::vec3 AO = O - a;
    glm::vec3 DAO = glm::cross(AO, D);
    float u = glm::dot(E2, DAO) * invdet;
    float v = -glm::dot(E1, DAO) * invdet;
    float t = glm::dot(AO, N) * invdet;
    return (det >= 1.0e-6f && t >= 0.0f && t <= 1.0f && u >= 0.0f && v >= 0.0f && (u + v) <= 1.0f);
}

/**
 * @brief Calculates the signed distance between a point and a plane.
 *
 * The result will be positive if the point is above the plane,
 * and negative if the point is below the plane.
 *
 * @param P The position of the point.
 * @param N The plane's normal vector, i.e. a unit vector perpendicular to the plane.
 * @param D A scalar representing the signed distance from the origin to the closest point on the plane.
 * @return The signed distance from point P to the plane.
 */
float signed_distance_to_plane(const glm::vec3& P, const glm::vec3& N, float D)
{
    return glm::dot(N, P) - D;
}

glm::vec3 intersect_ray_plane(const glm::vec3& ray_O, const glm::vec3& ray_D, const glm::vec3& plane_N, float plane_D)
{
    float t = (plane_D - glm::dot(plane_N, ray_O)) / glm::dot(plane_N, ray_D);
    return ray_O + t * ray_D;
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

    glfwSwapInterval(1);
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void run()
{
    Camera camera;
    MeshShader mesh_shader;
    Geometry terrain_geometry("res/models/terrain.obj");
    Mesh terrain(terrain_geometry, true);
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

        bool collision;

        do {
            collision = false;

            for (const Face& face : terrain_geometry.faces()) {
                glm::vec3 v0 = terrain_geometry.vertices().at(face.m_indices[0]).m_position;
                glm::vec3 v1 = terrain_geometry.vertices().at(face.m_indices[1]).m_position;
                glm::vec3 v2 = terrain_geometry.vertices().at(face.m_indices[2]).m_position;

                if (intersect_triangle(player_transform.get_position(), player_velocity, v0, v1, v2)) {
                    glm::vec3 new_position = intersect_ray_plane(player_transform.get_position(), glm::normalize(player_velocity), face.m_normal, glm::dot(face.m_normal, v0));
                    glm::vec3 new_velocity = face.m_normal * -signed_distance_to_plane(player_transform.get_position() + player_velocity, face.m_normal, glm::dot(face.m_normal, v0));
                    player_transform.set_position(new_position);
                    player_velocity = new_velocity;
                    collision = true;
                }
            }
        } while (collision);

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