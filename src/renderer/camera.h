#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>

class Camera {
public:
    Camera();

    void set_aspect_ratio(float aspect_ratio);
    void set_field_of_view(float field_of_view);
    void set_near_plane(float near_plane);
    void set_far_plane(float far_plane);

    void set_position(const glm::vec3& position);
    void set_orientation(float angle, const glm::vec3& axis);

    const glm::mat4& projection_matrix() const;
    const glm::mat4& view_matrix() const;
private:
    mutable glm::mat4 m_proj_mat;
    mutable glm::mat4 m_view_mat;

    float m_aspect_ratio;
    float m_field_of_view;
    float m_near_plane;
    float m_far_plane;

    glm::vec3 m_position;
    glm::quat m_orientation;

    mutable bool m_recalc_proj;
    mutable bool m_recalc_view;
};
