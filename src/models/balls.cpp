#include "glad/gl.h"
#include "models/model.hpp"
#include "models/balls.hpp"
#include "log.h"
#include "shader.hpp"

Balls::Balls(){

}

Balls::Balls(int const _dim, std::vector<float>& _centerCoords, std::vector<float>& _radius) : dim(_dim), centerCoords(_centerCoords), radius(_radius), radType(RADIUS_PER_BALL), colType(COLOR_DEFAULT) {
    delete vshd;
    vshd = new Shader(CMAKE_HOME_DIRECTORY "/assets/vertex/defaultBalls.vert", SHADER_VERTEX);

    gshd = new Shader(CMAKE_HOME_DIRECTORY "/assets/geometry/defaultBalls.geom", SHADER_GEOMETRY);

    delete fshd;
    fshd = new Shader(CMAKE_HOME_DIRECTORY "/assets/fragment/defaultBalls.frag", SHADER_FRAGMENT);

    delete prog;
    prog = new ShaderProgram;
    prog->attachShader(vshd);
    prog->attachShader(gshd);
    prog->attachShader(fshd);
    prog->compile();
    prog->use();

    // Attribute 2 is the color : set a generic white color
    glVertexAttrib3f(2, 1.0f, 1.0f, 1.0f);

    vao.bind();

    VBO* positions = new VBO;
    positions->bind();
    positions->setData(centerCoords.size()*sizeof(centerCoords[0]), centerCoords.data());
    positions->setAttribute(0, dim, BUF_FLOAT, GL_FALSE, 0, 0);
    bufIndices[MODEL_POS] = vao.attachBuffer(positions);
    vao.enableAttribute(0);

    VBO* rad = new VBO;
    rad->bind();
    rad->setData(sizeof(radius[0])*radius.size(), radius.data());
    rad->setAttribute(1, 1, BUF_FLOAT, GL_FALSE, 0, 0);
    bufIndices[MODEL_SIZE] = vao.attachBuffer(rad);
    vao.enableAttribute(1);
}

Balls::Balls(int const _dim, std::vector<float>& _centerCoords, float const _radius) : dim(_dim), centerCoords(_centerCoords), radius(std::vector<float>({_radius})), radType(RADIUS_CONSTANT), colType(COLOR_DEFAULT) {
    delete vshd;
    vshd = new Shader(CMAKE_HOME_DIRECTORY "/assets/vertex/defaultBalls.vert", SHADER_VERTEX);

    gshd = new Shader(CMAKE_HOME_DIRECTORY "/assets/geometry/defaultBalls.geom", SHADER_GEOMETRY);

    delete fshd;
    fshd = new Shader(CMAKE_HOME_DIRECTORY "/assets/fragment/defaultBalls.frag", SHADER_FRAGMENT);

    delete prog;
    prog = new ShaderProgram;
    prog->attachShader(vshd);
    prog->attachShader(gshd);
    prog->attachShader(fshd);
    prog->compile();
    prog->use();

    // Attribute 2 is the color : set a generic white color
    glVertexAttrib3f(2, 1.0f, 1.0f, 1.0f);

    vao.bind();

    VBO* positions = new VBO;
    positions->bind();
    positions->setData(centerCoords.size()*sizeof(centerCoords[0]), centerCoords.data());
    positions->setAttribute(0, dim, BUF_FLOAT, GL_FALSE, 0, 0);
    bufIndices[MODEL_POS] = vao.attachBuffer(positions);
    vao.enableAttribute(0);

    prog->use();
    glVertexAttrib1f(1, radius[0]);
}

void Balls::moveCenters(std::vector<float>& dx){
    for (size_t i = 0; i < dx.size(); i++) centerCoords[i] += dx[i];

    setBuffer(MODEL_POS, centerCoords.size()*sizeof(centerCoords[0]), centerCoords.data());
}

void Balls::setCenters(std::vector<float>& newCenters){
    centerCoords = newCenters;

    setBuffer(MODEL_POS, centerCoords.size()*sizeof(centerCoords[0]), centerCoords.data());
}

void Balls::setColors(uint8_t R, uint8_t G, uint8_t B) {
    colors = std::vector<uint8_t>({R, G, B});

    if (colType != COLOR_CONSTANT) {
        DEBUG("Changing node-coloring type of balls to COLOR_CONSTANT");
        colType = COLOR_CONSTANT;
        vao.bind();
        vao.disableAttribute(2);
    }

    prog->use();
    glVertexAttrib3f(2, R/255.f, G/255.f, B/255.f);
}

void Balls::setColors(std::vector<uint8_t>& _colors){
    vao.bind();
    colors = _colors;
    if (vao.buffers[bufIndices[MODEL_COL]]){
        setBuffer(MODEL_COL, colors.size()*sizeof(colors[0]), colors.data());
    } else {
        VBO* col = new VBO;
        col->bind();
        col->setData(colors.size()*sizeof(colors[0]), colors.data());
        col->setAttribute(2, 3, BUF_UBYTE, true, 0, 0);
        bufIndices[MODEL_COL] = vao.attachBuffer(col);
    }

    if (colType != COLOR_NODE) {
        DEBUG("Changing node-coloring type of balls to COLOR_NODE");
        colType = COLOR_NODE;
        vao.enableAttribute(2);
    }
}

void Balls::draw() const {
    DEBUG("Drawing balls with VAO %u", vao.id);
    vao.bind();
    prog->use();

    glDrawArrays(GL_POINTS, 0, centerCoords.size() / dim);
}
