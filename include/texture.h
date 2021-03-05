#ifndef MELODY_TEXUTRE_H_
#define MELODY_TEXUTRE_H_

#include "headers.h"
#include "shader.h"
#include "widgets.h"

#define STB_IMAGE_IMPLEMENTATION    // 使预处理器修改头文件，让其只包含相关的函数定义源码，等于是将这个头文件变为一个 .cpp 文件
#include "stb_image.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <string>
#include <map>

class Texture
{
public:
    GLuint id;
    virtual void bind(GLenum texture);
    virtual ~Texture();
};

void Texture::bind(GLenum texture)
{
    // 绑定到指定纹理单元
    glActiveTexture(texture);
    glBindTexture(GL_TEXTURE_2D, id);
}

Texture::~Texture()
{
    std::cout << "[Texture::~Texture]Delete Texture" << std::endl;
    glDeleteTextures(1, &id);
}


class TextureImage : public Texture
{
private:
    // GLenum img_type;
public:
    TextureImage(const char * path, GLenum img_type);
};

TextureImage::TextureImage(const char * path, GLenum img_type)
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
        std::cout << "[TextureImage::TextureImage]Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

class TextureText : public Texture
{
private:
    std::string m_str;
    FT_Library m_ft;
    FT_Face m_face;
    struct m_Size{ unsigned width; unsigned height; };

    struct Character {
        GLuint     TextureID;  // 字形纹理的ID
        glm::ivec2 Size;       // 字形大小
        glm::ivec2 Bearing;    // 从基准线到字形左部/顶部的偏移值
        long     Advance;    // 原点距下一个字形原点的距离
    };
    // For preloading `void proload()`
    std::map<GLchar, Character> Characters;

public:
    TextureText(const char * font_path, m_Size size = {0, 48}, const char *str = NULL);
    ~TextureText();

    void proload();
    void setString(const char *str);
    // void render(Shader &s, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
};

TextureText::TextureText(const char * font_path, m_Size size, const char * str)
{
    m_str = str;

    if (FT_Init_FreeType(&m_ft))
        std::cout << "[TextureText::TextureText]ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    if (FT_New_Face(m_ft, font_path, 0, &m_face))
        std::cout << "[TextureText::TextureText]ERROR::FREETYPE: Failed to load font" << std::endl;

    FT_Set_Pixel_Sizes(m_face, size.width, size.height);

    if(str != NULL)
    {
        long len = utfstrlen(str);
        GLuint *textures = (GLuint *)malloc(sizeof(char) * len);
        glGenTextures(utfstrlen(str), textures);

        char const *c;
        for(c = str; *c != '\0'; ++c)
        {
            // TODO liu.qihuan@outlook.com 03-04-2021
            FT_Load_Char(m_face, *c, FT_LOAD_RENDER);
        }
    }
}

TextureText::~TextureText()
{
    FT_Done_Face(m_face);
    FT_Done_FreeType(m_ft);
}

void TextureText::proload()
{
    for (GLubyte c = 0; c < 128; c++)
    {
        // 加载字符的字形
        if (FT_Load_Char(m_face, c, FT_LOAD_RENDER))
        {
            std::cout << "[TextureText::preload]ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // 生成纹理
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            m_face->glyph->bitmap.width,
            m_face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            m_face->glyph->bitmap.buffer
        );
        // 设置纹理选项
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // 储存字符供之后使用
        Character character = {
            texture,
            glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
            glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
            m_face->glyph->advance.x
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }
}

void TextureText::setString(const char *str)
{
    m_str = str;
}

// void TextureText::render(Shader &s, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
// {
//     // 激活对应的渲染状态
//     s.use();
//     glUniform3f(glGetUniformLocation(s.ID, "textColor"), color.x, color.y, color.z);
//     glActiveTexture(GL_TEXTURE0);
//     glBindVertexArray(VAO);
//
//     // 遍历文本中所有的字符
//     std::string::const_iterator c;
//     for (c = m_str.begin(); c != m_str.end(); c++)
//     {
//         Character ch = Characters[*c];
//
//         GLfloat xpos = x + ch.Bearing.x * scale;
//         GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
//
//         GLfloat w = ch.Size.x * scale;
//         GLfloat h = ch.Size.y * scale;
//         // 对每个字符更新VBO
//         GLfloat vertices[6][4] = {
//             { xpos,     ypos + h,   0.0, 0.0 },
//             { xpos,     ypos,       0.0, 1.0 },
//             { xpos + w, ypos,       1.0, 1.0 },
//
//             { xpos,     ypos + h,   0.0, 0.0 },
//             { xpos + w, ypos,       1.0, 1.0 },
//             { xpos + w, ypos + h,   1.0, 0.0 }
//         };
//         // 在四边形上绘制字形纹理
//         glBindTexture(GL_TEXTURE_2D, ch.TextureID);
//         // 更新VBO内存的内容
//         glBindBuffer(GL_ARRAY_BUFFER, VBO);
//         glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
//         glBindBuffer(GL_ARRAY_BUFFER, 0);
//         // 绘制四边形
//         glDrawArrays(GL_TRIANGLES, 0, 6);
//         // 更新位置到下一个字形的原点，注意单位是1/64像素
//         x += (ch.Advance >> 6) * scale; // 位偏移6个单位来获取单位为像素的值 (2^6 = 64)
//     }
//     glBindVertexArray(0);
//     glBindTexture(GL_TEXTURE_2D, 0);
// }

#endif // MELODY_TEXUTRE_H_
