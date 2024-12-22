#include "buffer.hpp"
#include "log.h"
#include <array.hpp>


VAO::VAO() {
    glGenVertexArrays(1, &id);
    DEBUG("Created vertex array %u", id);
}

VAO::~VAO() {
    DEBUG("Deleted vertex array %u", id);
    glDeleteVertexArrays(1, &id);
}

void VAO::bind() const {
    DEBUG("Bind vertex array %u", id);
    glBindVertexArray(id);
}

int VAO::attachBuffer(Buffer* buf){
    for (BufferAttribute& ba : buf->buflayout){
        DEBUG("Adding attribute %u from buffer %u to array %u", ba.loc, buf->id, id);
        glVertexAttribPointer(
            ba.loc,
            ba.nmemb, 
            ba.bt,
            ba.normalized,
            ba.stride, 
            ba.offset
        );
    }

    buffers.push_back(buf);
    return buffers.size() - 1;
}

void VAO::enableAttribute(unsigned int loc) const {
    glEnableVertexAttribArray(loc);
}
