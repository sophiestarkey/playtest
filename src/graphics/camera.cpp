#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"

Camera::Camera() :
    m_aspect_ratio(1.0f),
    m_field_of_view(45.0f),
    m_near_plane(0.01f),
    m_far_plane(1000.0f),
    m_recalculate(true),
    m_recalc_view(true),
    m_projection_matrix(),
    m_transform()
{
}

void Camera::set_aspect_ratio(float aspect_ratio)
{
    m_aspect_ratio = aspect_ratio;
    m_recalculate = true;
}

void Camera::set_field_of_view(float field_of_view)
{
    m_field_of_view = field_of_view;
    m_recalculate = true;
}

void Camera::set_near_plane(float near_plane)
{
    m_near_plane = near_plane;
    m_recalculate = true;
}

void Camera::set_far_plane(float far_plane)
{
    m_far_plane = far_plane;
    m_recalculate = true;
}

const glm::mat4& Camera::get_projection_matrix() const
{
    if (m_recalculate) {
        m_projection_matrix = glm::perspective(glm::radians(m_field_of_view), m_aspect_ratio, m_near_plane, m_far_plane);
        m_recalculate = false;
    }
    
    return m_projection_matrix;
}

Transform& Camera::get_transform()
{
    return m_transform;
}
