#ifndef SURFACE_H_
#define SURFACE_H_

#include "headers.h"
#include "object.h"

class Rectangular : public Object
{
private:
    GLuint VAO, VBO;
public:
    Rectangular(glm::vec3 pos = glm::vec3(0), float x = 100, float y = 100);
    ~Rectangular();
};

Rectangular::Rectangular(glm::vec3 pos, float x, float y)
    : Object(pos)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

}

#endif // SURFACE_H_
