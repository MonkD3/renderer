#include "buffer.hpp"
#include "glad/gl.h"
#include "shader.hpp"
#include "window.hpp"
#include "model.hpp"
#include <renderer.hpp>
#include <buffer.hpp>
#include <array.hpp>
#include <cstdio>
#include <cstdlib>
#include <vector>


void framebufferSizeCallback(__attribute__((unused)) GLFWwindow* window, int width, int height){ glViewport(0, 0, width, height); }  

int main(void){


    Window window(800, 600, "Hot-reloading shader example", NULL, NULL);

    glfwSetFramebufferSizeCallback(window.win, framebufferSizeCallback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    std::vector<float> positions({
         0.5f,  0.5f,  // top right
         0.5f, -0.5f,  // bottom right
        -0.5f, -0.5f,  // bottom left
        -0.5f, 0.51f,
         0.5f, 0.51f,
         -0.5f, -0.49f,
    });
    std::vector<int> indices({
        0, 1, 2,
        3, 4, 5
    });

    TriMesh mesh(2, positions, indices);
    while (!glfwWindowShouldClose(window.win)){
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        mesh.draw();
        glfwSwapBuffers(window.win);
    }

    glfwTerminate();
}
