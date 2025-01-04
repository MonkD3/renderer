#pragma once 

#include <glm/ext/vector_float4.hpp>
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <vector>

#include "models/model.hpp"

struct Scene {
    glm::mat4 mvp;    // Global Model-Viewport-Projection matrix
    glm::mat4 imvp;   // Inverse of Global Model-Viewport-Projection matrix

    std::vector<Model*> models; // Models in the scene
    std::vector<bool> modelIsRendered; // modelIsRendered[i] = true if models[i] is rendered

    float aspectRatio;
    float zoom;
    
    // Default constructor
    Scene();

    // Default destructor
    ~Scene();

    // Add the model @m to the rendered models in the scene
    size_t addModel(Model* m, bool isRendered = true);

    // Call model.draw() for each model in @models.
    // Additionnally set up the transforms
    void draw() const; 
};
