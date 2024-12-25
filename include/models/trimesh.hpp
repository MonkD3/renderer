#pragma once 

#include "models/model.hpp"

struct TriMesh : public Model {
    int dim;
    std::vector<float> nodeCoords;  // Coordinates of nodes
    std::vector<int>   elem;        // Indices of nodes forming an element

    // Color of nodes
    colorType colType;           // Type of node-coloring : 
                                 //   - COLOR_CONSTANT : each node has the same color (only supply R, G, B)
                                 //   - COLOR_NODE : each node has a separate color (supply color for each node)
    std::vector<uint8_t> colors; // Vector of colors, memory layout depends on the value of @colType
                                  
    TriMesh();

    // Triangular meshes with nodes coordinates @_nodeCoords and elements @_elem. 
    // Rendering uses default shaders.
    TriMesh(int const dim, std::vector<float>& _nodeCoords, std::vector<int>& _triangles);

    // Triangular meshes with nodes coordinates @_nodeCoords and elements @_elem. 
    // Rendering uses uniform color shaders
    TriMesh(int const dim, std::vector<float>& _nodeCoords, std::vector<int>& _triangles, uint8_t R, uint8_t G, uint8_t B);

    // Triangular meshes with nodes coordinates @_nodeCoords and elements @_elem. 
    // Rendering uses per node color shaders
    TriMesh(int const dim, std::vector<float>& _nodeCoords, std::vector<int>& _triangles, std::vector<uint8_t>& _colors);

    ~TriMesh() = default;

    // Computes nodeCoords[i] += dx[i]
    void moveNodes(std::vector<float>& dx);

    // Set nodeCoords[i] = newCoords[i]
    void setNodes(std::vector<float>& newNodes);

    void setColors(uint8_t R, uint8_t G, uint8_t B);
    void setColors(std::vector<uint8_t>& _colors);

    void draw() const;
};
