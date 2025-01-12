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

    /* Specify the layout of the attribute's data stored in 
     * various buffers
     * @loc : the location of the attribute (as in the shader)
     * @nmemb : the number of values per vertex 
     * @bt : the data-type
     * @normalize : should we normalize the data (i.e. transform 
     *              it to floats)
     * @stride : Number of bytes between two consecutive attributes
     * @offset : Number of bytes to offset before starting to read the attribute
     *
     * See : glVertexAttribPointer
     */
    void setAttribute(unsigned int loc, int nmemb, BufType bt, bool normalize, size_t stride, void* offset);

    /* Enable an attribute
     * @loc : the location of the attribute to enable (as in the shader)
     *
     * See : glEnableVertexAttribArray
     */
    void enableAttribute(unsigned int loc) const;

    /* Disable an attribute
     * @loc : the location of the attribute to disable (as in the shader)
     *
     * See : glDisableVertexAttribArray
     */
    void disableAttribute(unsigned int loc) const;

    /* Set default input values of disabled locations in shader
     * @loc : the location of the attribute
     * @v0 : value of the first component
     * @v1 : value of the second component
     * @v2 : value of the third component
     *
     * See : glVertexAttrib3f
     */
    void setDefaultAttributeValues3f(unsigned int loc, float v0, float v1, float v2) const;

    void setDefaultAttributeValues4f(unsigned int loc, float v0, float v1, float v2, float v3) const;
};
