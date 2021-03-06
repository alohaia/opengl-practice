/** #include <ft2build.h> */
// reference: https://learnopengl-cn.github.io/06%20In%20Practice/02%20Text%20Rendering/

// GLEW
#include "include/headers.h"

#include "include/font.h"
#include <shader.h>
#include <texture.h>

#include <iostream>
#include <map>

GLuint VAO, VBO;
// Properties
const GLuint WIDTH = 800, HEIGHT = 600;

int main(int argc, char** argv)
{
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL); // Windowed
    glfwMakeContextCurrent(window);

    // glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "[main]Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    // Set OpenGL options
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // OpenGL 默认要求所有的纹理都是4字节对齐的，即纹理的大小永远是4字节的倍数。
    // 通过将纹理解压对齐参数设为1，确保不会有对齐问题
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    Font font("/usr/share/fonts/nerd-fonts/JetBrains Mono Regular Nerd Font Complete Mono.ttf");
    font.setSize(0, 48);

    // Load glyph {{{
    char c = 'T';
    if (FT_Load_Char(font.face, c, FT_LOAD_RENDER))
    {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
    }
    // 生成纹理
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        font.face->glyph->bitmap.width,
        font.face->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        font.face->glyph->bitmap.buffer
    );
    // 设置纹理选项
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load glyph }}}

    // Compile and setup the shader
    Shader shader("shaders/character.vs", "shaders/character.fs");
    // 正射投影矩阵
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(WIDTH), 0.0f, static_cast<GLfloat>(HEIGHT));
    shader.use();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // NULL
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Render {{{
    glm::vec3 color(0.3, 0.7f, 0.9f);
    shader.use();
    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.r, color.g, color.b);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    GLfloat x = 100, y = 100;
    glm::vec2 Size = glm::ivec2(font.face->glyph->bitmap.width, font.face->glyph->bitmap.rows);
    glm::vec2 Bearing = glm::ivec2(font.face->glyph->bitmap_left, font.face->glyph->bitmap_top);

    GLfloat scale = 1.0f;

    GLfloat xpos = x + Bearing.x * scale;
    GLfloat ypos = y - (Size.y - Bearing.y) * scale;

        // texture,
        // glm::ivec2(font.face->glyph->bitmap.width, font.face->glyph->bitmap.rows),
        // glm::ivec2(font.face->glyph->bitmap_left, font.face->glyph->bitmap_top),
        // font.face->glyph->advance.x
    GLfloat w = Size.x * scale;
    GLfloat h = Size.y * scale;
    // 对每个字符更新VBO
    GLfloat vertices[6][4] = {
        { xpos,     ypos + h,   0.0, 0.0 },
        { xpos,     ypos,       0.0, 1.0 },
        { xpos + w, ypos,       1.0, 1.0 },

        { xpos,     ypos + h,   0.0, 0.0 },
        { xpos + w, ypos,       1.0, 1.0 },
        { xpos + w, ypos + h,   1.0, 0.0 }
    };
    std::cout << "xpos: " << xpos << std::endl;
    // 在四边形上绘制字形纹理
    glBindTexture(GL_TEXTURE_2D, texture);
    // 更新VBO内存的内容
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // 绘制四边形
    // 更新位置到下一个字形的原点，注意单位是1/64像素
    x += (font.face->glyph->advance.x >> 6) * scale; // 位偏移6个单位来获取单位为像素的值 (2^6 = 64)

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    // Render }}}

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check and call events
        glfwPollEvents();

        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
