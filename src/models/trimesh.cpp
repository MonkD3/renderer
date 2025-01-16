#include "glad/gl.h"
#include "models/model.hpp"
#include "models/trimesh.hpp"
#include "log.h"

TriMesh::TriMesh(){}

TriMesh::TriMesh(int const _dim, std::vector<float>& nodeCoords, std::vector<int>& triangles) : dim(_dim), nElems(triangles.size()) {

    vao.bind();

    VBO* positions = new VBO;
    positions->bind();
    positions->setData(nodeCoords.size()*sizeof(nodeCoords[0]), nodeCoords.data());
    bufIndices[MODEL_POS] = vao.attachBuffer(positions);

    EBO* indices = new EBO;
    indices->bind();
    indices->setData(sizeof(triangles[0])*triangles.size(), triangles.data());
    vao.setAttribute(0, dim, GL_FLOAT, GL_FALSE, 0, 0);
    bufIndices[MODEL_IDX] = vao.attachBuffer(indices);
    vao.enableAttribute(0);

    // Default color is set in Model constructor
    colType = COLOR_DEFAULT;
}

void TriMesh::setNodes(std::vector<float>& newNodes){
    setBuffer(MODEL_POS, newNodes.size()*sizeof(newNodes[0]), newNodes.data());
}

void TriMesh::setColor(uint8_t R, uint8_t G, uint8_t B) {
    vao.bind();

    if (colType != COLOR_CONSTANT) {
        DEBUG("Changing node-coloring type of trimesh to COLOR_CONSTANT");
        vao.disableAttribute(1);
        if (colType == COLOR_CMAP){
            prog->use();
            prog->setUniform1b("useCmap", false);
        }
        colType = COLOR_CONSTANT;
    }

    vao.setDefaultAttributeValues3f(1, R/255.f, G/255.f, B/255.f);
}

void TriMesh::setColor(std::vector<uint8_t>& colors){
    vao.bind();
    if (bufIndices[MODEL_COL] < 0){
        VBO* col = new VBO;
        bufIndices[MODEL_COL] = vao.attachBuffer(col);
    }     

    setBuffer(MODEL_COL, colors.size()*sizeof(colors[0]), colors.data());

    if (colType != COLOR_NODE) {
        DEBUG("Changing node-coloring type of trimesh to COLOR_NODE");
        vao.setAttribute(1, 3, GL_UNSIGNED_BYTE, true, 0, 0);
        if (colType == COLOR_CMAP){
            prog->use();
            prog->setUniform1b("useCmap", false);
        }
        vao.enableAttribute(1);
        colType = COLOR_NODE;
    }
}

void TriMesh::useCmap(ColorMap& _cmap) {
    vao.bind();
    cmap = _cmap;
    if (colType != COLOR_CMAP) {
        if (colType == COLOR_DEFAULT || colType == COLOR_CONSTANT){
            vao.enableAttribute(1);
        }
        prog->use();
        prog->setUniform1b("useCmap", true);
        prog->setUniform2f("cmapRange", cmap.cmin, cmap.cmax);
        colType = COLOR_CMAP;
    }
}

void TriMesh::setField(std::vector<float>& fieldValue){
    if (colType != COLOR_CMAP) return;

    vao.bind();
    if (bufIndices[MODEL_COL] >= 0){
        VBO* col = new VBO;
        bufIndices[MODEL_COL] = vao.attachBuffer(col);
    } 

    setBuffer(MODEL_COL, fieldValue.size()*sizeof(fieldValue[0]), fieldValue.data());
    vao.setAttribute(1, 1, GL_FLOAT, false, 0, 0);
}


void TriMesh::draw() const {
    DEBUG("Drawing trimesh with VAO %u", vao.id);
    vao.bind();
    prog->use();
    if (colType == COLOR_CMAP) cmap.bind();

    glDrawElements(GL_TRIANGLES, nElems, GL_UNSIGNED_INT, 0);
}
