#include "buffer.hpp"
#include "colormaps.hpp"
#include "glad/gl.h"
#include "shader.hpp"
#include "uniforms.hpp"
#include "window.hpp"
#include "buffer.hpp"
#include "array.hpp"
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include <sys/cdefs.h>

bool useNodeColor = true;
bool useConstantColor = false;
bool useCmap = false;

void myKeyCallback(Window* win, int key, int scancode, int action, int mods){
    Scene* scene = win->scene;
    WorldUniformBlock& world = scene->worldBlock; 

    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        useNodeColor = true;
        useConstantColor = false;
        useCmap = false;
    }
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        useNodeColor = false;
        useConstantColor = true;
        useCmap = false;
    }
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
        useNodeColor = false;
        useConstantColor = false;
        useCmap = true;
    }
    else if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        world.worldOpts[0] = !world.worldOpts[0];
        world.light = glm::vec4(1.0f, 1.0f, 0.0f, 0.0f);
    }
    else if (key == GLFW_KEY_I && action == GLFW_PRESS) {
        world.worldOpts[0] = true;
        world.worldOpts[1] = !world.worldOpts[1];
        world.light = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}

int main(void){

    // Initializing a window also initializes GLFW and GLAD
    // automatically
    Window window(800, 600, "Hello renderer", NULL, NULL);
    window.keyCallback = myKeyCallback;
    WorldUniformBlock& world = window.scene->worldBlock;

    Shader vshd("../examples/test-uniform-blocks/defaultVertex.vert", SHADER_VERTEX);
    Shader fshd("../examples/test-uniform-blocks/defaultFrag.frag", SHADER_FRAGMENT);
    ShaderProgram prog;
    prog.attachShader(&vshd);
    prog.attachShader(&fshd);
    prog.compile();

    prog.setUniformBlockBinding("worldBlock", 0);
    UBO ubo;
    ubo.bind();
    ubo.bindUniformRange(0, 0, sizeof(WorldUniformBlock));
    ubo.setData(sizeof(WorldUniformBlock), &world);

    // Colormap 
    std::vector<unsigned char> bwr({
        0,   0,   255, // Blue
        255, 255, 255, // White
        255, 0,   0,   // Red
    });
    ColorMap cmap(3, bwr);
    cmap.bind();

    float pos[8] = {
         0.5f, -0.5f,  // bottom right
        -0.5f, -0.5f,  // bottom left
         0.5f,  0.5f,  // top right
        -0.5f,  0.5f,  // top left
    };

    unsigned char nodecol[12] = {
        255, 0, 0,
        0, 255, 0, 
        0, 0, 255,
        255, 255, 255
    };

    float ccol[3] = {
        1.0f, 1.0f, 1.0f
    };

    float field[4] = {
        0.0f, 1.f, 0.f, 1.f 
    };

    float fieldRange[2] = {
        FLT_MAX,
        -FLT_MAX,
    };
    for (int i = 0; i < 4; i++){
        fieldRange[0] = fminf(fieldRange[0], field[i]);
        fieldRange[1] = fmaxf(fieldRange[1], field[i]);
    }


    VAO vao;
    vao.bind();

    VBO vbo;
    vbo.bind();
    vbo.setData(sizeof(pos), pos);
    vao.setAttribute(0, 2, BUF_FLOAT, false, 2 * sizeof(float), (void*)0);
    vao.attachBuffer(&vbo);
    vao.enableAttribute(0);

    VBO vcol;
    vcol.bind();
    vcol.setData(sizeof(nodecol), nodecol);
    vao.setAttribute(1, 3, BUF_UBYTE, true, 3*sizeof(unsigned char), 0);
    vao.attachBuffer(&vcol);
    vao.enableAttribute(1);

    // Normals
    vao.setDefaultAttributeValues4f(2, 0.0f, 0.0f, 1.0f, 0.0f);

    prog.use();
    while (!glfwWindowShouldClose(window.win)){
        glfwPollEvents();

        prog.setUniform1b("useCmap", useCmap);
        
        if (useNodeColor){
            vcol.bind();
            vcol.setData(sizeof(nodecol), nodecol);
            vao.setAttribute(1, 3, BUF_UBYTE, true, 3*sizeof(unsigned char), 0);
            vao.enableAttribute(1);
        }
        if (useConstantColor){
            vao.setDefaultAttributeValues3f(1, ccol[0], ccol[1], ccol[2]);
            vao.disableAttribute(1);
        }
        if (useCmap){
            vcol.bind();
            vcol.setData(sizeof(field), field);
            vao.setAttribute(1, 1, BUF_FLOAT, false, sizeof(float), 0);
            vao.enableAttribute(1);
            prog.setUniform2f("cmapRange", fieldRange[0], fieldRange[1]);
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ubo.setSubData(0, sizeof(WorldUniformBlock), &world);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glfwSwapBuffers(window.win);
    }

    glfwTerminate();
}
