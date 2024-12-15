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
    unsigned int id;
    unsigned int buftype;
    size_t size;
    std::vector<BufferAttribute> buflayout;

    Buffer();
    ~Buffer();

    void bind() const;
    void setData(size_t const bufsize, void const* buf);

    void setAttribute(unsigned int loc, int nmemb, BufType bt, bool normalized, size_t stride, void* offset);
    void setAttribute(BufferAttribute& ba);
};

struct VBO : public Buffer { 
    VBO();
};

struct EBO : public Buffer { 
    EBO();
};
