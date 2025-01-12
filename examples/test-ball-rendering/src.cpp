#include "buffer.hpp"
#include "glad/gl.h"
#include "shader.hpp"
#include "uniforms.hpp"
#include "window.hpp"
#include "buffer.hpp"
#include "array.hpp"
#include <cstdio>
#include <cstdlib>
#include <glm/ext/matrix_transform.hpp>
#include <vector>


int main(void){

    // Initializing a window also initializes GLFW and GLAD
    // automatically
    Window window(800, 600, "Hello renderer", NULL, NULL);
    glEnable(GL_DEPTH_TEST);

    Shader vshd("../examples/test-ball-rendering/shader.vert", SHADER_VERTEX);
    Shader fshd("../examples/test-ball-rendering/shader.frag", SHADER_FRAGMENT);
    ShaderProgram prog;
    prog.attachShader(&vshd);
    prog.attachShader(&fshd);
    prog.compile();

    // Drawed using a GL_TRIANGLE_STRIP, this makes a square
    float instanceVertexPos[12] = {
          1.f, -1.f, 0.0f,  // Bot-right
         -1.f, -1.f, 0.0f,  // Bot-left
          1.f,  1.f, 0.0f,  // Top-right
         -1.f,  1.f, 0.0f,  // Top-left
    };

    VAO vao;
    vao.bind();

    VBO vbo;
    vbo.bind();
    vbo.setData(sizeof(instanceVertexPos), instanceVertexPos);
    vao.setAttribute(0, 3, BUF_FLOAT, false, 3 * sizeof(float), (void*)0);
    vao.attachBuffer(&vbo);
    vao.enableAttribute(0);

    int n = 10;
    std::vector<float> light_direction;
    light_direction.reserve(3*n*n);
    std::vector<float> positions;
    positions.reserve(3*n*n);
    std::vector<float> rad;
    rad.reserve(n*n);
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            float const x = -1.0f + 2.0f/(n-1) * i;
            float const y = -1.0f + 2.0f/(n-1) * j;
            positions.push_back(x);
            positions.push_back(y);
            positions.push_back(0.0f);

            float const lx = -10.0f + 10.0f/(n-1) * i;
            float const ly = -10.0f + 10.0f/(n-1) * j;
            float const lz = 1.0f;
            light_direction.push_back(lx);
            light_direction.push_back(ly);
            light_direction.push_back(lz);

            rad.push_back(0.05f);
        }
    }

    VBO pos;
    pos.bind();
    pos.setData(sizeof(positions[0])*positions.size(), positions.data());
    vao.setAttribute(1, 3, BUF_FLOAT, false, 3*sizeof(float), 0);
    vao.attachBuffer(&pos);
    vao.enableAttribute(1);
    glVertexAttribDivisor(1, 1);

    VBO vbo_rad;
    vbo_rad.bind();
    vbo_rad.setData(sizeof(rad[0])*rad.size(), rad.data());
    vao.setAttribute(2, 1, BUF_FLOAT, false, sizeof(float), (void*)0);
    vao.attachBuffer(&vbo_rad);
    vao.enableAttribute(2);
    glVertexAttribDivisor(2, 1);


    VBO light;
    light.bind();
    light.setData(sizeof(light_direction[0])*light_direction.size(), light_direction.data());
    vao.setAttribute(3, 3, BUF_FLOAT, false, 3*sizeof(float), 0);
    vao.attachBuffer(&light);
    vao.enableAttribute(3);
    glVertexAttribDivisor(3, 1);

    prog.use();

    WorldUniformBlock& world = window.scene->worldBlock;
    while (!glfwWindowShouldClose(window.win)){
        glfwPollEvents();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        world.view = glm::rotate(world.view, glm::radians(0.2f), glm::vec3(1.0f, 1.0f, 1.0f));

        prog.setUniformMat4f("mvp", &(world.view[0][0]), false);
        prog.setUniformMat4f("imvp", &(world.iview[0][0]), false);
        prog.setUniform1f("aspectRatio", world.scalings[0]);
        prog.setUniform1f("zoom", world.scalings[1]);

        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, n*n); 
        glfwSwapBuffers(window.win);
    }

    glfwTerminate();
}
