#version 460 core
uniform vec4 bgColor;
uniform vec4 textColor;
out vec4 FragColor;

void main()
{
    FragColor = textColor;
}
