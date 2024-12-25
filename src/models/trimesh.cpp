#include "models/model.hpp"
#include "models/trimesh.hpp"
#include "log.h"

TriMesh::TriMesh(){}

TriMesh::TriMesh(int const _dim, std::vector<float>& _nodeCoords, std::vector<int>& _triangles) : dim(_dim), nodeCoords(_nodeCoords), elem(_triangles) {

    vao.bind();

    VBO* positions = new VBO;
    positions->bind();
    positions->setData(nodeCoords.size()*sizeof(_nodeCoords[0]), nodeCoords.data());
    bufIndices[MESH_POS] = vao.attachBuffer(positions);

    EBO* indices = new EBO;
    indices->bind();
    indices->setData(sizeof(elem[0])*elem.size(), elem.data());
    indices->setAttribute(0, dim, BUF_FLOAT, GL_FALSE, 0, 0);
    bufIndices[MESH_IDX] = vao.attachBuffer(indices);
    vao.enableAttribute(0);
}

TriMesh::TriMesh(int const dim, std::vector<float>& _nodeCoords, std::vector<int>& _triangles, uint8_t R, uint8_t G, uint8_t B) : TriMesh(dim, _nodeCoords, _triangles) {
    colors = std::vector<uint8_t>({R, G, B});
    colType = COLOR_CONSTANT;

    prog->use();
    glVertexAttrib3f(1, R/255.f, G/255.f, B/255.f);
}

TriMesh::TriMesh(int const dim, std::vector<float>& _nodeCoords, std::vector<int>& _triangles, std::vector<uint8_t>& _colors) : TriMesh(dim, _nodeCoords, _triangles) {
    colors = _colors;
    colType = COLOR_NODE;

    vao.bind();
    VBO* col = new VBO;
    col->bind();
    col->setData(colors.size()*sizeof(colors[0]), colors.data());
    col->setAttribute(1, 3, BUF_UBYTE, true, 0, 0);
    bufIndices[MESH_COL] = addBuffer(col);
    vao.enableAttribute(1);
}

void TriMesh::moveNodes(std::vector<float>& dx){
    for (size_t i = 0; i < dx.size(); i++) nodeCoords[i] += dx[i];

    setBuffer(MESH_POS, nodeCoords.size()*sizeof(nodeCoords[0]), nodeCoords.data());
}

void TriMesh::setNodes(std::vector<float>& newNodes){
    nodeCoords = newNodes;

    setBuffer(MESH_POS, nodeCoords.size()*sizeof(nodeCoords[0]), nodeCoords.data());
}

void TriMesh::setColors(uint8_t R, uint8_t G, uint8_t B) {
    if (colType != COLOR_CONSTANT) {
        fprintf(stderr, "Cannot change node-coloring type. You are trying to set a constant color on a TriMesh that uses a different node coloring.\n");
        return;
    }
    colors[0] = R;
    colors[1] = G;
    colors[2] = B;

    prog->use();
    glVertexAttrib3f(1, R/255.f, G/255.f, B/255.f);
}

void TriMesh::setColors(std::vector<uint8_t>& _colors){
    if (colType != COLOR_NODE) {
        fprintf(stderr, "Cannot change node-coloring type. You are trying to set a per-node color on a TriMesh that uses a different node coloring.\n");
        return;
    }

    colors = _colors;
    setBuffer(MESH_COL, colors.size()*sizeof(colors[0]), colors.data());
}

void TriMesh::draw() const {
    vao.bind();
    prog->use();

    glDrawElements(GL_TRIANGLES, elem.size(), BUF_UINT, 0);
}
