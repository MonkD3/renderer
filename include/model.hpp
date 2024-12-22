#pragma once

#include "array.hpp"
#include "shader.hpp"
#include <vector>

struct Model {
    VAO vao;    // Vertex Array Object describing the layout of the underlying buffers
    VBO positions;
    EBO indices;
    Shader* vshd;
    Shader* fshd;
    ShaderProgram* prog; // Shader program to use for rendering the model. Default is constant white.
    
    Model();
    ~Model();

    void setVertexShader(const Shader* vertexShader);
    void setFragmentShader(const Shader* vertexShader);
    void setShaderProgram(const ShaderProgram* program);

    void draw() const;
};

struct TriMesh : public Model {
    int dim;
    std::vector<float> nodeCoords;    // Coordinates of nodes
    std::vector<int>   elem;      // Indices of nodes forming an element
                                  
    TriMesh();
    TriMesh(int const dim, std::vector<float>& _nodeCoords, std::vector<int>& _triangles);
    ~TriMesh() = default;

    // Computes nodeCoords[i] += dx[i]
    void moveNodes(std::vector<float>& dx);

    // Set nodeCoords[i] = newCoords[i]
    void setNodes(std::vector<float>& newNodes);

    void draw() const;
};

struct PolyMesh : public TriMesh {
    std::vector<int>   startElem; // Index of the start of i-th element in @elem

    std::vector<int> triElem; // Processed @elem into an array of triangular elements
    
    PolyMesh();
    PolyMesh(std::vector<float>& _nodeCoords, std::vector<int>& _elem, std::vector<int>& _startElem);

    void draw() const;
};
