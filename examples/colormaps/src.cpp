#include "buffer.hpp"
#include "glad/gl.h"
#include "shader.hpp"
#include "window.hpp"
#include "buffer.hpp"
#include "array.hpp"
#include <cstdio>
#include <cstdlib>

int main(void){

    Window window(800, 600, "Hello renderer", NULL, NULL);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Colormap colors
    uint8_t bwr[9] = {
        0,   0,   255, // Blue
        255, 255, 255, // White
        255, 0,   0,   // Red
    };

    glEnable(GL_TEXTURE_1D);

    GLuint tex;
    glGenTextures(1, &tex); // get texture id
    glBindTexture(GL_TEXTURE_1D, tex);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB8, 3, 0, GL_RGB, GL_UNSIGNED_BYTE, bwr);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 


    Shader vshd("../examples/colormaps/texture.vert", SHADER_VERTEX);
    Shader fshd("../examples/colormaps/texture.frag", SHADER_FRAGMENT);
    ShaderProgram prog;
    prog.attachShader(&vshd);
    prog.attachShader(&fshd);
    prog.compile();

    float pos[12] = {
        -0.5f,  0.5f,  // top left
        -0.5f, -0.5f,  // bottom left
         0.5f,  0.5f,  // top right
         0.5f,  0.5f,  // top right
        -0.5f, -0.5f,  // bottom left
         0.5f, -0.5f,  // bottom right
    };

    float val[6] = {
        0.f, 0.f, 1.f, 1.f, 0.f, 1.f
    };

    VAO vao;
    vao.bind();

    VBO vbo;
    vbo.bind();
    vbo.setData(sizeof(pos), pos);
    vbo.setAttribute(0, 2, BUF_FLOAT, false, 2 * sizeof(float), (void*)0);
    vao.attachBuffer(&vbo);
    vao.enableAttribute(0);

    VBO vbo_val;
    vbo_val.bind();
    vbo_val.setData(sizeof(val), val);
    vbo_val.setAttribute(1, 1, BUF_FLOAT, false, sizeof(float), (void*)0);
    vao.attachBuffer(&vbo_val);
    vao.enableAttribute(1);

    prog.use();
    while (!glfwWindowShouldClose(window.win)){
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glfwSwapBuffers(window.win);
    }

    glfwTerminate();
}
