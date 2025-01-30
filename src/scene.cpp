#include "scene.hpp"
#include "models/model.hpp"
#include "shader.hpp"
#include "log.h"


Scene::Scene() {
    ubo.bind();
    ubo.setData(sizeof(worldBlock), NULL);
    ubo.bindUniformRange(0, 0, sizeof(worldBlock));

    models = std::vector<Model*>();
    modelIsRendered = std::vector<bool>();
}

Scene::~Scene() { }

size_t Scene::addModel(Model* m, bool isRendered) {
    RENDERER_DEBUG("Add model to scene");
    m->prog->setUniformBlockBinding("worldBlock", 0);
    models.push_back(m);
    modelIsRendered.push_back(isRendered);
    return models.size() - 1;
}

void Scene::draw() const {

    Model* model;
    for (size_t m = 0; m < models.size(); m++){
        if (modelIsRendered[m]) {
            model = models[m];

            // Set default uniforms
            ubo.bind();
            ubo.setSubData(0, sizeof(worldBlock), &worldBlock);

            // Load the scene transform in the shader program of the model
            ShaderProgram * prog = model->prog;
            prog->use();
            model->vao.bind();

            // Draw the model
            model->draw();
        }
    }
}
