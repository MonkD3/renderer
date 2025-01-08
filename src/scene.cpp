#include "scene.hpp"
#include "models/model.hpp"
#include "shader.hpp"
#include "log.h"


Scene::Scene() {
    models = std::vector<Model*>();
    modelIsRendered = std::vector<bool>();
}

Scene::~Scene() { }

size_t Scene::addModel(Model* m, bool isRendered) {
    DEBUG("Add model to scene");
    models.push_back(m);
    modelIsRendered.push_back(isRendered);
    return models.size() - 1;
}

void Scene::draw() const {

    Model* model;
    for (size_t m = 0; m < models.size(); m++){
        if (modelIsRendered[m]) {
            model = models[m];

            // Load the scene transform in the shader program of the model
            ShaderProgram * prog = model->prog;
            prog->use();
            model->vao.bind();

            // Set default uniforms
            prog->setUniformMat4f("mvp", &(worldBlock.view[0][0]), false);
            prog->setUniformMat4f("imvp", &(worldBlock.iview[0][0]), false);
            prog->setUniform1f("aspectRatio", worldBlock.scalings[0]);
            prog->setUniform1f("zoom", worldBlock.scalings[1]);

            // Draw the model
            model->draw();
        }
    }
}
