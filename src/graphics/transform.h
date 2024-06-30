#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Transform {
public:
    Transform();

    void reset();

    void set_origin(const glm::vec3& pivot);
    void set_position(const glm::vec3& position);
    void set_orientation(float angle, const glm::vec3& axis);
    void set_scale(const glm::vec3& scale);

    void translate(const glm::vec3& offset);
    void rotate(float angle, const glm::vec3& axis);
    void scale(const glm::vec3& scale);

    const glm::vec3& get_origin() const;
    const glm::vec3& get_position() const;
    const glm::quat& get_orientation() const;
    const glm::vec3& get_scale() const;

    glm::vec3 right() const;
    glm::vec3 up() const;
    glm::vec3 forward() const;
    const glm::mat4& get_matrix() const;
    const glm::mat4& get_inverse_matrix() const;
private:
    mutable glm::mat4 m_matrix;
    mutable glm::mat4 m_inverse_matrix;

    glm::vec3 m_origin;
    glm::vec3 m_position;
    glm::quat m_orientation;
    glm::vec3 m_scale;

    mutable bool m_recalculate_matrix;
    mutable bool m_recalculate_inverse_matrix;
};
