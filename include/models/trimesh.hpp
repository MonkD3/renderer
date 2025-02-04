#pragma once 

#include "buffer.hpp"
#include "colormaps.hpp"
#include "models/model.hpp"

struct TriMesh : public Model {
    int dim;
    size_t nElems;

    ColorMap cmap;
    colorType colType;     
    unsigned char col[3]; // In case the color is constant, cache the value 
                          // to feed it to glVertexAttrib at each draw call

    TriMesh();

    // Triangular meshes with nodes coordinates @_nodeCoords and elements @_elem. 
    // Rendering uses default shaders.
    TriMesh(int const dim, std::vector<float>& _nodeCoords, std::vector<int>& _triangles);
    TriMesh(int const dim, VBO* _nodeCoords, EBO* _triangles);
    
    void initShaderProgram();

    // Set nodeCoords[i] = newCoords[i]
    void setNodes(std::vector<float>& newNodes);

    // Set single color for all nodes
    void setColor(uint8_t R, uint8_t G, uint8_t B);
    // Set a color for each node, triangle color
    // is interpolated between its nodes
    void setColor(std::vector<uint8_t>& _colors);
    void useCmap(ColorMap& _cmap);
    void setField(std::vector<float>& fieldValue);

    void draw() const;
};
