#include "buffer.hpp"
#include "glad/gl.h"
#include "log.h"
#include <array.hpp>

VAO::VAO() {
    glGenVertexArrays(1, &id);
    RENDERER_DEBUG("Created vertex array %u", id);
}

VAO::~VAO() {
    RENDERER_DEBUG("Deleted vertex array %u", id);
    glDeleteVertexArrays(1, &id);
}

void VAO::bind() const {
    RENDERER_DEBUG("Bind vertex array %u", id);
    glBindVertexArray(id);
}

unsigned int VAO::attachBuffer(Buffer* buf){
    RENDERER_DEBUG("Attach buffer %u to array %u", buf->id, id);
    buffers.push_back(buf);
    return buffers.size() - 1;
}

void VAO::setAttribute(unsigned int loc, int nmemb, BufType bt, bool normalized, size_t stride, void* offset){
    bind();
    RENDERER_DEBUG("Add attribute [loc=%u,nmemb=%d,bt=%d,normalized=%d,stride=%zu] to vao %u", loc, nmemb, bt, normalized, stride, id);
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
    RENDERER_DEBUG("Enable attribute %u of vertex array %u", loc, id);
    glEnableVertexAttribArray(loc);
}

void VAO::disableAttribute(unsigned int loc) const {
    RENDERER_DEBUG("Disable attribute %u of vertex array %u", loc, id);
    glDisableVertexAttribArray(loc);
}

void VAO::setDefaultAttributeValues3f(unsigned int loc, float v0, float v1, float v2) const {
    bind();
    glVertexAttrib3f(loc, v0, v1, v2);
}

void VAO::setDefaultAttributeValues4f(unsigned int loc, float v0, float v1, float v2, float v3) const {
    bind();
    glVertexAttrib4f(loc, v0, v1, v2, v3);
}
