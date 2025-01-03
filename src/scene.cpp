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
            ShaderProgram const* prog = model->prog;
            prog->use();
            model->vao.bind();
            int mvpLoc = glGetUniformLocation(prog->id, "mvp");
            if (mvpLoc >= 0) {
                glUniformMatrix4fv(mvpLoc, 1, false, &(mvp[0][0]));
            }

            int imvpLoc = glGetUniformLocation(prog->id, "imvp");
            if (imvpLoc >= 0) {
                glUniformMatrix4fv(imvpLoc, 1, false, &(imvp[0][0]));
            }

            // Draw the model
            model->draw();
        }
    }
}
