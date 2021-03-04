#ifndef MELODY_TEXT_H_
#define MELODY_TEXT_H_

#include "headers.h"
#include "object.h"
#include <string>

class Text : public Object
{
protected:
    std::string m_str;
    float m_size;
public:
    Text(glm::vec3 pos = glm::vec3(0), const char * str = "Text", float size = 12);
    ~Text() {}
};

Text::Text(glm::vec3 pos, const char * str, float size)
    : Object(pos)
{
    m_str = str;
    m_size = 12;
}

#endif // MELODY_TEXT_H_
