#pragma once 

#include "models/model.hpp"

enum radiusType {
    RADIUS_CONSTANT = 0,
    RADIUS_PER_BALL = 1
};

struct Balls : public Model {
    int dim;
    std::vector<float> centerCoords;  // Coordinates of centers
    std::vector<float> radius;   
    std::vector<uint8_t> colors;

    Shader* gshd;

    radiusType radType;    // Type of radius : 
                           //   - RADIUS_CONSTANT : each ball has the same radius 
                           //   - RADIUS_PER_BALL : each ball has a different radius

    colorType colType;     // Type of node-coloring : 
                           //   - COLOR_CONSTANT : each node has the same color (only supply R, G, B)
                           //   - COLOR_NODE : each node has a separate color (supply color for each node)
                                  
    Balls();

    // Set of balls with center coordinates @_centerCoords and radiuses @_radius. 
    // Rendering uses white color for each ball
    Balls(int const dim, std::vector<float>& _centerCoords, std::vector<float>& _radius);

    // Set of balls with center coordinates @_centerCoords and radiuses @_radius. 
    // Rendering uses white color for each ball
    Balls(int const dim, std::vector<float>& _centerCoords, float const _radius);

    ~Balls() = default;

    // Computes nodeCoords[i] += dx[i]
    void moveCenters(std::vector<float>& dx);

    // Set nodeCoords[i] = newCoords[i]
    void setCenters(std::vector<float>& newNodes);

    // Set a constant color across all balls
    void setColors(uint8_t R, uint8_t G, uint8_t B);

    // Set a different color for each ball
    void setColors(std::vector<uint8_t>& _colors);

    // Set a constant radius accross all balls
    void setRadius(float const r);

    // Set a different radius for each ball
    void setRadiuses(std::vector<float>& r);

    void draw() const;
};
