#include "glad/gl.h"
#include "models/model.hpp"
#include "models/trimesh.hpp"
#include "log.h"

void TriMesh::initShaderProgram(){
    vshd = new Shader(ASSET_DIR "vertex/defaultVertex.vert", SHADER_VERTEX);
    fshd = new Shader(ASSET_DIR "fragment/defaultFrag.frag", SHADER_FRAGMENT);

    prog = new ShaderProgram();
    prog->attachShader(vshd);
    prog->attachShader(fshd);
    prog->compile();

    vao.bind();
    colType = COLOR_DEFAULT;
}

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
    vao.enableAttribute(MODEL_POS);

    initShaderProgram();
}

TriMesh::TriMesh(int const _dim, VBO* nodeCoords, EBO* triangles) : dim(_dim) {
    vao.bind();

    nodeCoords->bind();
    bufIndices[MODEL_POS] = vao.attachBuffer(nodeCoords);

    triangles->bind();
    nElems = triangles->size / sizeof(int);
    vao.setAttribute(0, dim, GL_FLOAT, GL_FALSE, 0, 0);
    bufIndices[MODEL_IDX] = vao.attachBuffer(triangles);
    vao.enableAttribute(MODEL_POS);

    initShaderProgram();
}

void TriMesh::setNodes(std::vector<float>& newNodes){
    setBuffer(MODEL_POS, newNodes.size()*sizeof(newNodes[0]), newNodes.data());
}

void TriMesh::setIndices(std::vector<int>& newTri){
    setBuffer(MODEL_IDX, newTri.size()*sizeof(newTri[0]), newTri.data());
    nElems = newTri.size();
}

void TriMesh::setColor(uint8_t R, uint8_t G, uint8_t B) {
    vao.bind();

    if (colType != COLOR_CONSTANT) {
        RENDERER_DEBUG("Changing node-coloring type of trimesh to COLOR_CONSTANT");
        vao.disableAttribute(MODEL_COL);
        if (colType == COLOR_CMAP){
            prog->use();
            prog->setUniform1b("useCmap", false);
        }
        colType = COLOR_CONSTANT;
    }

    col[0] = R;
    col[1] = G;
    col[2] = B;
}

void TriMesh::setColor(std::vector<uint8_t>& colors){
    vao.bind();
    if (bufIndices[MODEL_COL] < 0){
        VBO* col = new VBO;
        bufIndices[MODEL_COL] = vao.attachBuffer(col);
    }     

    setBuffer(MODEL_COL, colors.size()*sizeof(colors[0]), colors.data());

    if (colType != COLOR_NODE) {
        RENDERER_DEBUG("Changing node-coloring type of trimesh to COLOR_NODE");
        vao.setAttribute(MODEL_COL, 3, GL_UNSIGNED_BYTE, true, 0, 0);
        if (colType == COLOR_CMAP){
            prog->use();
            prog->setUniform1b("useCmap", false);
        }
        vao.enableAttribute(MODEL_COL);
        colType = COLOR_NODE;
    }
}

void TriMesh::useCmap(ColorMap& _cmap) {
    vao.bind();
    cmap = _cmap;
    if (colType != COLOR_CMAP){
        vao.enableAttribute(MODEL_COL);
    }
    prog->use();
    prog->setUniform1b("useCmap", true);
    prog->setUniform2f("cmapRange", cmap.cmin, cmap.cmax);
    colType = COLOR_CMAP;
}

void TriMesh::setField(std::vector<float>& fieldValue){
    if (colType != COLOR_CMAP) return;

    vao.bind();
    if (bufIndices[MODEL_COL] < 0){
        VBO* col = new VBO;
        bufIndices[MODEL_COL] = vao.attachBuffer(col);
    }

    setBuffer(MODEL_COL, fieldValue.size()*sizeof(fieldValue[0]), fieldValue.data());
    vao.setAttribute(MODEL_COL, 1, GL_FLOAT, false, 0, 0);
}


void TriMesh::draw() const {
    RENDERER_DEBUG("Drawing trimesh with VAO %u", vao.id);
    vao.bind();
    prog->use();

    switch (colType){
        case COLOR_DEFAULT:
            vao.setDefaultAttributeValues3f(MODEL_COL, 1.0f, 1.0f, 1.0f);
            break;
        case COLOR_CONSTANT:
            vao.setDefaultAttributeValues3f(MODEL_COL, col[0]/255.f, col[1]/255.f, col[2]/255.f);
            break;
        case COLOR_CMAP:
            cmap.bind();
            break;
        default:
            break;
    }

    glDrawElements(GL_TRIANGLES, nElems, GL_UNSIGNED_INT, 0);
}
