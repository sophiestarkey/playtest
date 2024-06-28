#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Transform {
public:
    Transform();

    void reset();

    void set_pivot(const glm::vec3& pivot);
    void set_position(const glm::vec3& position);
    void set_orientation(float angle, const glm::vec3& axis);
    void set_scale(const glm::vec3& scale);

    void translate(const glm::vec3& offset);
    void rotate(float angle, const glm::vec3& axis);
    void scale(const glm::vec3& scale);

    glm::vec3 right() const;
    glm::vec3 up() const;
    glm::vec3 forward() const;
    const glm::mat4& get_matrix() const;
private:
    mutable glm::mat4 m_matrix;
    mutable bool m_recalculate;

    glm::vec3 m_pivot;
    glm::vec3 m_position;
    glm::quat m_orientation;
    glm::vec3 m_scale;
};
