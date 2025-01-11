#pragma once 

#include <glm/ext/vector_float4.hpp>
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <vector>

#include "models/model.hpp"
#include "uniforms.hpp"
#include "buffer.hpp"

struct Scene {
    WorldUniformBlock worldBlock; // The state of the world, shared by all models in the scene
    UBO ubo; // The uniform buffer allowing easy modification of uniform blocks across all shader programs
    std::vector<Model*> models; // Models in the scene
    std::vector<bool> modelIsRendered; // modelIsRendered[i] = true if models[i] is rendered
    
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
