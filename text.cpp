// Headers {{{
#include <headers.h>
#include <shader.h>
#include <ft2build.h>
#include FT_FREETYPE_H
// #include <texture.h>
// Headers }}}

enum{
    W_WIDTH = 640,
    W_HEIGHT = 540
};

glm::vec4 bgColor(0.2f, 0.3f, 0.3f, 1.0f);

// GLFW callback functions {{{
void _glfwKeyFunc(GLFWwindow * window, int key, int scancode, int action, int mods);
void _glfwWindowSizeFunc(GLFWwindow * window, int width, int height);
// GLFW callback functions }}}

int main(int argc, char** argv)
{
    // Initialization {{{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "[main]Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))         // 须先调用 glfwMakeContextCurrent
    {
        std::cout << "[main]Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, W_WIDTH, W_HEIGHT);
    // Initialization }}}
    // glViewport(0, 0, 560, 480);

    // Special settings {{{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //禁用字节对齐限制
    // glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Special settings }}}

    // Glfw callback functions {{{
    glfwSetKeyCallback(window, _glfwKeyFunc);
    // Glfw callback functions }}}

    // Prepare original objects {{{
    GLuint indices[2][3] =
    {
        {0, 1, 3},
        {1, 2, 3}
    };

    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Stored in VAO {{{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6 * 4, (void*)0);
    glEnableVertexAttribArray(0);
    // Stored in VAO }}}

    glad_glBindVertexArray(0);
    // Prepare original objects }}}

    // Add text {{{
    Shader shader_text("shaders/text.vs", "shaders/text.fs");

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    FT_Face face;
    if(FT_New_Face(ft, "/usr/share/fonts/nerd-fonts/JetBrains Mono Regular Nerd Font Complete.ttf", 0, &face));
         std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    FT_Set_Pixel_Sizes(face, 0, 48);

    FT_Load_Char(face, 'H', FT_LOAD_RENDER);

    GLuint texture_H;
    glGenTextures(1, &texture_H);
    glBindTexture(GL_TEXTURE_2D, texture_H);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D
    (
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

    shader_text.use();
    shader_text.setVec3("textColor", 1, 1, 1);
    glActiveTexture(GL_TEXTURE0);

    GLfloat xpos = 0, ypos = 0;
    GLfloat h = face->glyph->bitmap.rows;
    GLfloat w = face->glyph->bitmap.width;

    GLfloat vertices[6][4] = {
        { xpos + w, ypos + h,   1.0, 0.0 },
        { xpos + w, ypos,       1.0, 1.0 },
        { xpos,     ypos,       0.0, 1.0 },
        { xpos,     ypos + h,   0.0, 0.0 },
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    // Add text }}}


    // shader.setVec4("bgColor", bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    // shader.setVec4("textColor", 1, 1, 1, 1);

    // Render loop {{{
    while(!glfwWindowShouldClose(window))
    {
        // Render {{{
        glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);   // 状态设置函数，设置清空屏幕所用的颜色
        glClear(GL_COLOR_BUFFER_BIT);           // 状态使用函数，清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色
        // render text
        shader_text.use();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // Render }}}

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // Render loop }}}

    // Optional
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    return 0;
}

// Function difinations {{{

void _glfwKeyFunc(GLFWwindow * window, int key, int scancode, int action, int mods)
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
        }
    }
}
// Function difinations }}}
