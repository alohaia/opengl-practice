#ifndef MELODY_LIGHT_H_
#define MELODY_LIGHT_H_

#include "headers.h"
#include "object.h"

class Light : public Object
{
protected:
    glm::vec3 color = glm::vec3(1);
public:
    Light(glm::vec3 pos) : Object(pos) {}
    ~Light() {}
};

#endif // MELODY_LIGHT_H_
