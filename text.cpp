// Headers {{{
#include <headers.h>
#include <shader.h>
#include <texture.h>
// Headers }}}

enum{
    W_WIDTH = 640,
    W_HEIGHT = 540
};

glm::vec4 bgColor(0.2f, 0.3f, 0.3f, 1.0f);

// GLFW callback functions
void _glfwKeyFunc(GLFWwindow * window, int key, int scancode, int action, int mods);
void _glfwWindowSizeFunc(GLFWwindow * window, int width, int height);

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

    // Data {{{
    GLfloat vertices[] = {
        // position
         0.5f,  0.5f, 0.0f,     // 右上角
         0.5f, -0.5f, 0.0f,     // 右下角
        -0.5f, -0.5f, 0.0f,     // 左下角
        -0.5f,  0.5f, 0.0f      // 左上角
    };

    GLuint indices[] = { // 注意索引从0开始!
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };
    // Data }}}

    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), NULL, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    TextureText text(
        "/usr/share/fonts/nerd-fonts/JetBrains Mono Regular Nerd Font Complete Mono.ttf",
        {0, 48},
        "hello"
    );
    text.proload();

    Shader shader("shaders/text.vs", "shaders/text.fs");
    shader.use();

    shader.setVec4("bgColor", bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    shader.setVec4("textColor", 1, 1, 1, 1);

    // Render loop {{{
    while(!glfwWindowShouldClose(window))
    {
        // Render {{{
        glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);   // 状态设置函数，设置清空屏幕所用的颜色
        glClear(GL_COLOR_BUFFER_BIT);           // 状态使用函数，清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // Render }}}

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // Render loop }}}

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    return 0;
}

// Function difinations {{{

void _glfwKeyFunc(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void _glfwWindowSizeFunc(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);
}
// Function difinations }}}
