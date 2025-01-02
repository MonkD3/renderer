#include "buffer.hpp"
#include "glad/gl.h"
#include "shader.hpp"
#include "window.hpp"
#include "buffer.hpp"
#include "array.hpp"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <sys/cdefs.h>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>

int main(void){

    // Initializing a window also initializes GLFW and GLAD
    // automatically
    Window window(800, 600, "Hello renderer", NULL, NULL);

    Shader vshd("../examples/rotate_3D_test/line.vert", SHADER_VERTEX);
    Shader fshd("../examples/rotate_3D_test/line.frag", SHADER_FRAGMENT);
    Shader gshd("../examples/rotate_3D_test/line.geom", SHADER_GEOMETRY);
    ShaderProgram prog;
    prog.attachShader(&vshd);
    prog.attachShader(&fshd);
    prog.attachShader(&gshd);
    prog.compile();

    float pos[18] = {
        0.0f, 0.0f, 0.0f, 
        0.1f, 0.0f, 0.0f, // x axis
        0.0f, 0.0f, 0.0f,
        0.0f, 0.1f, 0.0f, // y axis
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.1f, // z axis
    };

    std::vector<unsigned char> colors({
        255, 0, 0,
        255, 0, 0,
        0, 255, 0,
        0, 255, 0,
        0, 0, 255,
        0, 0, 255,
    });


    // Define the buffers and data that will be rendered, just as done classically with OpenGL :
    // 1) bind a Vertex Array Object (VAO)
    // 2) bind a Vertex Buffer Object (VBO)
    // 3) define how the data in the VBO is laid out 
    // 4) repeat steps 2-3 for every attributes

    VAO vao;
    vao.bind();

    VBO vbo;
    vbo.bind();
    vbo.setData(sizeof(pos), pos);
    vbo.setAttribute(0, 3, BUF_FLOAT, false, 3 * sizeof(float), (void*)0);
    vao.attachBuffer(&vbo);
    vao.enableAttribute(0);

    VBO col;
    col.bind();
    col.setData(colors.size()*sizeof(colors[0]), colors.data());
    col.setAttribute(1, 3, BUF_UBYTE, true, 3*sizeof(unsigned char), 0);
    vao.attachBuffer(&col);
    vao.enableAttribute(1);

    prog.use();
    while (!glfwWindowShouldClose(window.win)){
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniformMatrix4fv(glGetUniformLocation(prog.id, "mvp"), 1, false, &(window.scene->mvp[0][0]));

        glDrawArrays(GL_LINES, 0, 6);
        glfwSwapBuffers(window.win);
    }

    glfwTerminate();
}
