#pragma once 

#include "colormaps.hpp"
#include "models/model.hpp"

struct TriMesh : public Model {
    int dim;
    size_t nElems;

    ColorMap cmap;
    // Color of nodes
    colorType colType;           // Type of node-coloring : 
                                 //   - COLOR_CONSTANT : each node has the same color (only supply R, G, B)
                                 //   - COLOR_NODE : each node has a separate color (supply color for each node)
    TriMesh();

    // Triangular meshes with nodes coordinates @_nodeCoords and elements @_elem. 
    // Rendering uses default shaders.
    TriMesh(int const dim, std::vector<float>& _nodeCoords, std::vector<int>& _triangles);

    ~TriMesh() = default;

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
