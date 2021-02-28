#ifndef TEXUTRE_H
#define TEXUTRE_H

#include "headers.h"

#define STB_IMAGE_IMPLEMENTATION    // 使预处理器修改头文件，让其只包含相关的函数定义源码，等于是将这个头文件变为一个 .cpp 文件
#include "stb_image.h"
#include <iostream>

class Texture2D
{
private:
    // GLenum img_type;
public:
    GLuint id;
    Texture2D(const char * path, GLenum img_type);
    void use(GLenum texture);
    ~Texture2D();
};

Texture2D::Texture2D(const char * path, GLenum img_type)
{
    int width, height, nrChannels;
    // 让stb_image.h在图像加载时翻转y轴
    // 因为OpenGL要求y轴0.0坐标是在图片的底部的，但是图片的y轴0.0坐标通常在顶部。
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, img_type, width, height, 0, img_type, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void Texture2D::use(GLenum texture)
{
    // 绑定到指定纹理单元
    glActiveTexture(texture);
    glBindTexture(GL_TEXTURE_2D, id);
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &id);
}

#endif // TEXUTRE_H
