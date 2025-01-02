#include "glad/gl.h"
#include "window.hpp"
#include "models/trimesh.hpp"
#include <cmath>
#include <cstdint>
#include <renderer.hpp>
#include <buffer.hpp>
#include <array.hpp>
#include <cstdio>
#include <cstdlib>
#include <vector>

float fun(float const x, float const y){
    return std::sin(2.f*M_PI*x)*std::cos(2.f*M_PI*y);// std::exp(- (x*x + y*y)/0.1f);
}

void cmap(float const val, uint8_t* color){
    color[0] = 255*val;
    color[1] = 0;
    color[2] = 255*val;
}

int main(void){

    Window window(800, 600, "Hello renderer", NULL, NULL);

    float constexpr W = 2.f;
    float constexpr H = 2.f;
    float constexpr x0 = -1.0f;
    float constexpr y0 = -1.0f;
    int constexpr nx = 201;
    int constexpr ny = 201;
    float constexpr hx = W / (nx-1);
    float constexpr hy = H / (ny-1);

    std::vector<float> positions;
    for (int j = 0; j < ny; j++){
        for (int i = 0; i < nx; i++){
            positions.push_back(x0 + hx*i);
            positions.push_back(y0 + hy*j);
        }
    }


    std::vector<int> indices;
    for (int i = 0; i < nx-1; i++){
        for (int j = 0; j < ny-1; j++){
            // Lower triangle in cell (i, j)
            indices.push_back(j*nx + i);
            indices.push_back(j*nx + i+1);
            indices.push_back((j+1)*nx + i);

            // Upper triangle in cell (i, j)
            indices.push_back((j+1)*nx + i);
            indices.push_back((j+1)*nx + i+1);
            indices.push_back(j*nx + i+1);
        }
    }

    std::vector<uint8_t> colors;
    uint8_t col[3];
    for (int j = 0; j < ny; j++){
        for (int i = 0; i < nx; i++){
            float val = fun(positions[2*(j*nx+i)], positions[2*(j*nx+i)+1]);
            cmap(val, col);
            colors.push_back(col[0]);
            colors.push_back(col[1]);
            colors.push_back(col[2]);
        }
    }

    TriMesh mesh(2, positions, indices, colors);
    window.addModel(&mesh);

    while (!glfwWindowShouldClose(window.win)){
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        window.draw();
        glfwSwapBuffers(window.win);
    }

    glfwTerminate();
}
