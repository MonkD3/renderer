#include "glad/gl.h"
#include <shader.hpp>
#include <log.h>
#include <cstdio>
#include <string>

Shader::Shader() : fname(NULL), id(0), st(NO_SHADER){};

Shader::Shader(char const* _path, ShaderType _st) : fname(_path), st(_st){ 
    FILE* fd = fopen(fname, "r");
    if (fd == NULL) {
        ERROR("Can't open file '%s'", fname);
        return;
    }

    fseek(fd, 0L, SEEK_END);
    int len = ftell(fd);
    rewind(fd);

    char* buf = new char[len];

    size_t nread = fread(buf, sizeof(char), len, fd);
    if (nread < (size_t) len) {
        WARNING(
            "File '%s' is %u bytes long but only %zu bytes where read, attempting to continue.",
            fname, len, nread);
    }
    fclose(fd);

    GLint res = GL_FALSE;
    int log_len;

    id = glCreateShader(st);

    // Compile shader
    DEBUG("Compiling shader '%s' with id %u", fname, id);
    char const* shd_src = buf;
    glShaderSource(id, 1, &shd_src, &len);
    glCompileShader(id);

    delete [] buf;

    // Check Shader
    glGetShaderiv(id, GL_COMPILE_STATUS, &res);
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_len);
    if ( log_len > 0 ){
        char* err_msg = new char[log_len+1];
        glGetShaderInfoLog(id, log_len, NULL, err_msg);
        err_msg[log_len] = '\0';
        ERROR("%s", err_msg);
        delete [] err_msg;
        return;
    }
};

Shader::~Shader(){
    DEBUG("Deleting shader %u", id);
    glDeleteShader(id);
};

ShaderProgram::ShaderProgram() {
    id = glCreateProgram();
    is_compiled = false;
};

ShaderProgram::~ShaderProgram() {
    DEBUG("Deleting program %u", id);
    glDeleteProgram(id);
}

void ShaderProgram::attachShader(Shader* s){
    shd.push_back(s);
}

void ShaderProgram::compile(){

    bool should_update = !is_compiled;

    // (Re-)attach the modified shaders
    for (size_t i = 0; i < shd.size(); i++){
        if (!is_compiled || shd[i]->update()) {
            DEBUG("Attaching shader '%s' at program %d", shd[i]->fname, id);
            glAttachShader(id, shd[i]->id);
            should_update = true;
        }
    }

    if (should_update) {
        DEBUG("Linking program %u", id);
        glLinkProgram(id);

        GLint res;
        int log_len;

        // Check the program
        glGetProgramiv(id, GL_LINK_STATUS, &res);
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &log_len);
        if ( log_len > 0 ){
            char* err_msg = new char[log_len+1];
            glGetProgramInfoLog(id, log_len+1, NULL, err_msg);
            err_msg[log_len] = '\0';
            ERROR("%s\n", err_msg);
            delete [] err_msg;
            return;
        }
        is_compiled = true;
    }
}

void ShaderProgram::use() const {
    glUseProgram(id);
}

int ShaderProgram::getUniformLocation(const std::string& uname) {
    auto keyval = uniformsLocations.find(uname);
    if (keyval != uniformsLocations.end()) {
        return keyval->second;
    }

    int loc = glGetUniformLocation(id, uname.c_str());
    DEBUG("Inserting uniform '%s' with location %d into the hashtable of program %u", uname.c_str(), loc, id);
    uniformsLocations[uname] = loc;
    return loc;
}
unsigned int ShaderProgram::getUniformBlockIndex(const std::string& uname){
    auto keyval = uniformsBlockIndices.find(uname);
    if (keyval != uniformsBlockIndices.end()) {
        return keyval->second;
    }

    unsigned int index = glGetUniformBlockIndex(id, uname.c_str());
    DEBUG("Inserting uniform block '%s' with index %d into the hashtable of program %u", uname.c_str(), index, id);
    uniformsBlockIndices[uname] = index;
    return index;

}
void ShaderProgram::setUniformBlockBinding(const std::string& uname, unsigned int binding){
    unsigned int index = getUniformBlockIndex(uname);
    if (index != GL_INVALID_INDEX) {
        DEBUG("Set uniform block binding of program %u : index = %u, binding = %u", id, index, binding);
        glUniformBlockBinding(id, index, binding);
    }

}

void ShaderProgram::setUniformMat4f(const std::string& uname, float const* const mat, bool transpose) {
    int loc = getUniformLocation(uname);
    if (loc >= 0) glUniformMatrix4fv(loc, 1, transpose, mat);
    
}

void ShaderProgram::setUniform1f(const std::string& uname, float v0){
    int loc = getUniformLocation(uname);
    if (loc >= 0) glUniform1f(loc, v0);
}

void ShaderProgram::setUniform2f(const std::string& uname, float v0, float v1){
    int loc = getUniformLocation(uname);
    if (loc >= 0) glUniform2f(loc, v0, v1);
}
void ShaderProgram::setUniform1b(const std::string& uname, bool v0){
    int loc = getUniformLocation(uname);
    if (loc >= 0) glUniform1i(loc, v0);
}
