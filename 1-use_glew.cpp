#define GLEW_STATIC         // Does this work on linux?
// g++ ... -D GLEW_STATIC

// include glew.h before other header files that include gl.h(e.g. glfw3.g).

//       |  ^~~~~
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

int main(void)
{
    // Not sure if this will work.
    #ifdef GLEW_STATIC
        std::cout << "GLEW_STATIC\n";
    #else
        std::cout << "!GLEW_STATIC\n";
    #endif // GLEW_STATIC

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Must be called after creating a valid randering context */
    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW Load failed.\n";
        return -2;
    }

    /// {{{2

    float positions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    // Generate a buffer and get it's ID.
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    // Bind the buffer.
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // Fill the buffer.
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    /// 2}}}

    /// {{{3
    glEnableVertexAttribArray(0);
    // Get data from vertex 0,
    // 2 components per vertex attribute,
    // the type of components is float,
    // don't normalize fixed-point numbers,
    // the offset between consecutive vertices is 2 float numbers,
    // the offset of the attribute in every vertex is 0(get every data from the begin of every vertex).
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    /// 3}}}

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /// {{{1
        // glBegin(GL_TRIANGLES);
        // glVertex2f(-0.5f, -0.5f);
        // glVertex2f( 0.0f,  0.5f);
        // glVertex2f( 0.5f, -0.5f);
        // glEnd();
        /// 1}}}

        /// {{{2
        // Draw the bound buffer. Use data from index 0, 3 indices in all.
        glDrawArrays(GL_TRIANGLES, 0, 3);           // And you will get a black triangle on the black background...
        /// 2}}}

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
