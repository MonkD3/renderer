#include "glad/gl.h"
#include "window.hpp"
#include "models/trimesh.hpp"

#include <GLFW/glfw3.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <sys/cdefs.h>
#include <vector>

void myKeyCallback(Window* window, int key, __attribute_maybe_unused__ int scancode, int action, __attribute_maybe_unused__ int mods){
    if ((key == GLFW_KEY_1) && (action == GLFW_PRESS)){
        window->scene->modelIsRendered[0] = !window->scene->modelIsRendered[0];
    }
    if ((key == GLFW_KEY_2) && (action == GLFW_PRESS)){
        window->scene->modelIsRendered[1] = !window->scene->modelIsRendered[1];
    }
}

int main(void){

    Window window(800, 600, "Hello renderer", NULL, NULL);
    window.keyCallback = myKeyCallback;

    // First mesh (with only 1 triangle)
    std::vector<float> positions({
         0.5f,  0.5f,  // top right
         0.5f, -0.5f,  // bottom right
        -0.5f, -0.5f,  // bottom left
    });
    std::vector<int> indices({
        0, 1, 2,
    });

    std::vector<uint8_t> colors ({
        255, 0, 0,
        0, 255, 0,
        0, 0, 255,
    });
    TriMesh tri1(2, positions, indices);
    tri1.setColors(colors);
    window.addModel(&tri1);

    // Second mesh (with only 1 triangle)
    std::vector<float> positions2({
        -0.5f, 0.51f,
         0.5f, 0.51f,
         -0.5f, -0.49f,
    });
    std::vector<int> indices2({
        0, 1, 2,
    });

    std::vector<uint8_t> colors2({
        255, 255, 0,
        255, 0, 255,
        0, 255, 255
    });
    TriMesh tri2(2, positions2, indices2);
    tri1.setColors(colors2);
    window.addModel(&tri2);

    while (!glfwWindowShouldClose(window.win)){
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        window.draw();
        glfwSwapBuffers(window.win);
    }

    glfwTerminate();
}
