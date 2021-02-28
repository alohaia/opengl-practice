#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;		// GL_TEXTURE0
uniform sampler2D texture2;		// GL_TEXTURE1

void main()
{
	// FragColor = texture(texture1, TexCoord);
    // 混合纹理和颜色
	// FragColor = texture(texture1, TexCoord) * vec4(ourColor, 1.0);
    // 混合两个纹理后再与颜色混合
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2) * vec4(ourColor, 1.0);
}
