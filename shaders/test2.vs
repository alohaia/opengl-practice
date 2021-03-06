#version 460 core
layout (location = 0) in vec2 vCoord;
layout (location = 1) in vec2 texCoord;
out vec2 TexCoords;

// uniform mat4 projection;

void main()
{
    gl_Position = vec4(vCoord, 0.0, 1.0);
    TexCoords = texCoord;
}
