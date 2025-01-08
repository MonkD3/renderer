#include "models/model.hpp"
#include "buffer.hpp"
#include "glad/gl.h"
#include "log.h"
#include "shader.hpp"
#include <vector>

Model::Model() {
    DEBUG("Initializing new model");
    vshd = new Shader(CMAKE_HOME_DIRECTORY "/assets/vertex/pos_col.vert", SHADER_VERTEX);
    fshd = new Shader(CMAKE_HOME_DIRECTORY "/assets/fragment/const_col.frag", SHADER_FRAGMENT);

    // Attribute 1 is the color : set a generic white color
    glVertexAttrib3f(1, 1.0f, 1.0f, 1.0f);

    prog = new ShaderProgram();
    prog->attachShader(vshd);
    prog->attachShader(fshd);
    prog->compile();

    // Reserve 16 spots for the renderer. 
    // Indices greater or equal to 16 are considered user-managed
    bufIndices.reserve(MODEL_USER_START);
    for (int i = 0; i < MODEL_USER_START; i++) bufIndices[i] = -1;
}

Model::~Model() {
    DEBUG("Deleting model");
    delete vshd;
    delete fshd;
    delete prog;

    for (int i = 0; i < MODEL_USER_START; i++){
        int idx = bufIndices[i];
        if (idx >= 0) {
            DEBUG("Deleting model buffer %u at index %u", idx, i);
            delete vao.buffers[idx];
        }
    }
}

void Model::setBuffer(unsigned int const bufIndex, size_t const bufSize, void const* bufData) {
    DEBUG("Setting data of model buffer %u at index %u", bufIndices[bufIndex], bufIndex);
    Buffer* buf = vao.buffers[bufIndices[bufIndex]];
    buf->bind();
    buf->setData(bufSize, bufData);
}

unsigned int Model::addBuffer(Buffer* buf){
    vao.bind();
    int idx = vao.attachBuffer(buf);
    bufIndices.push_back(idx);
    DEBUG("Adding model buffer %u at index %lu", idx, bufIndices.size()-1);
    return bufIndices.size() - 1;
}

void Model::draw() const { }
