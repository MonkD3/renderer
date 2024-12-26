#include "buffer.hpp"
#include "glad/gl.h"
#include "shader.hpp"
#include "window.hpp"
#include "buffer.hpp"
#include "array.hpp"
#include <cstdio>
#include <cstdlib>

void myMouseButtonCallback(Window* win){
    if (win->windata->m1_pressed) win->useDefaultCursorPositionCallback = !win->useDefaultCursorPositionCallback;
}

int main(void){

    Window window(800, 600, "Hello renderer", NULL, NULL);
    window.mouseButtonCallback = myMouseButtonCallback;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Shader vshd("../assets/vertex/pos.vert", SHADER_VERTEX);
    Shader fshd("../assets/fragment/white.frag", SHADER_FRAGMENT);
    ShaderProgram prog;
    prog.attachShader(&vshd);
    prog.attachShader(&fshd);
    prog.compile();

    float pos[6] = {
         0.5f,  0.5f,  // top right
         0.5f, -0.5f,  // bottom right
        -0.5f, -0.5f,  // bottom left
    };
    VAO vao;
    vao.bind();

    VBO vbo;
    vbo.bind();
    vbo.setData(sizeof(pos), pos);
    vbo.setAttribute(0, 2, BUF_FLOAT, false, 2 * sizeof(float), (void*)0);
    vao.attachBuffer(&vbo);
    vao.enableAttribute(0);

    prog.use();
    while (!glfwWindowShouldClose(window.win)){
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window.win);
    }

    glfwTerminate();
}
