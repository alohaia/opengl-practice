#version 460 core
layout (location = 0) in vec3 vertex;
out vec2 TexCoord;

void main()
{
    gl_Position = projection * vec4(aPos, 1.0);
    TexCoord = vertex.zw;
}
