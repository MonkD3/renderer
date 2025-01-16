#include "buffer.hpp"
#include "glad/gl.h"
#include "shader.hpp"
#include "window.hpp"
#include "buffer.hpp"
#include "array.hpp"
#include <cstdio>
#include <cstdlib>
#include <sys/cdefs.h>

int main(void){

    // Initializing a window also initializes GLFW and GLAD
    // automatically
    Window window(800, 600, "Hello renderer", NULL, NULL);
    glEnable(GL_BLEND);
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    Shader vshd("../examples/test-blend/shader.vert", SHADER_VERTEX);
    Shader fshd("../examples/test-blend/shader.frag", SHADER_FRAGMENT);
    ShaderProgram prog;
    prog.attachShader(&vshd);
    prog.attachShader(&fshd);
    prog.compile();

    float pos[54] = {
         0.5f,  0.5f, 0.0f, // top right
         0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
         0.5f,  0.5f, 0.0f, // top right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f, // top left
                            
         0.5f,  0.5f, -0.2f, // top right
         0.5f, -0.5f, -0.2f, // bottom right
        -0.5f, -0.5f, -0.2f, // bottom left
         0.5f,  0.5f, -0.2f, // top right
        -0.5f, -0.5f, -0.2f, // bottom left
        -0.5f,  0.5f, -0.2f, // top left
                             
         0.5f,  0.5f, -0.4f, // top right
         0.5f, -0.5f, -0.4f, // bottom right
        -0.5f, -0.5f, -0.4f, // bottom left
         0.5f,  0.5f, -0.4f, // top right
        -0.5f, -0.5f, -0.4f, // bottom left
        -0.5f,  0.5f, -0.4f, // top left
    };
    unsigned char col[54] = {
         255, 0, 0, // top right
         255, 0, 0, // bottom right
         255, 0, 0, // bottom left
         255, 0, 0, // top right
         255, 0, 0, // bottom left
         255, 0, 0, // top left
                            
         0,  255, 0, // top right
         0,  255, 0, // bottom right
         0,  255, 0, // bottom left
         0,  255, 0, // top right
         0,  255, 0, // bottom left
         0,  255, 0, // top left
                             
         0, 0, 255, // top right
         0, 0, 255, // bottom right
         0, 0, 255, // bottom left
         0, 0, 255, // top right
         0, 0, 255, // bottom left
         0, 0, 255, // top left
    };

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
    vao.setAttribute(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
    vao.attachBuffer(&vbo);
    vao.enableAttribute(0);

    VBO vcol;
    vcol.bind();
    vcol.setData(sizeof(col), col);
    vao.setAttribute(1, 3, GL_UNSIGNED_BYTE, true, 3 * sizeof(unsigned char), (void*)0);
    vao.attachBuffer(&vcol);
    vao.enableAttribute(1);

    prog.use();
    while (!glfwWindowShouldClose(window.win)){
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // The renderer offers default user-interactions such as 
        // dragging and zooming. The related matrix transform is 
        // located in the "mvp" attributes of the window scene.
        // If you do not want it, you can either 
        // 1) not use the matrix transform (the transform is still 
        // computed)
        // 2) disable the default callbacks (the transform is not 
        // computed anymore)
        prog.setUniformMat4f("mvp", &(window.scene->worldBlock.view[0][0]), false);

        glDrawArrays(GL_TRIANGLES, 0, 18);
        glfwSwapBuffers(window.win);
    }

    glfwTerminate();
}
