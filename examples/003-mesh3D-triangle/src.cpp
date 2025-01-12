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
    glEnable(GL_DEPTH_TEST);
    
    window.keyCallback = myKeyCallback;

    // First mesh (with only 1 tet)
    std::vector<float> positions({
        -0.5f, 0.0f, -0.01f,
        0.5f, 0.0f, -0.01f,
        0.0f, 0.0f, -0.51f,
        0.0f, -0.5f, -0.01f
    });
    std::vector<int> indices({
        0, 1, 2,
        0, 1, 3,
        0, 2, 3,
        1, 2, 3
    });

    std::vector<uint8_t> colors ({
        255, 0, 0,
        255, 0, 0,
        255, 0, 0,
        255, 0, 0,
    });
    TriMesh tri1(3, positions, indices);
    tri1.setColor(colors);
    window.addModel(&tri1);

    // Second mesh (with only 1 triangle)
    std::vector<float> positions2({
        -0.5f, 0.0f, 0.01f,
        0.5f, 0.0f, 0.01f,
        0.0f, 0.0f, 0.51f,
        0.0f, -0.5f, 0.01f
    });
    std::vector<int> indices2({
        0, 1, 2,
        0, 1, 3,
        0, 2, 3,
        1, 2, 3
    });

    std::vector<uint8_t> colors2({
        0, 0, 255,
        0, 0, 255,
        0, 0, 255,
        0, 0, 255,
    });
    TriMesh tri2(3, positions2, indices2);
    tri2.setColor(colors2);
    window.addModel(&tri2);

    while (!glfwWindowShouldClose(window.win)){
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        window.draw();
        glfwSwapBuffers(window.win);
    }

    glfwTerminate();
}
