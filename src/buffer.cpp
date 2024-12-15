#include "buffer.hpp"
#include "glad/gl.h"
#include "log.h"
#include <cstdio>

Buffer::Buffer() : buftype(0), size(0) {
    glGenBuffers(1, &id);
    DEBUG("Creating buffer (id=%u)", id);
};

Buffer::~Buffer() {
    DEBUG("Deleting buffer (id=%u)", id);
    glDeleteBuffers(1, &id);
};

void Buffer::bind() const {
    DEBUG("Binding buffer : id=%u, buftype = %u", id, buftype);
    glBindBuffer(buftype, id);
};

void Buffer::setData(size_t const bufsize, void const* buf){
    size = bufsize;
    DEBUG("Setting buffer data : id=%u, buftype=%u, size=%zu", id, buftype, size);
    glBufferData(buftype, (GLsizeiptr) bufsize, buf, GL_DYNAMIC_DRAW);
}

void Buffer::setAttribute(BufferAttribute& ba){
    buflayout.push_back(ba);
}

void Buffer::setAttribute(unsigned int loc, int nmemb, BufType bt, bool normalized, size_t stride, void* offset){
    DEBUG("Adding attribute %u to buffer %u", loc, id);
    BufferAttribute ba{
        .loc = loc,
        .nmemb = nmemb,
        .bt = bt,
        .normalized = normalized,
        .stride = stride,
        .offset = offset
    };
    setAttribute(ba);
}

VBO::VBO() {
    buftype = GL_ARRAY_BUFFER;
};
EBO::EBO(){
    buftype = GL_ELEMENT_ARRAY_BUFFER;
};
