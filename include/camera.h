#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#define DEFAULT_CAMERA_SPEED 10.0f

struct Camera{

    Camera(glm::vec3 position,
           glm::vec3 up,
           glm::vec3 front,
           float speed = DEFAULT_CAMERA_SPEED);

    glm::mat4 view_matrix() const;
    void keypress_update(float delta);
    void mousemove_update(float x_offset, float y_offset);

    glm::vec3 m_position;
    glm::vec3 m_up;
    glm::vec3 m_front;

    float m_speed;
    float m_yaw;
    float m_pitch;
    float m_sens;
};
