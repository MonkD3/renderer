#pragma once

#include "buffer.hpp"
#include <vector>

    
struct VAO {
    unsigned int id;  // OpenGL id
    std::vector<Buffer*> buffers; // attached buffers

    VAO();
    ~VAO();

    void bind() const; // Bind the vao

    // Attach a Buffer to the Vertex Array and return its
    // index to the user
    unsigned int attachBuffer(Buffer* buf);

    // Enable an attribute
    void enableAttribute(unsigned int loc) const;

    // Disable an attribute
    void disableAttribute(unsigned int loc) const;
};
