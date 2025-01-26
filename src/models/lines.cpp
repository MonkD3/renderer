#include "glad/gl.h"
#include "models/model.hpp"
#include "models/lines.hpp"
#include "log.h"

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
    vao.setAttribute(0, dim, GL_FLOAT, GL_FALSE, 0, 0);
    bufIndices[MODEL_IDX] = vao.attachBuffer(indices);
    vao.enableAttribute(0);
}

Lines::Lines(int const _dim, VBO* nodeCoords, EBO* lines) : dim(_dim) {

    vao.bind();

    nodeCoords->bind();
    bufIndices[MODEL_POS] = vao.attachBuffer(nodeCoords);

    lines->bind();
    vao.setAttribute(0, dim, GL_FLOAT, GL_FALSE, 0, 0);
    bufIndices[MODEL_IDX] = vao.attachBuffer(lines);
    vao.enableAttribute(0);
}

void Lines::setNodes(std::vector<float>& newNodes){
    setBuffer(MODEL_POS, newNodes.size()*sizeof(newNodes[0]), newNodes.data());
}

void Lines::setColor(uint8_t R, uint8_t G, uint8_t B) {
    vao.bind();
    vao.setDefaultAttributeValues3f(1, R/255.f, G/255.f, B/255.f);
}

void Lines::draw() const {
    DEBUG("Drawing trimesh with VAO %u", vao.id);
    vao.bind();
    prog->use();

    glDrawElements(GL_LINES, nElems, GL_UNSIGNED_INT, 0);
}
