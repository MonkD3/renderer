#pragma once 

#include <vector>
struct ColorMap {
    unsigned int id;
    int nColors; // Number of different colors
    std::vector<unsigned char> colors; // colors, stored as R1 G1 B1 R2 G2 B2 ...
    float cmin; // Minimum value for the color map
    float cmax; // Maximum value for the color map
    
    ColorMap() = default;
    ColorMap(int _nColors, std::vector<unsigned char>& _colors);

    ~ColorMap();

    void bind() const;
};
