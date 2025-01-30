#include "models/model.hpp"
#include "buffer.hpp"
#include "glad/gl.h"
#include "log.h"
#include "shader.hpp"
#include <vector>

Model::Model() {
    RENDERER_DEBUG("Initializing new model");

    // Reserve 16 spots for the renderer. 
    // Indices greater or equal to 16 are considered user-managed
    bufIndices.reserve(MODEL_USER_START);
    for (int i = 0; i < MODEL_USER_START; i++) bufIndices[i] = -1;
}

Model::~Model() {
    RENDERER_DEBUG("Deleting model");
    delete vshd;
    delete fshd;
    delete prog;

    for (int i = 0; i < MODEL_USER_START; i++){
        int idx = bufIndices[i];
        if (idx >= 0) {
            RENDERER_DEBUG("Deleting model buffer %u at index %u", idx, i);
            delete vao.buffers[idx];
        }
    }
}

void Model::setBuffer(unsigned int const bufIndex, size_t const bufSize, void const* bufData) {
    RENDERER_DEBUG("Setting data of model buffer %u at index %u", bufIndices[bufIndex], bufIndex);
    Buffer* buf = vao.buffers[bufIndices[bufIndex]];
    buf->bind();
    buf->setData(bufSize, bufData);
}

unsigned int Model::addBuffer(Buffer* buf){
    vao.bind();
    int idx = vao.attachBuffer(buf);
    bufIndices.push_back(idx);
    RENDERER_DEBUG("Adding model buffer %u at index %lu", idx, bufIndices.size()-1);
    return bufIndices.size() - 1;
}

void Model::draw() const { }
