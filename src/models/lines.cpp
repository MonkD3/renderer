#include "glad/gl.h"
#include "models/model.hpp"
#include "models/lines.hpp"
#include "log.h"

void Lines::initShaderProgram(){
    vshd = new Shader(ASSET_DIR "vertex/defaultVertex.vert", SHADER_VERTEX);
    fshd = new Shader(ASSET_DIR "fragment/defaultFrag.frag", SHADER_FRAGMENT);

    prog = new ShaderProgram();
    prog->attachShader(vshd);
    prog->attachShader(fshd);
    prog->compile();

    vao.bind();
    colType = COLOR_DEFAULT;
}

Lines::Lines(){}

Lines::Lines(int const _dim, std::vector<float>& nodeCoords, std::vector<int>& lines) : dim(_dim), nElems(lines.size()) {

    vao.bind();

    VBO* positions = new VBO;
    positions->bind();
    positions->setData(nodeCoords.size()*sizeof(nodeCoords[0]), nodeCoords.data());
    bufIndices[MODEL_POS] = vao.attachBuffer(positions);

    EBO* indices = new EBO;
    indices->bind();
    indices->setData(sizeof(lines[0])*lines.size(), lines.data());
    vao.setAttribute(MODEL_POS, dim, GL_FLOAT, GL_FALSE, 0, 0);
    bufIndices[MODEL_IDX] = vao.attachBuffer(indices);
    vao.enableAttribute(MODEL_POS);
    initShaderProgram();
}

Lines::Lines(int const _dim, VBO* nodeCoords, EBO* lines) : dim(_dim) {

    vao.bind();

    nodeCoords->bind();
    bufIndices[MODEL_POS] = vao.attachBuffer(nodeCoords);

    lines->bind();
    vao.setAttribute(MODEL_POS, dim, GL_FLOAT, GL_FALSE, 0, 0);
    bufIndices[MODEL_IDX] = vao.attachBuffer(lines);
    vao.enableAttribute(MODEL_POS);
    initShaderProgram();
}

void Lines::setNodes(std::vector<float>& newNodes){
    setBuffer(MODEL_POS, newNodes.size()*sizeof(newNodes[0]), newNodes.data());
}

void Lines::setIndices(std::vector<int>& newIndices){
    setBuffer(MODEL_IDX, newIndices.size()*sizeof(newIndices[0]), newIndices.data());
    nElems = newIndices.size();
}

void Lines::setColor(uint8_t R, uint8_t G, uint8_t B) {

    if (colType != COLOR_CONSTANT) {
        RENDERER_DEBUG("Changing node-coloring type of balls to COLOR_CONSTANT");
        colType = COLOR_CONSTANT;
        vao.bind();
        vao.disableAttribute(MODEL_COL);
    }

    color[0] = R;
    color[1] = G;
    color[2] = B;
}

void Lines::draw() const {
    RENDERER_DEBUG("Drawing trimesh with VAO %u", vao.id);
    vao.bind();
    prog->use();

    switch (colType){
        case COLOR_DEFAULT:
            vao.setDefaultAttributeValues3f(MODEL_COL, 1.0f, 1.0f, 1.0f);
            break;
        case COLOR_CONSTANT:
            vao.setDefaultAttributeValues3f(MODEL_COL, color[0]/255.f, color[1]/255.f, color[2]/255.f);
            break;
        default:
            break;
    }

    glDrawElements(GL_LINES, nElems, GL_UNSIGNED_INT, 0);
}
