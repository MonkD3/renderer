#pragma once 

#include "colormaps.hpp"
#include "models/model.hpp"

enum radiusType {
    RADIUS_CONSTANT = 0,
    RADIUS_PER_BALL = 1
};

struct Balls : public Model {
    int dim;
    int nElems;

    radiusType radType;

    ColorMap cmap;
    colorType colType; 
                           
    Balls();

    // Set of balls with center coordinates @_centerCoords and radiuses @_radius. 
    // Rendering uses white color for each ball
    Balls(int const dim, std::vector<float>& _centerCoords, std::vector<float>& _radius);

    // Set of balls with center coordinates @_centerCoords and radiuses @_radius. 
    // Rendering uses white color for each ball
    Balls(int const dim, std::vector<float>& _centerCoords, float const _radius);
    Balls(int const _dim, VBO* centerCoords, VBO* radius);
    Balls(int const _dim, VBO* centerCoords, float const radius);

    ~Balls() = default;

    void initShaderProgram();

    // Set nodeCoords[i] = newCoords[i]
    void setCenters(std::vector<float>& newNodes);

    // Set a constant color across all balls
    void setColor(uint8_t R, uint8_t G, uint8_t B);

    // Set a different color for each ball
    void setColor(std::vector<uint8_t>& _colors);

    void useCmap(ColorMap& _cmap);
    void setField(std::vector<float>& fieldValue);

    // Set a constant radius accross all balls
    void setRadius(float const r);

    // Set a different radius for each ball
    void setRadiuses(std::vector<float>& r);

    void draw() const;
};
