#pragma once

#include <glad/gl.h>
#include <vector>
#include <ctime>

enum ShaderType {
    NO_SHADER = 0,
    SHADER_GEOMETRY = GL_GEOMETRY_SHADER,
    SHADER_VERTEX   = GL_VERTEX_SHADER,
    SHADER_FRAGMENT = GL_FRAGMENT_SHADER,
};

struct Shader {
    char const* fname;
    unsigned int id;
    ShaderType st;

    Shader();
    Shader(char const* _path, ShaderType _st);
    ~Shader();

    // Not implemented yet
    bool update() const {return false;};
};


struct ShaderProgram {
    unsigned int id;
    bool is_compiled;
    std::vector<Shader*> shd;

    ShaderProgram();
    ~ShaderProgram();

    void attachShader(Shader* s);
    void compile();
    void use() const;
};
