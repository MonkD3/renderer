#include "buffer.hpp"
#include "glad/gl.h"
#include "log.h"
#include <cstdio>

Buffer::Buffer() : buftype(0), size(0) {
    glGenBuffers(1, &id);
    DEBUG("Created buffer %u", id);
};

Buffer::~Buffer() {
    DEBUG("Deleted buffer %u", id);
    glDeleteBuffers(1, &id);
};

void Buffer::bind() const {
    DEBUG("Bind buffer %u [type = %u]", id, buftype);
    glBindBuffer(buftype, id);
};

void Buffer::setData(size_t const bufsize, void const* buf){
    DEBUG("Set data of buffer %u [type = %u]. data-size=%zu", id, buftype, bufsize);
    size = bufsize;
    glBufferData(buftype, (GLsizeiptr) bufsize, buf, GL_DYNAMIC_DRAW);
}

void Buffer::setAttribute(BufferAttribute& ba){
    buflayout.push_back(ba);
}

void Buffer::setAttribute(unsigned int loc, int nmemb, BufType bt, bool normalized, size_t stride, void* offset){
    DEBUG("Add attribute %u to buffer %u [type = %u]", loc, id, buftype);
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
