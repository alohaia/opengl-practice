#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <unistd.h>

// 2: 你好，三角形

//{{{2
// 顶点着色器源码
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
// 片段着色器源码
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";
//2}}}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);         // 使 glfwWindowShouldClose 返回 true
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // 创建 glfw 窗口对象
    GLFWwindow* window = glfwCreateWindow(560, 480, "Window Title", NULL, NULL);
    // full screen
    // GLFWwindow* window = glfwCreateWindow(1920, 1080, "Window Title", glfwGetPrimaryMonitor(), NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 将 window 的 OpenGL or OpenGL ES context 设置为当前进程正在使用的 OpenGL or OpenGL ES context
    // 一个进程只能同时使用一个 context，一个 context 在一个时刻也只能由一个进程使用
    glfwMakeContextCurrent(window);
    // 初始化 glad (要在调用 glfwMakeContextCurrent 后进行)
    // 使用 glfw 提供的 glfwGetProcAddress 来加载系统相关的 OpenGL 函数指针地址
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /******************/
    /* 以上为准备工作 */
    /******************/

    // 设置 OpenGL 视口大小
    glViewport(0, 0, 560, 480);

    // sleep(5);
    // glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, 1920, 1080, 60);
    // sleep(5);


    //{{{2 Shaers
    int  success;
    char infoLog[512];
    // 创建顶点着色器（GL_VERTEX_SHADER）
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // 为着色器对象添加源码：着色器对象，源码字符串数量，源码字符串指针
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // 编译着色器
    glCompileShader(vertexShader);
    // 错误检查
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 片段着色器
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // 错误检查
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 着色器程序
    unsigned int shaderProgram;
    // 创建着色器程序
    shaderProgram = glCreateProgram();
    // 将着色器附加到着色器程序 shaderProgram
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // 链接
    glLinkProgram(shaderProgram);
    // 检查链接错误
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // 删除着色器对象（链接之后就不再需要了）
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 2. 当我们渲染一个物体时要使用着色器程序
    // 设置用于后续绘制的着色器对象
    glUseProgram(shaderProgram);

    //2}}}

    //{{{2 顶点数据
    unsigned int VBO, VAO, EBO;
    // float vertices[] = {
    //     // 第一个三角形
    //     0.5f, 0.5f, 0.0f,   // 右上角
    //     0.5f, -0.5f, 0.0f,  // 右下角
    //     -0.5f, 0.5f, 0.0f,  // 左上角
    //     // 第二个三角形
    //     0.5f, -0.5f, 0.0f,  // 右下角
    //     -0.5f, -0.5f, 0.0f, // 左下角
    //     -0.5f, 0.5f, 0.0f   // 左上角
    // };
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    // 生成一个或多个 顶点数组对象（Vertex Array Object)
    glGenVertexArrays(1, &VAO);
    // 绑定一个顶点数组对象 VAO
    // OpenGL 核心（core）模式要求使用 顶点数组对象，若绑定失败，OpenGL 会拒绝绘制任何东西
    // 解绑：glBindVertexArray(0);
    glBindVertexArray(VAO);
    // 一个顶点数组对象会储存以下这些内容：
    // https://learnopengl-cn.github.io/img/01/04/vertex_array_objects.png
    // 1. glEnableVertexAttribArray 和 glDisableVertexAttribArray 的调用。
    // 2. 通过 glVertexAttribPointer 设置的顶点属性配置。
    // 3. 通过 glVertexAttribPointer 调用与顶点属性关联的顶点缓冲对象。
    // 4. 目标为 GL_ELEMENT_ARRAY_BUFFER 时，glBindBuffer 的函数调用（绑定和解绑），
    //    即 IBO/EBO 的绑定状态

    // 0. 复制顶点数组到缓冲中供OpenGL使用
    // 生成一个或多个 缓冲对象（Buffer Object），如 顶点缓冲对象（VBO）、索引缓冲对象（EBO）
    glGenBuffers(1, &VBO);
    // 将相应缓冲对象绑定（设置）为“当前”顶点缓冲对象和索引缓冲对象
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 分配内存并将数据复制到 被绑定到指定缓冲对象类型的 缓冲对象中（将数据复制到缓冲中供OpenGL使用）
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 基本同上
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /********* 索引缓冲对象 ********/
    // https://learnopengl-cn.github.io/img/01/04/vertex_array_objects_ebo.png
    // glDrawElements函数从当前绑定到GL_ELEMENT_ARRAY_BUFFER目标的EBO中获取索引。
    // 这意味着我们必须在每次要用索引渲染一个物体时绑定相应的EBO。
    // 不过顶点数组对象（VAO）同样可以保存索引缓冲对象（IBO，index buffer object）的绑定状态。
    // VAO 绑定时正在绑定的索引缓冲对象（IBO）会被保存为VAO的元素缓冲对象（EBO，element buffer object）。绑定VAO的同时也会自动绑定EBO。

    // 1. 设置顶点属性指针
    // 设置 OpenGL 解析顶点数组的方式
    // 并且会将当前 vertex buffer object（GL_ARRAY_BUFFER） 注册
    // - index：设置 vertex attribute 的索引，在 顶点shader 中将会使用（layout( location = 0 )）
    // - size：每个 vertex attribute 的元素个数
    // - type：元素类型
    // - normalized：是否要将顶点坐标转换为标准坐标—— 0~1 或 -1~1
    // - 每个 vertex attribute 之间的偏移（byte）。
    //   0 等效于 vertex attribute 的大小（紧密排列，tightly packed）
    // - pointer：第一个元素在 vertex attribute 中的位置（从零开始）
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // 每个顶点属性从一个VBO管理的内存中获得它的数据，而具体是从哪个VBO（程序中可以有多个VBO）
    // 获取则是通过在调用glVertexAttribPointer时绑定到GL_ARRAY_BUFFER的VBO决定的。

    // 解绑 （顶点）数组缓冲对象
    // glVertexAttribPointer 已经将 VBO 对应的 顶点缓冲对象 绑定为
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    // ？索引缓冲对象被储存在 顶点数组对象 中，当一个顶点数组对象处于使用状态时不要解绑 索引缓冲对象
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // 解绑 顶点数组对象 以避免其被意外修改（绘制时需要再次绑定）
    // glBindVertexArray(0);

    // 将多边形渲染为线段
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //2}}}

    // 每次窗口大小改变时，调整视口的大小
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // 主循环
    while(!glfwWindowShouldClose(window))
    {
        // 处理输入
        processInput(window);

        // 渲染
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   // 状态设置函数，设置清空屏幕所用的颜色
        glClear(GL_COLOR_BUFFER_BIT);           // 状态使用函数，清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色

        //{{{2
        // 3. 绘制物体
        // ~~glDrawArrays(GL_TRIANGLES, 0, 6);~~
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        // glDrawArrays(GL_TRIANGLES, 3, 6);
        //
        // 从当前绑定到GL_ELEMENT_ARRAY_BUFFER目标的EBO中获取索引
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //2}}}

        // 检查并调用事件，交换缓冲
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    return 0;
}
