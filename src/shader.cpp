#include <shader.hpp>
#include <log.h>
#include <cstdio>

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

void ShaderProgram::attach_shader(Shader* s){
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
