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

void Buffer::setSubData(size_t start, size_t bufsize, void const* buf) const {
    DEBUG("Set subdata of buffer %u [type = %u]. data-size=%zu, offset = %zu", id, buftype, bufsize, start);
    glBufferSubData(buftype, (GLsizeiptr) start, (GLsizeiptr) bufsize, buf);
}

VBO::VBO() {
    buftype = GL_ARRAY_BUFFER;
};

EBO::EBO(){
    buftype = GL_ELEMENT_ARRAY_BUFFER;
};

UBO::UBO(){
    buftype = GL_UNIFORM_BUFFER;
};

void UBO::bindUniformRange(int index, int offset, size_t size) const {
    DEBUG("Bind uniform range of index %d on program %u. offset = %d, size=%zu", index, id, offset, size);
    glBindBufferRange(buftype, index, id, offset, size);
}
