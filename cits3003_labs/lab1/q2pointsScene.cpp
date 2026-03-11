#include <iostream>
#include <cstdlib> // Required for rand()

using uint = unsigned int;

// GLM Includes
#include <glm/glm.hpp>

// GLAD and GLFW
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define OPENGL_VERSION_MAJOR 4
#define OPENGL_VERSION_MINOR 1

#include "../helpers/ShaderHelper.h"

// define window properties
#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512
#define WINDOW_TITLE "Lab1 Q2 Points Scene"

// Number of points to generate
const int NumPoints = 4;

// Array to store the points
glm::vec2 points[NumPoints];

void init() {
    // Specify the vertices for a triangle
    glm::vec2 vertices[3] = {
        glm::vec2(-1.0, -1.0), 
        glm::vec2(0.0, 1.0), 
        glm::vec2(1.0, -1.0)
    };

    // Select an arbitrary initial point inside of the triangle
    points[0] = glm::vec2(0.25, 0.50);
    points[1] = glm::vec2(0.25, 0.75);
    points[2] = glm::vec2(0.15, 0.45);
    points[3] = glm::vec2(0.20, 0.60);
    
    // -----------------------------------------------------------------

    // Create a vertex array object
    uint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create and initialize a buffer object
    uint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    
    // Load the data into the GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    // Load shaders
    uint program = ShaderHelper::load_shader("vshader21.glsl", "fshader21.glsl");
    glUseProgram(program);

    // Initialize the vertex position attribute from the vertex shader.
    uint loc = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // Set point size (optional, makes points more visible)
    glPointSize(20.0f); 

    glClearColor(1.0, 1.0, 1.0, 1.0); /* white background */
}

void draw(GLFWwindow *window) {
    glClear(GL_COLOR_BUFFER_BIT); 
    glDrawArrays(GL_POINTS, 0, NumPoints);
    glFlush();
}

void key_callback(GLFWwindow *window, int key, int /*scancode*/, int action, int /*mods*/) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, true);
                break;
        }
    }
}

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif 
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    int status = gladLoadGL((GLADloadfunc) glfwGetProcAddress);
    if (!status) {
        std::cerr << "Failed to Load OpenGL functions, via GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    glfwSetKeyCallback(window, key_callback);
    
    init();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents(); 
        draw(window); 
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}