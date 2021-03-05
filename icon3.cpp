#include <headers.h>

#include <iostream>
#include <unistd.h>

#include <lua_read.h>
#include <shader.h>
#include <texture.h>
#include <camera.h>

struct BgColor
{
    float r;
    float g;
    float b;
    float a;
};

static double W_WIDTH;
static double W_HEIGHT;

static BgColor bg = {0.5f, 0.5f, 0.5f, 0.5f};

static Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// timeing
double deltaTime = 0.0f;
double lastFrame = 0.0f;

// display opengl information
void info(int argc, char** argv);

// process input
void processInput(GLFWwindow* window);

// glfw key callback function
void keycallback(GLFWwindow* window, int key, int scancode, int action, int mode);

// glfw mouse(click) callback function
void mousecallback(GLFWwindow* window, double xpos, double ypos);

// glfw mouse(scroll) callback function
void scrollcallback(GLFWwindow* window, double xoffset, double yoffset);

// glfw callback function for size changing
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main(int argc, char** argv)
{
    // load configurations
    lua_State * L = lua::lua_init("./config.lua");
    lua::get_number(L, "wscreen", W_WIDTH);
    lua::get_number(L, "hscreen", W_HEIGHT);
    bool fullscreen;
    lua::get_bool(L, "fullscreen", fullscreen);
    char *title;
    // lua::get_string(L, "window_title", title);
    char *tex_img1, *tex_img2;
    // lua::get_string(L, "texture1", tex_img1);
    // lua::get_string(L, "texture2", tex_img2);
    char *shader_vs, *shader_fs;
    // lua::get_string(L, "shader_vs", shader_vs);
    // lua::get_string(L, "shader_fs", shader_fs);

    // lua::get_string causes unknown error
    title        = const_cast<char*>("LearnOpenGL");
    tex_img1     = const_cast<char*>("./textures/awesomeface.png");
    tex_img2     = const_cast<char*>("./textures/container.jpg");
    shader_vs    = const_cast<char*>("shaders/icon.vs");
    shader_fs    = const_cast<char*>("shaders/icon.fs");

    // glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__


    GLFWwindow* window;
    if (fullscreen)
        window = glfwCreateWindow(W_WIDTH, W_HEIGHT, title, glfwGetPrimaryMonitor(), NULL);
    else
        window = glfwCreateWindow(W_WIDTH, W_HEIGHT, title, NULL, NULL);

    if (window == NULL)
    {
        std::cout << "[main]Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, keycallback);
    glfwSetCursorPosCallback(window, mousecallback);
    glfwSetScrollCallback(window, scrollcallback);

    // glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "[main]Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell GLFW to capture our mouse
    // 如此能避免
    // - 处理窗口边界的问题
    // - 鼠标初次进入的突变问题
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // opengl
    glEnable(GL_DEPTH_TEST);

    Shader myShader(shader_vs, shader_fs);
    myShader.use();         // glUseProgram(myShader.ID);

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

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);

    unsigned int VBO, VAO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // textures
    TextureImage texture1(tex_img1, GL_RGBA);
    // TextureImage texture1("./textures/awesomeface.png", GL_RGBA);
    TextureImage texture2(tex_img2, GL_RGB);
    // use texture in shader
    texture1.bind(GL_TEXTURE0);
    texture2.bind(GL_TEXTURE1);
    myShader.setInt("texture1", 0);   // 0 : GL_TEXTURE0, texture1
    myShader.setInt("texture2", 1);   // 1 : GL_TEXTURE1, texture2

    // opengl information
    info(argc, argv);
    // texture information
    // std::cout << "GL_TEXTURE0:\t" << GL_TEXTURE0 << std::endl;
    // int t[2] = {-1, -1};
    // glGetUniformiv(myShader.ID, glGetUniformLocation(myShader.ID, "texture1"), t);
    // std::cout << "texture1:\t" << t[0] << std::endl;

    while(!glfwWindowShouldClose(window))
    {
        // 1. poll events
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        glfwPollEvents();

        // 2. rander
        glClearColor(bg.r, bg.g, bg.b, bg.a);   // 状态设置函数，设置清空屏幕所用的颜色
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);           // 状态使用函数，清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色

        // projection matrix
        glm::mat4 projection(1.0f);
        projection = glm::perspective(camera.getFov(), float(W_WIDTH/W_HEIGHT), 0.1f, 100.0f);
        myShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.

        myShader.setMat4("view", camera.getViewMatrix());

        // draw boxes
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

        // 3. swap buffers
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwDestroyWindow(window);
    return 0;
}

void info(int argc, char** argv)
{
    const GLubyte* vendor = glGetString(GL_VENDOR); //返回负责当前OpenGL实现厂商的名字
    const GLubyte* shade_deivce = glGetString(GL_RENDERER); //返回一个渲染器标识符，通常是个硬件平台
    const GLubyte* opengl_version = glGetString(GL_VERSION); //返回当前OpenGL实现的版本号

    printf("OpenGL厂商：\t%s\n", vendor);
    printf("渲染器标识符：\t%s\n", shade_deivce);
    printf("OpenGL实现：\t%s\n", opengl_version);
}

void processInput(GLFWwindow* window)
{
    // 用帧间隔时间决定移动距离
    // 两帧间隔长 --> 速度大 --> 移动距离长 --> 实际感觉速度稳定
    int speed_times = 5;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.move(Direction::UP, deltaTime * speed_times);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.move(Direction::DOWN, deltaTime * speed_times);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.move(Direction::FORWARD, deltaTime * speed_times);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.move(Direction::BACKWARD, deltaTime * speed_times);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.move(Direction::LEFT, deltaTime * speed_times);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.move(Direction::RIGHT, deltaTime * speed_times);
}

void keycallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);         // 使 glfwWindowShouldClose 返回 true

    // ! 在 keycallback 函数中，无法简单地利用每帧时间做移动的平滑处理
    // float cameraSpeed = 10 * deltaTime;
    // if (key == GLFW_KEY_UP)
    //     camera.move(UP, cameraSpeed);
    // if (key == GLFW_KEY_DOWN)
    //     camera.move(DOWN, cameraSpeed);
    // if (key == GLFW_KEY_W)
    //     camera.move(FORWARD, cameraSpeed);
    // if (key == GLFW_KEY_S)
    //     camera.move(BACKWARD, cameraSpeed);
    // if (key == GLFW_KEY_A)
    //     camera.move(LEFT, cameraSpeed);
    // if (key == GLFW_KEY_D)
    //     camera.move(RIGHT, cameraSpeed);
}

void mousecallback(GLFWwindow* window, double xpos, double ypos)
{
    static bool firstMouse = true;
    static float lastX = 0;
    static float lastY = 0;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    // float xoffset = xpos - lastX;
    // float yoffset = lastY - ypos;

    float sensitivity = 0.05f;

    // camera.rotate(xoffset * sensitivity, yoffset * sensitivity);
    camera.rotate((xpos - lastX) * sensitivity, (lastY - ypos) * sensitivity);

    lastX = xpos;
    lastY = ypos;
}

void scrollcallback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (yoffset == -1)
        camera.zoom(Zoom::OUT, 0.05);
    if (yoffset == 1)
        camera.zoom(Zoom::IN, 0.05);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
