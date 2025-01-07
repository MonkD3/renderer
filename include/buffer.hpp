#pragma once

#include "glad/gl.h"
#include <cstddef>
#include <vector>

enum BufType {
    BUF_INT   = GL_INT,
    BUF_UINT  = GL_UNSIGNED_INT,
    BUF_BYTE  = GL_BYTE,
    BUF_UBYTE = GL_UNSIGNED_BYTE,
    BUF_SHORT = GL_SHORT,
    BUF_USHORT = GL_UNSIGNED_SHORT,
    BUF_FLOAT = GL_FLOAT,
    BUF_MAT2  = GL_FLOAT_MAT2,
    BUF_MAT3  = GL_FLOAT_MAT3,
    BUF_MAT4  = GL_FLOAT_MAT4,
};

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
    std::vector<BufferAttribute> buflayout; // Layout of each attribute in the buffer

    Buffer();
    ~Buffer();

    void bind() const; // Bind the buffer
    
    // Change the data inside the buffer
    void setData(size_t const bufsize, void const* buf);

    // Specify the layout of the attribute's data inside the buffer
    void setAttribute(unsigned int loc, int nmemb, BufType bt, bool normalized, size_t stride, void* offset);
    // Specify the layout of the attribute's data inside the buffer
    void setAttribute(BufferAttribute& ba);
};

struct VBO : public Buffer { 
    VBO();
};

struct EBO : public Buffer { 
    EBO();
};
