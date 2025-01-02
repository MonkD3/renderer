#pragma once

#include "buffer.hpp"
#include <vector>

    
struct VAO {
    unsigned int id;
    std::vector<Buffer*> buffers;

    VAO();
    ~VAO();

    void bind() const;

    // Attach a Buffer to the Vertex Array and return its
    // index to the user
    unsigned int attachBuffer(Buffer* buf);
    void enableAttribute(unsigned int loc) const;
    void disableAttribute(unsigned int loc) const;
};
