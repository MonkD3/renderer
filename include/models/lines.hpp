#pragma once 

#include "models/model.hpp"
#include "buffer.hpp"

struct Lines : public Model {
    int dim;
    size_t nElems;

    unsigned char color[3];
    Lines();

    // Triangular meshes with nodes coordinates @_nodeCoords and elements @_elem. 
    // Rendering uses default shaders.
    Lines(int const dim, std::vector<float>& _nodeCoords, std::vector<int>& _lines);
    Lines(int const _dim, VBO* nodeCoords, EBO* lines);

    ~Lines() = default;

    void initShaderProgram();

    // Set nodeCoords[i] = newCoords[i]
    void setNodes(std::vector<float>& newNodes);

    // Set single color for all nodes
    void setColor(uint8_t R, uint8_t G, uint8_t B);

    void draw() const;
};
