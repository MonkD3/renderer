#include "scene.hpp"
#include "glad/gl.h"
#include "models/model.hpp"
#include "shader.hpp"
#include "log.h"


Scene::Scene() {
    mvp = glm::mat4(1.0f);
    imvp = glm::mat4(1.0f);

    models = std::vector<Model*>();
    modelIsRendered = std::vector<bool>();
    zoom = 1.0f;
    aspectRatio = 1.0f;
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
            prog->setUniformMat4f("mvp", &(mvp[0][0]), false);
            prog->setUniformMat4f("imvp", &(imvp[0][0]), false);
            prog->setUniform1f("aspectRatio", aspectRatio);
            prog->setUniform1f("zoom", zoom);

            // Draw the model
            model->draw();
        }
    }
}
