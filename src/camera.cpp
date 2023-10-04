#include "camera.h"
#include <glm/ext/matrix_transform.hpp>
#include "input.h"

Camera::Camera(glm::vec3 position,
           glm::vec3 up,
           glm::vec3 front,
           float speed):
    m_position(position),
    m_up(up),
    m_front(front),
    m_speed(speed),
    m_yaw(-90.0f),
    m_pitch(0.0f),
    m_sens(0.1f)
{

};

//Updates the position based on keyboard input
void Camera::keypress_update(float delta_time){
    if(Input::is_key_pressed(Input::Key::D))
        m_position -= glm::normalize(glm::cross(m_up, m_front)) * m_speed * delta_time;

    if(Input::is_key_pressed(Input::Key::A))
        m_position += glm::normalize(glm::cross(m_up, m_front)) * m_speed * delta_time;

    if(Input::is_key_pressed(Input::Key::W))
        m_position += m_front * m_speed * delta_time;

    if(Input::is_key_pressed(Input::Key::S))
        m_position -= m_front * m_speed * delta_time;
};

//Updates the front direction vector based on mouse input
void Camera::mousemove_update(float x_offset, float y_offset){
    float yaw_offset = x_offset * m_sens;
    float pitch_offset = y_offset * m_sens; 

    m_yaw += yaw_offset;
    m_pitch += pitch_offset;

    if(m_pitch > 89.0f) m_pitch = 89.0f;
    if(m_pitch < -89.0f) m_pitch = -89.0f;

    glm::vec3 new_direction;

    new_direction.x = std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
    new_direction.y = std::sin(glm::radians(m_pitch));
    new_direction.z = std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));

    m_front = glm::normalize(new_direction);
};

glm::mat4 Camera::view_matrix() const{ 
    glm::vec3 target = m_position + m_front;
    return glm::lookAt(m_position, target, m_up); 
};
