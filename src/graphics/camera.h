#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>

#include "../graphics/transform.h"

class Camera {
public:
    Camera();

    void set_aspect_ratio(float aspect_ratio);
    void set_field_of_view(float field_of_view);
    void set_near_plane(float near_plane);
    void set_far_plane(float far_plane);

    const glm::mat4& get_projection_matrix() const;
    Transform& get_transform();
private:
    Transform m_transform;
    mutable glm::mat4 m_projection_matrix;

    float m_aspect_ratio;
    float m_field_of_view;
    float m_near_plane;
    float m_far_plane;

    mutable bool m_recalculate;
    mutable bool m_recalc_view;
};
