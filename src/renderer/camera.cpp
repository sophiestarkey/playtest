#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"

Camera::Camera() :
    m_aspect_ratio(1.0f),
    m_field_of_view(45.0f),
    m_near_plane(0.01f),
    m_far_plane(1000.0f),
    m_recalc_proj(true),
    m_recalc_view(true),
    m_proj_mat(),
    m_view_mat(),
    m_position(),
    m_orientation(1.0f, 0.0f, 0.0f, 0.0f)
{
}

void Camera::set_aspect_ratio(float aspect_ratio)
{
    m_aspect_ratio = aspect_ratio;
    m_recalc_proj = true;
}

void Camera::set_field_of_view(float field_of_view)
{
    m_field_of_view = field_of_view;
    m_recalc_proj = true;
}

void Camera::set_near_plane(float near_plane)
{
    m_near_plane = near_plane;
    m_recalc_proj = true;
}

void Camera::set_far_plane(float far_plane)
{
    m_far_plane = far_plane;
    m_recalc_proj = true;
}

void Camera::set_position(const glm::vec3& position)
{
    m_position = position;
    m_recalc_view = true;
}

void Camera::set_orientation(float angle, const glm::vec3& axis)
{
    m_orientation = glm::angleAxis(glm::radians(angle), axis);
    m_recalc_view = true;
}


const glm::mat4& Camera::projection_matrix() const
{
    if (m_recalc_proj) {
        m_proj_mat = glm::perspective(glm::radians(m_field_of_view), m_aspect_ratio, m_near_plane, m_far_plane);
        m_recalc_proj = false;
    }
    
    return m_proj_mat;
}

const glm::mat4& Camera::view_matrix() const
{
    if (m_recalc_view) {
        // TODO not sure why this works, the order of transformations is technically the wrong way around
        // maybe there's something special about quaternions
        m_view_mat = glm::mat4(1.0f);
        m_view_mat = glm::rotate(m_view_mat, -glm::angle(m_orientation), glm::axis(m_orientation));
        m_view_mat = glm::translate(m_view_mat, -m_position);
        m_recalc_view = false;
    }

    return m_view_mat;
}
