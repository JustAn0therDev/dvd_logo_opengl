#include "glad.h"
#include <GLFW/glfw3.h>  
#include <stdio.h>
#include <stdbool.h>


# define WINDOW_HEIGHT 1080
# define WINDOW_WIDTH 1920

typedef struct rgba_colors {
    float red;
    float green;
    float blue;
    float alpha;
} rgba;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    // If the ESCAPE key wasn't pressed, the "glfwGetKey" function returns GLFW_RELEASE.
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main(void)
{
    rgba render_colors;
    render_colors.red = 0.0f;
    render_colors.green = 0.7f;
    render_colors.blue = 0.0f;
    render_colors.alpha = 0.5f;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);

    if (window == NULL)
    {
        printf("Failed to create GLFW window.\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD.\n");
        return -1;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(render_colors.red, render_colors.green, render_colors.blue, render_colors.alpha);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwTerminate();
    return 0;
}
