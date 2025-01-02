#include "glad/gl.h"
#include "window.hpp"
#include "models/trimesh.hpp"

#include <GLFW/glfw3.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>

int main(void){

    Window window(800, 600, "Hello renderer", NULL, NULL);

    std::vector<float> positions({
         0.5f,  0.5f,  // top right
         0.5f, -0.5f,  // bottom right
        -0.5f, -0.5f,  // bottom left
        -0.5f, 0.51f,
         0.5f, 0.51f,
         -0.5f, -0.49f,
    });
    std::vector<int> indices({
        0, 1, 2,
        3, 4, 5
    });

    std::vector<uint8_t> colors ({
        255, 0, 0,
        0, 255, 0,
        0, 0, 255,
        255, 255, 0,
        255, 0, 255,
        0, 255, 255
    });

    TriMesh mesh(2, positions, indices, colors);

    while (!glfwWindowShouldClose(window.win)){
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (glfwGetKey(window.win, GLFW_KEY_V) == GLFW_PRESS){
            mesh.setColors(255, 255, 255);
        } 
        if (glfwGetKey(window.win, GLFW_KEY_C) == GLFW_PRESS) {
            mesh.setColors(colors);
        }

        glUniformMatrix4fv(glGetUniformLocation(mesh.prog->id, "mvp"), 1, false, &(window.windata->mvp[0][0]));
        mesh.draw();
        glfwSwapBuffers(window.win);
    }

    glfwTerminate();
}
