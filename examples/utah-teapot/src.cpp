#include "buffer.hpp"
#include "glad/gl.h"
#include "shader.hpp"
#include "uniforms.hpp"
#include "window.hpp"
#include "buffer.hpp"
#include "array.hpp"
#include <cstdio>
#include <cstdlib>
#include <glm/geometric.hpp>
#include <sys/cdefs.h>
#include <glm/ext/matrix_transform.hpp>

int main(void){

    // Initializing a window also initializes GLFW and GLAD
    // automatically
    Window window(800, 600, "Hello renderer", NULL, NULL);
    glEnable(GL_DEPTH_TEST);

    Shader vshd("../examples/utah-teapot/shader.vert", SHADER_VERTEX);
    Shader fshd("../examples/utah-teapot/shader.frag", SHADER_FRAGMENT);
    ShaderProgram prog;
    prog.attachShader(&vshd);
    prog.attachShader(&fshd);
    prog.compile();

    std::vector<float> pos;
    FILE* tris = fopen("../examples/utah-teapot/teapot_bezier1.tris", "r");

    int ntris;
    fscanf(tris, "%d", &ntris);

    float x, y, z;
    for (int i = 0; i < ntris; i++){
        for (int j = 0; j < 3; j++){
            fscanf(tris, "%f %f %f", &x, &y, &z);
            pos.push_back(x);
            pos.push_back(y);
            pos.push_back(z);
        }
    }

    std::vector<float> normals;
    for (int i = 0; i < ntris; i++){
        glm::vec3 v0(pos[9*i], pos[9*i+1], pos[9*i+2]);
        glm::vec3 v1(pos[9*i+3], pos[9*i+4], pos[9*i+5]);
        glm::vec3 v2(pos[9*i+6], pos[9*i+7], pos[9*i+8]);

        v1 -= v0;
        v2 -= v0;
        glm::vec3 n = glm::cross(v1, v2);
        for (int j = 0; j < 3; j++){
            normals.push_back(n[0]);
            normals.push_back(n[1]);
            normals.push_back(n[2]);
        }
    }

    VAO vao;
    vao.bind();

    VBO vbo;
    vbo.bind();
    vbo.setData(sizeof(pos[0])*pos.size(), pos.data());
    vao.setAttribute(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
    vao.attachBuffer(&vbo);
    vao.enableAttribute(0);
    
    VBO vbo_normals;
    vbo_normals.bind();
    vbo_normals.setData(sizeof(normals[0])*normals.size(), normals.data());
    vao.setAttribute(1, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
    vao.attachBuffer(&vbo_normals);
    vao.enableAttribute(1);

    prog.use();
    WorldUniformBlock& world = window.scene->worldBlock;
    while (!glfwWindowShouldClose(window.win)){
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        world.view = glm::rotate(world.view, glm::radians(0.2f), glm::vec3(1.0f, 1.0f, 1.0f));
        world.iview = glm::inverse(world.view);

        glUniformMatrix4fv(glGetUniformLocation(prog.id, "mvp"), 1, false, &(world.view[0][0]));
        glUniformMatrix4fv(glGetUniformLocation(prog.id, "imvp"), 1, false, &(world.iview[0][0]));

        glDrawArrays(GL_TRIANGLES, 0, ntris*3);
        glfwSwapBuffers(window.win);
    }

    glfwTerminate();
}
