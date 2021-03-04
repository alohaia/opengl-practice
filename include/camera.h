#ifndef CAMERA_H
#define CAMERA_H

#include "headers.h"
#include "object.h"

#include <vector>
#include <iostream>

enum class Zoom {
    IN,
    OUT
};

void echoVec3(glm::vec3 v3)
{
    std::cout << "(" << v3.x << ", " << v3.y << ", " << v3.z << ")" << std::endl;
}

class Camera : public Object
{
protected:
    float fov = FOV;

public:
    Camera(glm::vec3 pos) : Object(pos) {}
    // ~Camera();
    void move(Direction direction, float offset);
    void zoom(Zoom zoom, float degree);
    void rotate(float cursorXOffset, float cursorYOffset);

    glm::mat4 getViewMatrix();

    float getFov() { return fov; }
};

void Camera::move(Direction direction, float offset)
{
    static int count = 0;

    switch(direction)
    {
        case Direction::UP:
            m_position += m_up * offset;
            break;
        case Direction::DOWN:
            m_position -= m_up * offset;
            break;
        case Direction::RIGHT:
            m_position += m_right * offset;
            break;
        case Direction::LEFT:
            m_position -= m_right * offset;
            break;
        case Direction::FORWARD:
            m_position += m_front * offset;
            break;
        case Direction::BACKWARD:
            m_position -= m_front * offset;
            break;
        default:
            std::cout << "[Camera::move]Undefined direction" << std::endl;
            break;
    }

    ++count;
    if(count == 3)
    {
        count = 0;
        std::cout << "position:\t" << "(" << m_position.x << ", " << m_position.y << ", " << m_position.z << ")" << std::endl;
    }
}


// caculate cursor offsets to rotate
void Camera::rotate(float cursorXOffset, float cursorYOffset)
{
    // initialization of presisted variables
    static float pitch = PITCH;
    static float yaw   = YAW;
    // static float roll  = 0.0f;

    static int count = 0;

    // change angles
    pitch += cursorYOffset;
    yaw   += cursorXOffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    // set front
    m_front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    m_front.y = sin(glm::radians(pitch));
    m_front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    m_front = glm::normalize(m_front);

    // update other vectors
    m_right = glm::normalize(glm::cross(m_front, WORLDUP));
    m_up    = glm::normalize(glm::cross(m_right, m_front));

    ++count;
    if (count == 10)
    {
        count = 0;
        std::cout << "Pitch:\t" << pitch << std::endl;
        std::cout << "Yaw:\t" << yaw << std::endl;
        std::cout << "(" << m_front.x << ", " << m_front.y << ", " << m_front.z << ")" << std::endl;
    }
}

glm::mat4 Camera::getViewMatrix()
{
    // echoVec3(front);
    // echoVec3(right);
    // echoVec3(up);
    //
    // glm::mat4 lookat = glm::lookAt(position, position + front, WORLDUP);
    // for (int i = 0; i < 4; ++i)
    // {
    //     for (int j = 0; j < 4; ++j)
    //     {
    //         std::cout << lookat[i][j] << '\t';
    //     }
    //     std::cout << "\n";
    // }
    // return lookat;
    return glm::lookAt(m_position, m_position + m_front, WORLDUP);
}

void Camera::zoom(Zoom zoom, float degree)
{
    switch (zoom)
    {
        case Zoom::IN:
            fov -= degree;
            break;
        case Zoom::OUT:
            fov += degree;
            break;
        default: break;
    }
    if(fov < 1.0f)
        fov = 1.0f;
    if(fov > FOV)
        fov = FOV;
    std::cout << "Field of View:\t" << fov << std::endl;
}

#endif
