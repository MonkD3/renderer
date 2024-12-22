#include "model.hpp"
#include "buffer.hpp"
#include "glad/gl.h"
#include "shader.hpp"

Model::Model(){
    vshd = new Shader(CMAKE_HOME_DIRECTORY "/assets/vertex/pos.vert", SHADER_VERTEX);
    fshd = new Shader(CMAKE_HOME_DIRECTORY "/assets/fragment/white.frag", SHADER_FRAGMENT);

    prog = new ShaderProgram();
    prog->attachShader(vshd);
    prog->attachShader(fshd);
    prog->compile();
}

Model::~Model() {
    delete vshd;
    delete fshd;
    delete prog;
}

TriMesh::TriMesh(){}

TriMesh::TriMesh(int const _dim, std::vector<float>& _nodeCoords, std::vector<int>& _triangles) : dim(_dim), nodeCoords(_nodeCoords), elem(_triangles) {

    vao.bind();

    positions.bind();
    positions.setData(dim*nodeCoords.size()*sizeof(_nodeCoords[0]), nodeCoords.data());
    vao.attachBuffer(&positions);

    indices.bind();
    indices.setData(3*sizeof(elem[0])*elem.size(), elem.data());
    indices.setAttribute(0, dim, BUF_FLOAT, GL_FALSE, 0, 0);
    vao.attachBuffer(&indices);
    vao.enableAttribute(0);
}

void TriMesh::moveNodes(std::vector<float>& dx){
    for (size_t i = 0; i < dx.size(); i++) nodeCoords[i] += dx[i];

    positions.bind();
    positions.setData(dim*nodeCoords.size()*sizeof(nodeCoords[0]), nodeCoords.data());
}

void TriMesh::setNodes(std::vector<float>& newNodes){
    nodeCoords = newNodes;

    positions.bind();
    positions.setData(dim*nodeCoords.size()*sizeof(nodeCoords[0]), nodeCoords.data());
}

void TriMesh::draw() const {
    vao.bind();
    prog->use();

    glDrawArrays(GL_TRIANGLES, 0, nodeCoords.size()/2);
}
