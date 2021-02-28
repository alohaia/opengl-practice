#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <unistd.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/shader.h"
#include "include/texture.h"
#include "include/camera_old.h"

const float W_WIDTH  = 1920.0f;
const float W_HEIGHT = 1080.0f;

struct BgColor
{
    float r;
    float g;
    float b;
    float a;
};

static BgColor bg = {0.0f, 0.0f, 0.0f, 0.0f};

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
glm::vec3 vmove = glm::vec3(0, 0, 0);
float speed = 3;

void info(int argc, char** argv)
{
    const GLubyte* vendor = glGetString(GL_VENDOR); //返回负责当前OpenGL实现厂商的名字
    const GLubyte* shade_deivce = glGetString(GL_RENDERER); //返回一个渲染器标识符，通常是个硬件平台
    const GLubyte* opengl_version = glGetString(GL_VERSION); //返回当前OpenGL实现的版本号

    printf("OpenGL厂商：\t%s\n", vendor);
    printf("渲染器标识符：\t%s\n", shade_deivce);
    printf("OpenGL实现：\t%s\n", opengl_version);
}


void keyboard(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);         // 使 glfwWindowShouldClose 返回 true

    if(key == GLFW_KEY_W)
        vmove += glm::vec3(0.0f, -0.5f, 0.0f);
    if(key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
        vmove += glm::vec3(0.0f, 0.5f, 0.0f);
    if(key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
        vmove += glm::vec3(-0.5f, 0.0f, 0.0f);
    if(key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
        vmove += glm::vec3(0.5f, 0.0f, 0.0f);
}

static bool firstMouse = true;
static GLfloat lastX=W_WIDTH/2, lastY=W_HEIGHT/2;
void mousecallback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }


    GLfloat xoffset = 0;
    GLfloat yoffset = 0;
    if (xpos <= 10)
        xoffset = xpos - lastX - 10;
    else if (xpos >= W_WIDTH - 10)
        xoffset = xpos - lastX + 10;
    else
        xoffset = xpos - lastX;

    if (ypos <= 10)
        yoffset = lastY - ypos + 10;
    if (ypos >= W_HEIGHT - 10)
        yoffset = lastY - ypos - 10;
    else
        yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scrollcallback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (yoffset == -1)
        vmove += glm::vec3(0.0f, 0.0f, -0.5f);
    if (yoffset == 1)
        vmove += glm::vec3(0.0f, 0.0f, 0.5f);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(int argc, char** argv)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "Window Title", glfwGetPrimaryMonitor(), NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, keyboard);
    glfwSetCursorPosCallback(window, mousecallback);
    glfwSetScrollCallback(window, scrollcallback);
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader myShader("shaders/icon.vs", "shaders/icon.fs");

    unsigned int VBO, VAO;

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };    //}}}
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    Texture2D texture1("/home/aloha/Pictures/Gimp/smail.png", GL_RGBA);
    Texture2D texture2("./textures/container.jpg", GL_RGB);

    texture1.use(GL_TEXTURE0);
    texture2.use(GL_TEXTURE1);

    myShader.use();         // glUseProgram(myShader.ID);
    myShader.setInt("texture1", 0);                                     // 1 : GL_TEXTURE1
    myShader.setInt("texture2", 1);                                     // 1 : GL_TEXTURE1

    info(argc, argv);

    std::cout << "GL_TEXTURE0:\t" << GL_TEXTURE0 << std::endl;
    int t[2] = {-1, -1};
    glGetUniformiv(myShader.ID, glGetUniformLocation(myShader.ID, "texture1"), t);
    std::cout << "texture1:\t" << t[0] << std::endl;

    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(45.0f), W_WIDTH/W_HEIGHT, 0.1f, 100.0f);

    while(!glfwWindowShouldClose(window))
    {

        glClearColor(bg.r, bg.g, bg.b, bg.a);   // 状态设置函数，设置清空屏幕所用的颜色
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);           // 状态使用函数，清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色

        myShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        myShader.setMat4("view", glm::translate(camera.GetViewMatrix(), vmove * 3.0f));

        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f) + glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            myShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwDestroyWindow(window);
    return 0;
}
