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

    Shader vshd("../assets/vertex/pos_col.vert", SHADER_VERTEX);
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
    vbo.setAttribute(0, 2, BUF_FLOAT, false, 2 * sizeof(float), (void*)0);
    vao.attachBuffer(&vbo);
    vao.enableAttribute(0);

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

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window.win);
    }

    glfwTerminate();
}
