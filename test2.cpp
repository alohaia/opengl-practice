#include <headers.h>
#include <iostream>

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

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "[main]Failed to load glad" << std::endl;
        return -1;
    }

    glViewport(0, 0, W_WIDTH, W_HEIGHT);

    glfwSetKeyCallback(window, _glfwKeyCallback);

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
    GLfloat vertices[4][6] =
    {
        {  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f },
        {  0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f },
        { -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f },
        { -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f },
    };

    GLuint indices[2][3] =
    {
        { 0, 1, 3 },
        { 1, 2, 3 },
    };

    GLuint VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid *)0);
    glEnableVertexAttribArray(0);
    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid *)(sizeof(GLfloat)*3));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Shader {{{
    Shader shader("shaders/test2.vs", "shaders/test2.fs");
    shader.use();
    // Shader }}}

    glBindVertexArray(VAO);
    while(!glfwWindowShouldClose(window))
    {
        // render
        glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);   // 状态设置函数，设置清空屏幕所用的颜色
        glClear(GL_COLOR_BUFFER_BIT);           // 状态使用函数，清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色

        // glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

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
