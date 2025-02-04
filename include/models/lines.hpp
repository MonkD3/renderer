#pragma once 

#include "models/model.hpp"
#include "buffer.hpp"

struct Lines : public Model {
    int dim;
    size_t nElems;

    colorType colType;
    unsigned char color[3];


    Lines();

    // Triangular meshes with nodes coordinates @_nodeCoords and elements @_elem. 
    // Rendering uses default shaders.
    Lines(int const dim, std::vector<float>& _nodeCoords, std::vector<int>& _lines);
    Lines(int const _dim, VBO* nodeCoords, EBO* lines);

    void initShaderProgram();

    // Set nodeCoords[i] = newCoords[i]
    void setNodes(std::vector<float>& newNodes);
    void setIndices(std::vector<int>& newLines);

    // Set single color for all nodes
    void setColor(uint8_t R, uint8_t G, uint8_t B);

    void draw() const;
};
