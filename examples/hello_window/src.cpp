#include "buffer.hpp"
#include "glad/gl.h"
#include "shader.hpp"
#include "window.hpp"
#include <renderer.hpp>
#include <buffer.hpp>
#include <array.hpp>
#include <cstdio>
#include <cstdlib>


void framebufferSizeCallback(__attribute__((unused)) GLFWwindow* window, int width, int height){ glViewport(0, 0, width, height); }  

int main(void){


    Window window(800, 600, "Hot-reloading shader example", NULL, NULL);

    glfwSetFramebufferSizeCallback(window.win, framebufferSizeCallback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Shader vshd("../assets/vertex/pos.vert", VERTEX_SHADER);
    Shader fshd("../assets/fragment/black.frag", FRAGMENT_SHADER);
    ShaderProgram prog;
    prog.attach_shader(&vshd);
    prog.attach_shader(&fshd);
    prog.compile();

    float pos[8] = {
         0.5f,  0.5f,  // top right
         0.5f, -0.5f,  // bottom right
        -0.5f, -0.5f,  // bottom left
        -0.5f,  0.5f   // top left 
    };
    unsigned int indices[6] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };  

    VAO vao;
    vao.bind();

    VBO vbo;
    vbo.bind();
    vbo.setData(sizeof(pos), pos);
    vao.attach_buffer(&vbo);

    EBO ebo;
    ebo.bind();
    ebo.setData(sizeof(indices), indices);
    ebo.setAttribute(0, 2, BUF_FLOAT, false, 2 * sizeof(float), (void*)0);
    vao.attach_buffer(&ebo);
    vao.enableAttribute(0);

    prog.use();
    while (!glfwWindowShouldClose(window.win)){
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window.win);
    }

    glfwTerminate();
}
