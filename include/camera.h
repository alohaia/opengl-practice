#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>

void echoVec3(glm::vec3 v3)
{
    std::cout << "(" << v3.x << ", " << v3.y << ", " << v3.z << ")" << std::endl;
}

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Direction {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

enum Zoom {
    IN,
    OUT
};

// Default camera values
const float PITCH = 0.0f;
const float YAW   = -90.0f;
const float FOV   = 45.0f;

const glm::vec3 WORLDUP = glm::vec3(0.0f, 1.0f, 0.0f);

class Camera
{
private:
    glm::vec3 position  = glm::vec3(0.0f);
    glm::vec3 front     = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 right     = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 up        = glm::vec3(0.0f, 1.0f, 0.0f);

    float fov = FOV;

public:
    Camera(glm::vec3);
    // ~Camera();
    void moveto(glm::vec3);
    void moveby(glm::vec3);
    void move(Direction direction, float offset);
    void move(glm::vec3);
    void rotate(float cursorXOffset, float cursorYOffset);
    void zoom(Zoom zoom, float degree);

    float getFov() { return fov; }
    glm::mat4 getViewMatrix();
};

Camera::Camera(glm::vec3 pos)
{
    position = pos;
}

void Camera::moveto(glm::vec3 to)
{
    position = to;
}

void Camera::moveby(glm::vec3 by)
{
    position += by;
}

void Camera::move(Direction direction, float offset)
{
    static int count = 0;

    switch(direction)
    {
        case UP:
            position += up * offset;
            break;
        case DOWN:
            position -= up * offset;
            break;
        case RIGHT:
            position += right * offset;
            break;
        case LEFT:
            position -= right * offset;
            break;
        case FORWARD:
            position += front * offset;
            break;
        case BACKWARD:
            position -= front * offset;
            break;
        default:
            std::cout << "[Camera::move]Undefined direction" << std::endl;
            break;
    }

    ++count;
    if(count == 3)
    {
        count = 0;
        std::cout << "position:\t" << "(" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
    }
}

void Camera::move(glm::vec3 offset)
{
    position += offset;
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
    if (pitch > 90.0f)
        pitch = 90.0f;
    if (pitch < -90.0f)
        pitch = -90.0f;

    // set front
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    front = glm::normalize(front);

    // update other vectors
    right = glm::normalize(glm::cross(front, WORLDUP));
    up    = glm::normalize(glm::cross(right, front));

    ++count;
    if (count == 10)
    {
        count = 0;
        std::cout << "Pitch:\t" << pitch << std::endl;
        std::cout << "Yaw:\t" << yaw << std::endl;
        std::cout << "(" << front.x << ", " << front.y << ", " << front.z << ")" << std::endl;
    }
}

void Camera::zoom(Zoom zoom, float degree)
{
    switch (zoom)
    {
        case IN:
            fov -= degree;
            break;
        case OUT:
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
    return glm::lookAt(position, position + front, WORLDUP);
}

#endif
