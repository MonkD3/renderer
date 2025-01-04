#include "glad/gl.h"
#include "window.hpp"
#include "models/balls.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <sys/cdefs.h>
#include <vector>

int main(void){

    int constexpr dim = 3;

    // Initializing a window also initializes GLFW and GLAD
    // automatically
    Window window(800, 600, "Hello renderer", NULL, NULL);
    glEnable(GL_DEPTH_TEST);

    std::vector<float> pos({
        -0.5f, 0.0f, 0.0f, 
         0.5f, 0.0f, 0.0f, 
    });
    std::vector<float> rad({
        0.25f,
        0.25f
    });

    Balls b(dim, pos, rad);
    window.addModel(&b);

    while (!glfwWindowShouldClose(window.win)){
        glfwWaitEventsTimeout(1);
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        window.draw();
        
        glfwSwapBuffers(window.win);
    }

    glfwTerminate();
}
