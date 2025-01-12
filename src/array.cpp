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

unsigned int VAO::attachBuffer(Buffer* buf){
    DEBUG("Attach buffer %u to array %u", buf->id, id);
    buffers.push_back(buf);
    return buffers.size() - 1;
}

void VAO::setAttribute(unsigned int loc, int nmemb, BufType bt, bool normalized, size_t stride, void* offset){
    bind();
    DEBUG("Add attribute [loc=%u,nmemb=%d,bt=%d,normalized=%d,stride=%zu] to vao %u", loc, nmemb, bt, normalized, stride, id);
    glVertexAttribPointer(
        loc,
        nmemb, 
        bt,
        normalized,
        stride, 
        offset
    );
}

void VAO::enableAttribute(unsigned int loc) const {
    DEBUG("Enable attribute %u of vertex array %u", loc, id);
    glEnableVertexAttribArray(loc);
}

void VAO::disableAttribute(unsigned int loc) const {
    DEBUG("Disable attribute %u of vertex array %u", loc, id);
    glDisableVertexAttribArray(loc);
}
