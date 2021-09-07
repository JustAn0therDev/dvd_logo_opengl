#include "glad.h"
#include <GLFW/glfw3.h>  
#include <stdio.h>
#include <stdbool.h>


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define DEFAULT_AMOUNT_OF_BUFFERS_TO_GENERATE 1



const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "\n"
                                   "void main()\n"
                                   "{\n"
                                   "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}";

typedef struct rgbaColors {
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


unsigned int bindVbo(float vertices[], unsigned int numberOfTriangles)
{
    unsigned int VBO;
    glGenBuffers(DEFAULT_AMOUNT_OF_BUFFERS_TO_GENERATE, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 24 * numberOfTriangles, vertices, GL_STATIC_DRAW);
    return VBO;
}


unsigned int bindVao()
{
    unsigned int VAO;
    glGenVertexArrays(DEFAULT_AMOUNT_OF_BUFFERS_TO_GENERATE, &VAO);
    glBindVertexArray(VAO);
    return VAO;
}


unsigned int bindEbo(unsigned int indices[], unsigned int numberOfIndices)
{
    unsigned int EBO;
    glGenBuffers(DEFAULT_AMOUNT_OF_BUFFERS_TO_GENERATE, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * numberOfIndices, indices, GL_STATIC_DRAW);
    return EBO;
}


unsigned int compileVertexShader()
{
    int success;
    char infoLog[512];
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("SHADER COMPILATION FAILED. LOG:\n%s\n", infoLog);
    }

    return vertexShader;
}


unsigned int compileFragmentShader()
{
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    return fragmentShader;
}


unsigned int createShaderProgram()
{
    return glCreateProgram();
}


void linkShadersToShaderProgram(unsigned int vertexShader, unsigned int fragmentShader, unsigned int shaderProgram)
{
    int success;
    char infoLog[512];

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("SHADER PROGRAM LINKING FAILED. LOG:\n%s\n", infoLog);
        return;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}


void useProgram(unsigned int shaderProgram)
{
    glUseProgram(shaderProgram);
}


void interpretVertexData()
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
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

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "DVD Logo OpenGL", NULL, NULL);

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

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    unsigned int shaderProgram = createShaderProgram();
    unsigned int vertexShader = compileVertexShader();
    unsigned int fragmentShader = compileFragmentShader();

    linkShadersToShaderProgram(vertexShader, fragmentShader, shaderProgram);

    // In OpenGL (or shaders) the x,y,z Cartesian plane is in the middle of the screen, meaning that
    // upwards in the Y axis uses positive values.

    float triangle_size_in_normalized_unit_vectors = 0.5f;
    float vertices[] = {
            triangle_size_in_normalized_unit_vectors, triangle_size_in_normalized_unit_vectors, 0.0f,
            triangle_size_in_normalized_unit_vectors, -triangle_size_in_normalized_unit_vectors, 0.0f,
            -triangle_size_in_normalized_unit_vectors, -triangle_size_in_normalized_unit_vectors, 0.0f,
            -triangle_size_in_normalized_unit_vectors, triangle_size_in_normalized_unit_vectors, 0.0f
    };

    unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
    };

    unsigned int VBO = bindVbo(vertices, 3);
    unsigned int VAO = bindVao();
    unsigned int EBO = bindEbo(indices, 6);

    interpretVertexData();

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(render_colors.red, render_colors.green, render_colors.blue, render_colors.alpha);
        glClear(GL_COLOR_BUFFER_BIT);

        useProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteShader(shaderProgram);

    glfwTerminate();
    return 0;
}
