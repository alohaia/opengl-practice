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

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "[main]Failed to load glad" << std::endl;
        return -1;
    }

    glViewport(0, 0, W_WIDTH, W_HEIGHT);

    // glEnable(GL_CULL_FACE);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetKeyCallback(window, _glfwKeyCallback);

    // GLfloat xpos = 100, ypos = 100;
    //
    // GLfloat vertices[4][4] =
    // {
    //     { (xpos + w)/W_WIDTH, (ypos    )/W_HEIGHT,   1.0, 1.0 },
    //     { (xpos + w)/W_WIDTH, (ypos + h)/W_HEIGHT,   1.0, 0.0 },
    //     { (xpos    )/W_WIDTH, (ypos + h)/W_HEIGHT,   0.0, 0.0 },
    //     { (xpos    )/W_WIDTH, (ypos    )/W_HEIGHT,   0.0, 1.0 },
    // };

    // GLuint indices[2][3] =
    // {
    //     { 1, 2, 3 },
    //     { 0, 1, 3 },
    // };

    float vertices[] = {
        // positions  // texture coords
         0.5f,  0.5f, 1.0f, 1.0f, // top right
         0.5f, -0.5f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
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


    // Position
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    // Data }}}

    // Texture {{{
    TextureImage texImg("./textures/awesomeface.png", GL_RGBA);
    texImg.bind(GL_TEXTURE0);
    // };

    // Shader {{{
    Shader shader("shaders/test3.vs", "shaders/test3.fs");
    shader.use();
    // shader.setVec3("textColor", 0.3, 0.7, 0.9);
    glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);
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
