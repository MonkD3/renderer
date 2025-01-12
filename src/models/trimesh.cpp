#include "glad/gl.h"
#include "models/model.hpp"
#include "models/trimesh.hpp"
#include "log.h"

TriMesh::TriMesh(){}

TriMesh::TriMesh(int const _dim, std::vector<float>& _nodeCoords, std::vector<int>& _triangles) : dim(_dim), nodeCoords(_nodeCoords), elem(_triangles) {

    vao.bind();

    VBO* positions = new VBO;
    positions->bind();
    positions->setData(nodeCoords.size()*sizeof(_nodeCoords[0]), nodeCoords.data());
    bufIndices[MODEL_POS] = vao.attachBuffer(positions);

    EBO* indices = new EBO;
    indices->bind();
    indices->setData(sizeof(elem[0])*elem.size(), elem.data());
    vao.setAttribute(0, dim, BUF_FLOAT, GL_FALSE, 0, 0);
    bufIndices[MODEL_IDX] = vao.attachBuffer(indices);
    vao.enableAttribute(0);

    // Default color is set in Model constructor
    colType = COLOR_DEFAULT;
}

void TriMesh::moveNodes(std::vector<float>& dx){
    for (size_t i = 0; i < dx.size(); i++) nodeCoords[i] += dx[i];

    setBuffer(MODEL_POS, nodeCoords.size()*sizeof(nodeCoords[0]), nodeCoords.data());
}

void TriMesh::setNodes(std::vector<float>& newNodes){
    nodeCoords = newNodes;

    setBuffer(MODEL_POS, nodeCoords.size()*sizeof(nodeCoords[0]), nodeCoords.data());
}

void TriMesh::setColor(uint8_t R, uint8_t G, uint8_t B) {
    colors = std::vector<uint8_t>({R, G, B});
    vao.bind();

    if (colType != COLOR_CONSTANT) {
        DEBUG("Changing node-coloring type of trimesh to COLOR_CONSTANT");
        colType = COLOR_CONSTANT;
        vao.disableAttribute(1);
    }

    prog->use();
    glVertexAttrib3f(1, R/255.f, G/255.f, B/255.f);
}

void TriMesh::setColors(std::vector<uint8_t>& _colors){
    colors = _colors;
    vao.bind();
    if (bufIndices[MODEL_COL] >= 0){
        setBuffer(MODEL_COL, colors.size()*sizeof(colors[0]), colors.data());
    } else {
        VBO* col = new VBO;
        col->bind();
        col->setData(colors.size()*sizeof(colors[0]), colors.data());
        vao.setAttribute(1, 3, BUF_UBYTE, true, 0, 0);
        bufIndices[MODEL_COL] = vao.attachBuffer(col);
    }

    if (colType != COLOR_NODE) {
        DEBUG("Changing node-coloring type of trimesh to COLOR_NODE");
        colType = COLOR_NODE;
        vao.enableAttribute(1);
    }
}

void TriMesh::draw() const {
    DEBUG("Drawing trimesh with VAO %u", vao.id);
    vao.bind();
    prog->use();

    glDrawElements(GL_TRIANGLES, elem.size(), BUF_UINT, 0);
}
