#pragma once

#include "array.hpp"
#include "buffer.hpp"
#include "shader.hpp"
#include "uniforms.hpp"
#include <vector>

enum colorType {
    COLOR_DEFAULT,
    COLOR_NODE,
    COLOR_CONSTANT,
    COLOR_CMAP,
    COLOR_USER,
};

enum modelBufType {
    MODEL_POS = 0,   // Index of position buffer
    MODEL_IDX = 1,   // Index of indices buffer in case of EBO
    MODEL_COL = 2,   // Index of color buffer
    MODEL_SIZE = 3,  // Index of "size" buffer : any single float 
                     // that is supposed to give the width/radius... 
                     // of the primitive
    MODEL_NORMAL = 4, // Index of normal buffer (storing normals to the vertex)
    MODEL_USER_START = 16,  // Special value : Starting index of user-defined buffers
};

/**
 *  Defines a generic model that should be extended to provide "renderable" objects
 */
struct Model {
    // Vertex Array Object describing the layout of the underlying buffers
    VAO vao;    
    
    // Gives the index of an attached buffer in the VAO
    // Allows to differentiate between renderer and user-managed 
    // buffers
    std::vector<int> bufIndices;

    Shader* vshd; // Vertex shader
    Shader* fshd; // Fragment shader
    ShaderProgram* prog; // Shader program to use for rendering the model. Default is constant white.

    Model();
    ~Model();

    void setVertexShader(const Shader* vertexShader);
    void setFragmentShader(const Shader* vertexShader);
    void setShaderProgram(const ShaderProgram* program);

    // Add a user-defined buffer to the model. 
    // @return : the ID of the buffer in this model.
    // @param buf : the user-defined buffer to attach
    unsigned int addBuffer(Buffer* buf);

    // Modify the content of a buffer attached to the model.
    // @param bufIndex : the ID of the model buffer
    // @param bufSize : the size of the data (in bytes)
    // @param bufData : a pointer to the data
    void setBuffer(unsigned int const bufIndex, size_t const bufSize, void const* bufData);

    virtual void draw() const;
};

