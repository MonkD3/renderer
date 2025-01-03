#include "glad/gl.h"
#include "window.hpp"
#include "models/balls.hpp"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <sys/cdefs.h>
#include <vector>

int main(void){

    int constexpr dim = 3;
    int constexpr n = 2000;

    // Initializing a window also initializes GLFW and GLAD
    // automatically
    Window window(800, 600, "Hello renderer", NULL, NULL);
    glEnable(GL_DEPTH_TEST);

    std::vector<float> pos;
    pos.reserve(dim*n);

    std::vector<float> rad;
    rad.reserve(n);

    std::vector<float> v;
    v.reserve(dim*n);

    for (int i = 0; i < n; i++){
        for (int d = 0; d < dim; d++){
            pos.push_back((float) rand() / RAND_MAX);
            v.push_back(0.002f * (float) rand() / RAND_MAX);
        }

        rad.push_back(0.005f * (float) rand() / RAND_MAX);
    }

    Balls b(dim, pos, rad);
    window.addModel(&b);

    while (!glfwWindowShouldClose(window.win)){
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        window.draw();

        for (int i = 0; i < n; i++){
            for (int d = 0; d < dim; d++){
                pos[dim*i+d] += v[dim*i+d];
                if (pos[dim*i+d] > 1.0f) pos[dim*i+d] = -1.0f;
                if (pos[dim*i+d] < -1.0f) pos[dim*i+d] = 1.0f;
            }
        }
        b.setCenters(pos);
        
        glfwSwapBuffers(window.win);
    }

    /*delete positions;*/
    /*delete radius;*/

    glfwTerminate();
}
