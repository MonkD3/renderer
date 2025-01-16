#pragma once

#include "glad/gl.h"
#include <cstddef>
#include <vector>

typedef GLenum BufType;

struct BufferAttribute {
    unsigned int loc; // Location of the attribute in the shader
    int nmemb;        // Number of members
    BufType bt;       // Type of data
    bool normalized;  // Is it normalized ?
    size_t stride;    // Number of bytes between two elements
    void* offset;    // Offset in the buffer
};
    
struct Buffer {
    unsigned int id; // OpenGL buffer ID
    unsigned int buftype; // OpenGL buffer type (GL_ARRAY_BUFFER, ...)
    size_t size; // Size of the data in the buffer (in bytes)

    Buffer();
    ~Buffer();

    void bind() const; // Bind the buffer
    
    // Change the data inside the buffer
    void setData(size_t const bufsize, void const* buf);
    void setSubData(size_t start, size_t const bufsize, void const* buf) const;
};

struct VBO : public Buffer { 
    VBO();
};

struct EBO : public Buffer { 
    EBO();
};

struct UBO : public Buffer {
    UBO();

    // Wrapper around glBindBufferRange
    void bindUniformRange(int index, int offset, size_t size) const;
};
