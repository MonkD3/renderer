#include "glad/gl.h"
#include "models/model.hpp"
#include "models/balls.hpp"
#include "log.h"
#include "shader.hpp"

void Balls::initShaderProgram(){
    vshd = new Shader(ASSET_DIR "vertex/defaultBalls.vert", SHADER_VERTEX);
    fshd = new Shader(ASSET_DIR "fragment/defaultBalls.frag", SHADER_FRAGMENT);

    prog = new ShaderProgram();
    prog->attachShader(vshd);
    prog->attachShader(fshd);
    prog->compile();

    vao.bind();
    // Attribute 1 is the color : set a generic white color
    vao.setDefaultAttributeValues3f(MODEL_COL, 1.0f, 1.0f, 1.0f);

    // Attribute 2 is the normals, set a generic normal z normal
    vao.setDefaultAttributeValues4f(MODEL_NORMAL, 0.0f, 0.0f, 1.0f, 0.0f);

    float instanceVertexPos[12] = {
          1.f, -1.f, 0.0f,  // Bot-right
         -1.f, -1.f, 0.0f,  // Bot-left
          1.f,  1.f, 0.0f,  // Top-right
         -1.f,  1.f, 0.0f,  // Top-left
    };
    VBO *vbo = new VBO();
    vbo->bind();
    vbo->setData(sizeof(instanceVertexPos), instanceVertexPos);
    vao.setAttribute(MODEL_TRANS, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
    vao.attachBuffer(vbo);
    vao.enableAttribute(MODEL_TRANS);
}

Balls::Balls(){ }

Balls::Balls(int const _dim, std::vector<float>& centerCoords, std::vector<float>& radius) : dim(_dim), nElems(centerCoords.size()/dim), radType(RADIUS_PER_BALL), colType(COLOR_DEFAULT) {
    vao.bind();

    VBO* positions = new VBO;
    positions->bind();
    positions->setData(centerCoords.size()*sizeof(centerCoords[0]), centerCoords.data());
    vao.setAttribute(MODEL_POS, dim, GL_FLOAT, GL_FALSE, 0, 0);
    bufIndices[MODEL_POS] = vao.attachBuffer(positions);
    vao.enableAttribute(MODEL_POS);
    glVertexAttribDivisor(MODEL_POS, 1);

    VBO* rad = new VBO;
    rad->bind();
    rad->setData(sizeof(radius[0])*radius.size(), radius.data());
    vao.setAttribute(MODEL_SIZE, 1, GL_FLOAT, GL_FALSE, 0, 0);
    bufIndices[MODEL_SIZE] = vao.attachBuffer(rad);
    vao.enableAttribute(MODEL_SIZE);
    glVertexAttribDivisor(MODEL_SIZE, 1);

    initShaderProgram();
}

Balls::Balls(int const _dim, std::vector<float>& centerCoords, float const radius) : dim(_dim), nElems(centerCoords.size()/dim), radType(RADIUS_CONSTANT), colType(COLOR_DEFAULT) {

    vao.bind();

    VBO* positions = new VBO;
    positions->bind();
    positions->setData(centerCoords.size()*sizeof(centerCoords[0]), centerCoords.data());
    vao.setAttribute(MODEL_POS, dim, GL_FLOAT, GL_FALSE, 0, 0);
    bufIndices[MODEL_POS] = vao.attachBuffer(positions);
    vao.enableAttribute(MODEL_POS);
    glVertexAttribDivisor(MODEL_POS, 1);

    glVertexAttrib1f(MODEL_SIZE, radius);
    initShaderProgram();
}

Balls::Balls(int const _dim, VBO* centerCoords, VBO* radius) : dim(_dim), nElems(centerCoords->size/dim), radType(RADIUS_PER_BALL), colType(COLOR_DEFAULT) {
    vao.bind();

    centerCoords->bind();
    vao.setAttribute(MODEL_POS, dim, GL_FLOAT, GL_FALSE, 0, 0);
    bufIndices[MODEL_POS] = vao.attachBuffer(centerCoords);
    vao.enableAttribute(MODEL_POS);
    glVertexAttribDivisor(MODEL_POS, 1);

    radius->bind();
    vao.setAttribute(MODEL_SIZE, 1, GL_FLOAT, GL_FALSE, 0, 0);
    bufIndices[MODEL_SIZE] = vao.attachBuffer(radius);
    vao.enableAttribute(MODEL_SIZE);
    glVertexAttribDivisor(MODEL_SIZE, 1);

    initShaderProgram();
}

Balls::Balls(int const _dim, VBO* centerCoords, float const radius) : dim(_dim), nElems(centerCoords->size/dim), radType(RADIUS_CONSTANT), colType(COLOR_DEFAULT) {

    vao.bind();

    centerCoords->bind();
    vao.setAttribute(MODEL_POS, dim, GL_FLOAT, GL_FALSE, 0, 0);
    bufIndices[MODEL_POS] = vao.attachBuffer(centerCoords);
    vao.enableAttribute(MODEL_POS);
    glVertexAttribDivisor(MODEL_POS, 1);

    glVertexAttrib1f(MODEL_SIZE, radius);
    initShaderProgram();
}


void Balls::setCenters(std::vector<float>& newCenters){
    setBuffer(MODEL_POS, newCenters.size()*sizeof(newCenters[0]), newCenters.data());
}

void Balls::setColor(uint8_t R, uint8_t G, uint8_t B) {

    if (colType != COLOR_CONSTANT) {
        RENDERER_DEBUG("Changing node-coloring type of balls to COLOR_CONSTANT");
        colType = COLOR_CONSTANT;
        vao.bind();
        vao.disableAttribute(MODEL_COL);
    }

    vao.setDefaultAttributeValues3f(MODEL_COL, R/255.f, G/255.f, B/255.f);
}

void Balls::setColor(std::vector<uint8_t>& colors){
    vao.bind();
    if (bufIndices[MODEL_COL] < 0){
        VBO* col = new VBO;
        bufIndices[MODEL_COL] = vao.attachBuffer(col);
    }

    setBuffer(MODEL_COL, colors.size()*sizeof(colors[0]), colors.data());

    if (colType != COLOR_NODE) {
        RENDERER_DEBUG("Changing node-coloring type of balls to COLOR_NODE");
        vao.setAttribute(MODEL_COL, 3, GL_UNSIGNED_BYTE, true, 0, 0);
        if (colType == COLOR_CMAP){
            prog->use();
            prog->setUniform1b("useCmap", false);
        }
        vao.enableAttribute(MODEL_COL);
        glVertexAttribDivisor(MODEL_COL, 1);
        colType = COLOR_NODE;
    }
}

void Balls::useCmap(ColorMap& _cmap) {
    vao.bind();
    cmap = _cmap;
    if (colType != COLOR_CMAP) {
        if (colType == COLOR_DEFAULT || colType == COLOR_CONSTANT){
            vao.enableAttribute(MODEL_COL);
        }
        prog->use();
        prog->setUniform1b("useCmap", true);
        prog->setUniform2f("cmapRange", cmap.cmin, cmap.cmax);
        colType = COLOR_CMAP;
    }
}

void Balls::setField(std::vector<float>& fieldValue){
    if (colType != COLOR_CMAP) return;

    vao.bind();
    if (bufIndices[MODEL_COL] < 0){
        VBO* col = new VBO;
        bufIndices[MODEL_COL] = vao.attachBuffer(col);
    } 

    setBuffer(MODEL_COL, fieldValue.size()*sizeof(fieldValue[0]), fieldValue.data());
    vao.setAttribute(MODEL_COL, 1, GL_FLOAT, false, 0, 0);
    glVertexAttribDivisor(MODEL_COL, 1);
}

void Balls::draw() const {
    RENDERER_DEBUG("Drawing balls with VAO %u", vao.id);
    vao.bind();
    prog->use();

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, nElems);
}
