#include "buffer.hpp"
#include "colormaps.hpp"
#include "glad/gl.h"
#include "shader.hpp"
#include "trimesh.hpp"
#include "uniforms.hpp"
#include "window.hpp"
#include "buffer.hpp"
#include "array.hpp"
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include <sys/cdefs.h>
#include <vector>

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

    // Colormap 
    std::vector<unsigned char> bwr({
        0,   0,   255, // Blue
        255, 255, 255, // White
        255, 0,   0,   // Red
    });
    ColorMap cmap(3, bwr);
    cmap.bind();

    std::vector<float>pos({
         0.5f, -0.5f,  // bottom right
        -0.5f, -0.5f,  // bottom left
         0.5f,  0.5f,  // top right
        -0.5f,  0.5f,  // top left
    });
    std::vector<int> indices({
        0, 1, 2,
        2, 1, 3,
    });
    TriMesh mesh(2, pos, indices);
    window.addModel(&mesh);

    std::vector<unsigned char> nodecol({
        255, 0, 0,
        0, 255, 0, 
        0, 0, 255,
        255, 255, 255
    });

    std::vector<unsigned char> ccol({
        255, 255, 255
    });

    std::vector<float> field({
        0.0f, 1.f, 0.f, 1.f 
    });

    float fieldRange[2] = {
        FLT_MAX,
        -FLT_MAX,
    };
    for (int i = 0; i < 4; i++){
        fieldRange[0] = fminf(fieldRange[0], field[i]);
        fieldRange[1] = fmaxf(fieldRange[1], field[i]);
    }
    cmap.cmin = fieldRange[0];
    cmap.cmax = fieldRange[1];

    // Normals
    mesh.vao.setDefaultAttributeValues4f(2, 0.0f, 0.0f, 1.0f, 0.0f);

    /*prog.use();*/
    while (!glfwWindowShouldClose(window.win)){
        glfwPollEvents();

        if (useNodeColor){
            mesh.setColor(nodecol);
        }
        if (useConstantColor){
            mesh.setColor(ccol[0], ccol[1], ccol[2]);
        }
        if (useCmap){
            mesh.useCmap(cmap);
            mesh.setField(field);
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        window.draw();

        glfwSwapBuffers(window.win);
    }

    glfwTerminate();
}
