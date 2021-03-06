#include "freetype/freetype.h"
#include FT_FREETYPE_H
#include <headers.h>
#include <iostream>

#include <texture.h>

static int W_WIDTH = 640;
static int W_HEIGHT = 560;

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

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "[main]Failed to load glad" << std::endl;
        return -1;
    }

    glViewport(0, 0, W_WIDTH, W_HEIGHT);

    // glEnable(GL_CULL_FACE);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
    if(FT_Load_Char(face, 'T', FT_LOAD_RENDER))
    {
        std::cout << "[main]Failed to load glyph." << std::endl;
    }
    // Load text glyph }}}

    // Texture {{{
    // GLuint texText;
    // glGenTextures(1, &texText);
    // glTexImage2D(
    //     GL_TEXTURE_2D,
    //     0,
    //     GL_RED,
    //     face->glyph->bitmap.width,
    //     face->glyph->bitmap.rows,
    //     0,
    //     GL_RED,
    //     GL_UNSIGNED_BYTE,
    //     face->glyph->bitmap.buffer
    // );
    // // 设置纹理选项
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //
    // // bind
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, texText);
    //
    // GLuint texImg2;
    // glBindTexture(GL_TEXTURE_2D, texImg2);
    // // set the texture wrapping parameters
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // // set texture filtering parameters
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //
    // int width, height, nrChannels;
    // // 让stb_image.h在图像加载时翻转y轴
    // // 因为OpenGL要求y轴0.0坐标是在图片的底部的，但是图片的y轴0.0坐标通常在顶部。
    // stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    //
    // // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    // unsigned char *data = stbi_load("./textures/container.jpg", &width, &height, &nrChannels, 0);
    // if (data)
    // {
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //     glGenerateMipmap(GL_TEXTURE_2D);
    // }
    // else
    // {
    //     std::cout << "[TextureImage::TextureImage]Failed to load texture" << std::endl;
    // }
    // stbi_image_free(data);
    // glActiveTexture(GL_TEXTURE1);
    // glBindTexture(GL_TEXTURE_2D, texImg2);

    TextureImage texImg("./textures/awesomeface.png", GL_RGBA);
    TextureImage texImg2("textures/container.jpg", GL_RGB);

    texImg.bind(GL_TEXTURE0);
    texImg2.bind(GL_TEXTURE1);
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

    GLfloat xpos = 100, ypos = 100;

    // GLfloat vertices[4][4] =
    // {
    //     { (xpos + w)/W_WIDTH, (ypos + h)/W_HEIGHT,   1.0, 0.0 },
    //     { (xpos + w)/W_WIDTH, (ypos    )/W_HEIGHT,   1.0, 1.0 },
    //     { (xpos    )/W_WIDTH, (ypos    )/W_HEIGHT,   0.0, 1.0 },
    //     { (xpos    )/W_WIDTH, (ypos + h)/W_HEIGHT,   0.0, 0.0 },
    // };

    float vertices[] = {
        // positions  // texture coords
         0.5f,  0.5f, 1.0f, 1.0f, // top right
         0.5f, -0.5f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f, 1.0f  // top left
    };

    GLuint indices[2][3] =
    {
        { 1, 2, 3 },
        { 0, 1, 3 },
    };

    GLuint VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // vCoord
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (GLvoid *)0);
    glEnableVertexAttribArray(0);

    // texCoord
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (GLvoid *)( sizeof(GLfloat) * 2 ));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    // Data }}}

    // Shader {{{
    Shader shader("shaders/test2.vs", "shaders/test2.fs");
    shader.use();
    shader.setVec3("textColor", 0.3, 0.7, 0.9);
    shader.setInt("image", 0);
    shader.setInt("text", 1);
    // Shader }}}

    glBindVertexArray(VAO);
    while(!glfwWindowShouldClose(window))
    {
        // render
        glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);   // 状态设置函数，设置清空屏幕所用的颜色
        glClear(GL_COLOR_BUFFER_BIT);           // 状态使用函数，清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
        // glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

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
    glViewport(0, 0, width, height);
}
// Function difinations }}}
