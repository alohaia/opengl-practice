#ifndef MELODY_TEXUTRE_H_
#define MELODY_TEXUTRE_H_

#include "headers.h"

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
        std::cout << "Failed to load texture" << std::endl;
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
    TextureText(const char * str, const char * font_path, m_Size size = {0, 48});
    ~TextureText();
    void proload();
};

TextureText::TextureText(const char * str, const char * font_path, m_Size size)
{
    m_str = str;

    if (FT_Init_FreeType(&m_ft))
        std::cout << "[TextureText::TextureText]ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    if (FT_New_Face(m_ft, "fonts/arial.ttf", 0, &m_face))
        std::cout << "[TextureText::TextureText]ERROR::FREETYPE: Failed to load font" << std::endl;

    FT_Set_Pixel_Sizes(m_face, size.width, size.height);
}

TextureText::~TextureText()
{
    FT_Done_Face(m_face);
    FT_Done_FreeType(m_ft);
}

void TextureText::proload()
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //禁用字节对齐限制
    for (GLubyte c = 0; c < 128; c++)
    {
        // 加载字符的字形
        if (FT_Load_Char(m_face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
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

#endif // MELODY_TEXUTRE_H_
