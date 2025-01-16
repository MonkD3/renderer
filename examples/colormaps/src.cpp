#include "buffer.hpp"
#include "colormaps.hpp"
#include "glad/gl.h"
#include "shader.hpp"
#include "window.hpp"
#include "buffer.hpp"
#include "array.hpp"
#include <cstdio>
#include <cstdlib>

int main(void){

    Window window(800, 600, "Hello renderer", NULL, NULL);

    // Colormap colors
    std::vector<unsigned char> bwr({
        0,   0,   255, // Blue
        255, 255, 255, // White
        255, 0,   0,   // Red
    });

    ColorMap cmap(3, bwr);
    cmap.bind();

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
    vao.setAttribute(0, 2, GL_FLOAT, false, 2 * sizeof(float), (void*)0);
    vao.attachBuffer(&vbo);
    vao.enableAttribute(0);

    VBO vbo_val;
    vbo_val.bind();
    vbo_val.setData(sizeof(val), val);
    vao.setAttribute(1, 1, GL_FLOAT, false, sizeof(float), (void*)0);
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
