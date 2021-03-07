#include "freetype/freetype.h"
#include FT_FREETYPE_H
#include <headers.h>
#include <iostream>

// #include <texture.h>

static int W_WIDTH = 640;
static int W_HEIGHT = 560;

#define XPOS(x) ((x)/W_WIDTH)
#define YPOS(y) ((y)/W_HEIGHT)

glm::vec4 bgColor(0.2f, 0.3f, 0.3f, 1.0f);

// GLFW callback functions {{{
void _glfwKeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods);
void _glfwWindowSizeCallback(GLFWwindow * window, int width, int height);
// GLFW callback functions }}}

int main(int argc, char** argv)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "[main]Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, _glfwKeyCallback);
    glfwSetWindowSizeCallback(window, _glfwWindowSizeCallback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "[main]Failed to load glad" << std::endl;
        return -1;
    }

    glViewport(0, 0, W_WIDTH, W_HEIGHT);

    // glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Load text glyph {{{
    FT_Library ft;
    if(FT_Init_FreeType(&ft))
    {
        std::cout << "[main]Failed to load FreeType library." << std::endl;
    }

    const char * font_path = "/usr/share/fonts/nerd-fonts/JetBrains Mono Regular Nerd Font Complete.ttf";
    FT_Face face;
    if(FT_New_Face(ft, font_path, 0, &face))
    {
        std::cout << "[main]Failed to load face from file \"" << font_path << '"' << std::endl;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);
    if(FT_Load_Char(face, 'L', FT_LOAD_RENDER))
    {
        std::cout << "[main]Failed to load glyph." << std::endl;
    }
    // Load text glyph }}}

    // Texture {{{
    GLuint texText;
    glGenTextures(1, &texText);
    glBindTexture(GL_TEXTURE_2D, texText);

    // 设置纹理选项
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        face->glyph->bitmap.width,
        face->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        face->glyph->bitmap.buffer
    );
    glGenerateMipmap(GL_TEXTURE_2D);

    glActiveTexture(GL_TEXTURE0);

    // Texture }}}

    // Data {{{
    // GLfloat vertices[6][6] =
    // {
    //     {  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f },
    //     {  0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f },
    //     { -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f },
    //
    //     {  0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f },
    //     { -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f },
    //     { -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f },
    // };
    // GLfloat vertices[4][6] =
    // {
    //     {  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f },
    //     {  0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f },
    //     { -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f },
    //     { -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f },
    // };

    unsigned int w = face->glyph->bitmap.width * 100;
    unsigned int h = face->glyph->bitmap.rows * 100;

    GLfloat xpos = 0, ypos = 0;

    // GLfloat vertices[4][4] =
    // {
    //     { (xpos + w)/W_WIDTH, (ypos + h)/W_HEIGHT,   1.0, 0.0 },
    //     { (xpos + w)/W_WIDTH, (ypos    )/W_HEIGHT,   1.0, 1.0 },
    //     { (xpos    )/W_WIDTH, (ypos    )/W_HEIGHT,   0.0, 1.0 },
    //     { (xpos    )/W_WIDTH, (ypos + h)/W_HEIGHT,   0.0, 0.0 },
    // };

    GLuint indices[] =
    {
        1, 2, 3,
        0, 1, 3,
    };

    GLuint VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 16, NULL, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // vCoord
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (GLvoid *)0);
    glEnableVertexAttribArray(0);

    // texCoord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (GLvoid *)(sizeof(GLfloat) * 2));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    // Data }}}

    // Shader {{{
    Shader shader("shaders/test2.vs", "shaders/test2.fs");
    shader.use();
    shader.setVec3("textColor", 0.3, 0.7, 0.9);
    shader.setInt("text", 0);
    // Shader }}}

    glBindVertexArray(VAO);
    while(!glfwWindowShouldClose(window))
    {
        GLfloat x, y, w, h;
        w = face->glyph->bitmap.width;
        h = face->glyph->bitmap.rows;
        x = xpos + face->glyph->bitmap_left;
        y = ypos + face->glyph->bitmap_top - h;
        GLfloat vertices[] = {
            // positions  // texture coords
            XPOS(x + w), YPOS(y + h), 1.0f, 0.0f, // top right
            XPOS(x + w), YPOS(y    ), 1.0f, 1.0f, // bottom right
            XPOS(x    ), YPOS(y    ), 0.0f, 1.0f, // bottom left
            XPOS(x    ), YPOS(y + h), 0.0f, 0.0f  // top left
        };
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        // render
        glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);   // 状态设置函数，设置清空屏幕所用的颜色
        glClear(GL_COLOR_BUFFER_BIT);           // 状态使用函数，清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
        // glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    return 0;
}

// Function difinations {{{
void _glfwKeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS)
    {
        switch(key)
        {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
            case GLFW_KEY_Q:
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
            default:
                break;
        }
    }
}

void _glfwWindowSizeCallback(GLFWwindow* window, int width, int height)
{
    W_WIDTH = width;
    W_HEIGHT = height;
    glViewport(0, 0, width, height);
}
// Function difinations }}}
