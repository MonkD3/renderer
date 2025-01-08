#pragma once

#include <glm/ext/vector_float4.hpp>
#include <glm/glm.hpp>
#include <glm/vec4.hpp>

struct WorldUniformBlock {
    glm::mat4 view = glm::mat4(1.0f);  // Camera View matrix : accounts for translation/rotation of camera
    glm::mat4 proj = glm::mat4(1.0f); // Perspective projection matrix : account for the projection (perspective/orthographic)
    glm::mat4 iview = glm::mat4(1.0f); // Precomputed inverse of View matrix
    glm::vec4 light = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f); // Origin of the light
    
    // Scalings :
    //     scalings[0] = aspect ratio (width / height)
    //     scalings[1] = zoom 
    //     scalings[2] and scalings[3] : reserved for future use
    glm::vec4 scalings = glm::vec4(1.0f);

    // Options : 
    //    opts[0] = true if lighting is enabled, false if it is not
    //    opts[1] = true if light is pointwise (i.e. @light is a position)
    //            = false if light is ambient (i.e. @light is a direction)
    //    opts[2] and opts[3] : reserved for future use
    glm::bvec4 worldOpts = glm::bvec4(false, false, false, false);
};
