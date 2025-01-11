#pragma once

#include <glad/gl.h>
#include <string>
#include <vector>
#include <ctime>
#include <unordered_map>

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
    unsigned int id;  // OpenGL id for the Shader program
    bool is_compiled; // Is the program compiled ?
    std::vector<Shader*> shd; // List of shader attached to this program

    // HashMap linking the uniforms name to their location in the program
    std::unordered_map<std::string, int> uniformsLocations;
    std::unordered_map<std::string, unsigned int> uniformsBlockIndices;

    ShaderProgram();
    ~ShaderProgram();

    // Attach a given shader to the program
    void attachShader(Shader* s);

    // Compile the program
    void compile();

    // Use the program for the subsequent rendering calls
    void use() const;

    int getUniformLocation(const std::string& uname);
    unsigned int getUniformBlockIndex(const std::string& uname);
    void setUniformBlockBinding(const std::string& uname, unsigned int binding);
    

    void setUniformMat4f(const std::string& uname, float const* const mat, bool transpose);
    void setUniform1f(const std::string& uname, float v0);
    void setUniform2f(const std::string& uname, float v0, float v1);
    void setUniform1b(const std::string& uname, bool v0);
};
