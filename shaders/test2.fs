#version 460 core

in vec2 TexCoords;
out vec4 FragColor;

// uniform sampler2D image;
uniform sampler2D text;
uniform vec3 textColor;

void main()
{
    // FragColor = mix(texture(image, TexCoords), vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r), 0.5);

    // FragColor = vec4(1.0, 1.0, 1.0, 1.0) * vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);

    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    FragColor = vec4(textColor, 1.0) * sampled;
}
