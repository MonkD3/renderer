#pragma once 

#include <glm/ext/vector_float4.hpp>
#include <glm/glm.hpp>
#include <glm/vec4.hpp>

struct Scene {
    glm::vec4 up;     // What is the upward direction of the camera
    glm::vec4 lookAt; // Where should the camera be looking to
    glm::mat4 mvp;    // Model-Viewport-Projection matrix
    glm::mat4 itmvp;    // Inverse-transpose of Model-Viewport-Projection matrix

    // Apply the transformation @transform to the scene. 
    // This can be scaling/transflation/rotation or any other 4x4 matrix
    void applyTransform(const glm::mat4& transform);

};
