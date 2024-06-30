#include "transform.h"

Transform::Transform() :
m_matrix(1.0f),
m_inverse_matrix(1.0f),
m_recalculate_matrix(false),
m_recalculate_inverse_matrix(false),
m_origin(0.0f),
m_position(0.0f),
m_orientation(1.0f, 0.0f, 0.0f, 0.0f),
m_scale(1.0f)
{
}

void Transform::reset()
{
    m_matrix = glm::mat4(1.0f);
    m_inverse_matrix = glm::mat4(1.0f);
    m_recalculate_matrix = false;
    m_recalculate_inverse_matrix = false;
    m_origin = glm::vec3(0.0f);
    m_position = glm::vec3(0.0f);
    m_orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    m_scale = glm::vec3(1.0f);
}

void Transform::set_origin(const glm::vec3& origin)
{
    m_origin = origin;
    m_recalculate_matrix = m_recalculate_inverse_matrix = true;
}

void Transform::set_position(const glm::vec3& position)
{
    m_position = position;
    m_recalculate_matrix = m_recalculate_inverse_matrix = true;
}

void Transform::set_orientation(float angle, const glm::vec3& axis)
{
    m_orientation = glm::angleAxis(glm::radians(angle), axis);
    m_recalculate_matrix = m_recalculate_inverse_matrix = true;
}

void Transform::set_scale(const glm::vec3& scale)
{
    m_scale = scale;
    m_recalculate_matrix = m_recalculate_inverse_matrix = true;
}

void Transform::translate(const glm::vec3& offset)
{
    m_position += offset;
    m_recalculate_matrix = m_recalculate_inverse_matrix = true;
}

void Transform::rotate(float angle, const glm::vec3& axis)
{
    m_orientation = glm::angleAxis(glm::radians(angle), axis) * m_orientation;
    m_recalculate_matrix = m_recalculate_inverse_matrix = true;
}

void Transform::scale(const glm::vec3& scale)
{
    m_scale *= scale;
    m_recalculate_matrix = m_recalculate_inverse_matrix = true;
}

const glm::vec3& Transform::get_origin() const
{
    return m_origin;
}

const glm::vec3& Transform::get_position() const
{
    return m_position;
}

const glm::quat& Transform::get_orientation() const
{
    return m_orientation;
}

const glm::vec3& Transform::get_scale() const
{
    return m_scale;
}

glm::vec3 Transform::right() const
{
    return m_orientation * glm::vec3(1.0f, 0.0f, 0.0f);
}

glm::vec3 Transform::up() const
{
    return m_orientation * glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 Transform::forward() const
{
    return m_orientation * glm::vec3(0.0f, 0.0f, -1.0f);
}

const glm::mat4& Transform::get_matrix() const
{
    if (m_recalculate_matrix) {
        m_matrix = glm::mat4(1.0f);
        m_matrix = glm::translate(m_matrix, m_position);
        m_matrix = glm::rotate(m_matrix, glm::angle(m_orientation), glm::axis(m_orientation));
        m_matrix = glm::translate(m_matrix, -m_origin);
        m_matrix = glm::scale(m_matrix, m_scale);
        m_recalculate_matrix = false;
    }

    return m_matrix;
}

const glm::mat4& Transform::get_inverse_matrix() const
{
    if (m_recalculate_inverse_matrix) {
        m_inverse_matrix = glm::mat4(1.0f);
        m_inverse_matrix = glm::scale(m_inverse_matrix, 1.0f / m_scale);
        m_inverse_matrix = glm::translate(m_inverse_matrix, m_origin);
        m_inverse_matrix = glm::rotate(m_inverse_matrix, -glm::angle(m_orientation), glm::axis(m_orientation));
        m_inverse_matrix = glm::translate(m_inverse_matrix, -m_position);
        m_recalculate_inverse_matrix = false;
    }

    return m_inverse_matrix;
}