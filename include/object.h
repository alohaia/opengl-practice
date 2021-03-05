#ifndef OBJECT_H_
#define OBJECT_H_

#include "headers.h"
#include <iostream>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum class Direction {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Default camera values
const float PITCH = 0.0f;
const float YAW   = -90.0f;
const float FOV   = 45.0f;

const glm::vec3 WORLDUP = glm::vec3(0.0f, 1.0f, 0.0f);

struct Rotation
{
    glm::vec3 axis;
    float angle;
};

// Basic virtual class of all objects
class Object
{
protected:
    glm::vec3 m_position = glm::vec3(0);
    glm::vec3 m_front    = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_right    = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 m_up       = glm::vec3(0.0f, 1.0f, 0.0f);
    // Rotation m_rotation  = {glm::vec3(1, 0, 0), 0};
    float m_scale        = 1;
public:
    Object(glm::vec3 pos = glm::vec3(0, 0, 0));
    virtual ~Object(){}

    void moveto(glm::vec3);
    void moveby(glm::vec3);
    virtual void rotate(Rotation) { /* TODO */ }
    virtual void rotate(glm::vec3 axis, float angle) { /* TODO */ }
};

Object::Object(glm::vec3 pos)
{
    m_position = pos;
}

void Object::moveto(glm::vec3 to)
{
    m_position = to;
}

void Object::moveby(glm::vec3 by)
{
    m_position += by;
}

#endif  // OBJECT_H_
