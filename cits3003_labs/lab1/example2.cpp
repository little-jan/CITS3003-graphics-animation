#include <iostream>
#include <vector>

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
#define WINDOW_TITLE "Sierpinski Gasket - Triangle"

// Logic Constants
const int NumTimesToSubdivide = 5;
const int NumTriangles = 243;  // 3^5
const int NumVertices  = 3 * NumTriangles;

glm::vec2 points[NumVertices];
int Index = 0;

//----------------------------------------------------------------------------

void triangle(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
{
    points[Index++] = a;
    points[Index++] = b;
    points[Index++] = c;
}

void divide_triangle(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, int count)
{
    if (count > 0) {
        glm::vec2 v0 = (a + b) / 2.0f;
        glm::vec2 v1 = (a + c) / 2.0f;
        glm::vec2 v2 = (b + c) / 2.0f;
        
        divide_triangle(a, v0, v1, count - 1);
        divide_triangle(c, v1, v2, count - 1);
        divide_triangle(b, v2, v0, count - 1);
    }
    else {
        triangle(a, b, c); // Base case: add the vertices to our array
    }
}

//----------------------------------------------------------------------------

void init() {
    glm::vec2 vertices[3] = {
        glm::vec2(-1.0, -1.0), 
        glm::vec2(0.0, 1.0), 
        glm::vec2(1.0, -1.0)
    };

    // Subdivide the original triangle using the recursive logic
    divide_triangle(vertices[0], vertices[1], vertices[2], NumTimesToSubdivide);

    // Create a vertex array object
    uint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create and initialize a buffer object
    uint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    // Load shaders (using your ShaderHelper)
    uint program = ShaderHelper::load_shader("vshader21.glsl", "fshader21.glsl");
    glUseProgram(program);

    // Initialize the vertex position attribute
    uint loc = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glClearColor(1.0, 1.0, 1.0, 1.0); /* white background */
}

void draw(GLFWwindow *window) {
    glClear(GL_COLOR_BUFFER_BIT);
    // Use GL_TRIANGLES instead of GL_POINTS
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
    glFlush();
}

// GLFW key callback
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, true);
                break;
        }
    }
}

int main() {
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE); // Match original "glFlush" style
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif 

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        std::cerr << "Failed to Load OpenGL functions" << std::endl;
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);
    
    init();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents(); 
        draw(window); 
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}