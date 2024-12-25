#pragma once 

#include "models/trimesh.hpp"

struct PolyMesh : public TriMesh {
    std::vector<int>   startElem; // Index of the start of i-th element in @elem

    std::vector<int> triElem; // Processed @elem into an array of triangular elements
    
    PolyMesh();
    PolyMesh(std::vector<float>& _nodeCoords, std::vector<int>& _elem, std::vector<int>& _startElem);

    void draw() const;
};
