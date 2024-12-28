#include "buffer.hpp"
#include "glad/gl.h"
#include "shader.hpp"
#include "window.hpp"
#include <cmath>
#include <cstdint>
#include <renderer.hpp>
#include <buffer.hpp>
#include <array.hpp>
#include <cstdio>
#include <cstdlib>
#include <vector>

void velocity_field(float const x, float const y, float* vx, float* vy){
    *vx = 1.f;
    *vy = 0.f;
}

void update_field(int nx, int ny, float hx, float hy, std::vector<float>& values, std::vector<float>& positions){
    float constexpr dt = 1e-4f;
    std::vector<float> newValues(values.size());

    float vx, vy;
    float x, y;

    for (int j = 1; j < ny-1; j++){
        for (int i = 1; i < nx-1; i++){
            x = positions[2*(j*nx+i)];
            y = positions[2*(j*nx+i)+1];
            velocity_field(x, y, &vx, &vy);
            newValues[j*nx + i] = values[j*nx+i] - dt*(
                    vx * (values[j*nx + i+1] - values[j*nx + i-1])/(2.f*hx) 
                    + vy * (values[(j+1)*nx + i] - values[(j-1)*nx + i])/(2.f*hy)
                );
        }
    }
    
    for (int j = 1; j < ny-1; j++){
        for (int i = 1; i < nx-1; i++){
            values[j*nx + i] = newValues[j*nx+i];
        }
    }
}

int main(void){

    Window window(800, 600, "Hello renderer", NULL, NULL);

    float constexpr W = 2.f;
    float constexpr H = 2.f;
    float constexpr x0 = -1.0f;
    float constexpr y0 = -1.0f;
    int constexpr nx = 100;
    int constexpr ny = 100;
    float constexpr hx = W / (nx-1);
    float constexpr hy = H / (ny-1);

    // Colormap colors
    uint8_t bwr[9] = {
        0,   0,   255, // Blue
        255, 255, 255, // White
        255, 0,   0,   // Red
    };

    glEnable(GL_TEXTURE_1D);

    GLuint tex;
    glGenTextures(1, &tex); // get texture id
    glBindTexture(GL_TEXTURE_1D, tex);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB8, 3, 0, GL_RGB, GL_UNSIGNED_BYTE, bwr);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

    Shader vshd("../examples/advection/texture.vert", SHADER_VERTEX);
    Shader fshd("../examples/advection/texture.frag", SHADER_FRAGMENT);
    ShaderProgram prog;
    prog.attachShader(&vshd);
    prog.attachShader(&fshd);
    prog.compile();
    prog.use();

    VAO vao;
    vao.bind();

    std::vector<float> positions;
    for (int j = 0; j < ny; j++){
        for (int i = 0; i < nx; i++){
            positions.push_back(x0 + hx*i);
            positions.push_back(y0 + hy*j);
        }
    }

    VBO pos;
    pos.bind();
    pos.setData(positions.size()*sizeof(float), positions.data());
    vao.attachBuffer(&pos);

    std::vector<unsigned int> indices;
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

    EBO elem;
    elem.bind();
    elem.setData(indices.size()*sizeof(unsigned int), indices.data());
    elem.setAttribute(0, 2, BUF_FLOAT, false, 0, 0);
    vao.attachBuffer(&elem);
    vao.enableAttribute(0);

    std::vector<float> val(nx*ny);
    // Interior initial value
    for (int j = 1; j < ny-1; j++){
        for (int i = 1; i < nx-1; i++){
            val[j*nx + i] = 0.f;
        }
    }

    // Boundary conditions
    for (int i = 0; i < nx; i++){
        val[i] = 0.0f; // lower boundary
        val[(ny-1)*nx + i] = 0.0f; // top boundary
    }
    for (int j = 0; j < ny; j++){
        val[j*nx] = 1.0f; // left boundary
        val[j*nx + nx-1] = 0.0f; // right boundary
    }
    VBO col;
    col.bind();
    col.setData(val.size()*sizeof(float), val.data());
    col.setAttribute(1, 1, BUF_FLOAT, false, 0, 0);
    vao.attachBuffer(&col);
    vao.enableAttribute(1);

    while (!glfwWindowShouldClose(window.win)){
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        update_field(nx, ny, hx, hy, val, positions);
        col.bind();
        col.setData(val.size()*sizeof(float), val.data());
        glfwSwapBuffers(window.win);
    }

    glfwTerminate();
}
