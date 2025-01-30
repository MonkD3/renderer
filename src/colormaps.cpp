#include "colormaps.hpp"
#include "glad/gl.h"
#include "log.h"
#include <vector>

ColorMap::ColorMap(int _nColors, std::vector<unsigned char>& _colors) : nColors(_nColors), colors(_colors) {
    glEnable(GL_TEXTURE_1D);

    glGenTextures(1, &id);
    RENDERER_DEBUG("Create texture %u", id)
    glBindTexture(GL_TEXTURE_1D, id);

    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB8, nColors, 0, GL_RGB, GL_UNSIGNED_BYTE, colors.data());
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
}

ColorMap::~ColorMap(){
    RENDERER_DEBUG("Delete texture %u", id)
    glDeleteTextures(1, &id);
}

void ColorMap::bind() const {
    glBindTexture(GL_TEXTURE_1D, id);
}
