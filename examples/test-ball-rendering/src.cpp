#include "buffer.hpp"
#include "glad/gl.h"
#include "shader.hpp"
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
    vbo.setAttribute(0, 3, BUF_FLOAT, false, 3 * sizeof(float), (void*)0);
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
    pos.setAttribute(1, 3, BUF_FLOAT, false, 3*sizeof(float), 0);
    vao.attachBuffer(&pos);
    vao.enableAttribute(1);
    glVertexAttribDivisor(1, 1);

    VBO vbo_rad;
    vbo_rad.bind();
    vbo_rad.setData(sizeof(rad[0])*rad.size(), rad.data());
    vbo_rad.setAttribute(2, 1, BUF_FLOAT, false, sizeof(float), (void*)0);
    vao.attachBuffer(&vbo_rad);
    vao.enableAttribute(2);
    glVertexAttribDivisor(2, 1);


    VBO light;
    light.bind();
    light.setData(sizeof(light_direction[0])*light_direction.size(), light_direction.data());
    light.setAttribute(3, 3, BUF_FLOAT, false, 3*sizeof(float), 0);
    vao.attachBuffer(&light);
    vao.enableAttribute(3);
    glVertexAttribDivisor(3, 1);

    prog.use();

    int mvpLoc = glGetUniformLocation(prog.id, "mvp");
    int imvpLoc = glGetUniformLocation(prog.id, "imvp");
    int arLoc = glGetUniformLocation(prog.id, "aspectRatio");
    int zLoc = glGetUniformLocation(prog.id, "zoom");
    while (!glfwWindowShouldClose(window.win)){
        glfwPollEvents();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        window.scene->mvp = glm::rotate(window.scene->mvp, glm::radians(0.2f), glm::vec3(1.0f, 1.0f, 1.0f));

        if (mvpLoc >= 0) glUniformMatrix4fv(mvpLoc, 1, false, &(window.scene->mvp[0][0]));
        if (imvpLoc >= 0) glUniformMatrix4fv(imvpLoc, 1, false, &(window.scene->imvp[0][0]));
        if (arLoc >= 0) glUniform1f(arLoc, window.scene->aspectRatio);
        if (zLoc >= 0) glUniform1f(zLoc, window.scene->zoom);

        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, n*n); 
        glfwSwapBuffers(window.win);
    }

    glfwTerminate();
}
