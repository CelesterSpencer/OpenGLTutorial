//
// Created by Windrian on 08.12.2015.
//

#include "Scene.h"

cgf::Scene::Scene() {

}

cgf::Scene::~Scene() {

}

void cgf::Scene::addLight(Light* p_light) {
    m_lights.push_back(p_light);
}

void cgf::Scene::addMesh(Mesh* p_mesh) {
    m_meshes.push_back(p_mesh);
}

void cgf::Scene::addCamera(Camera* p_camera) {
    m_cameras.push_back(p_camera);
    if (!mp_activeCamera) {
        mp_activeCamera = m_cameras[0];
    }
}

bool cgf::Scene::setActiveCamera(int cameraNumber) {
    if (cameraNumber < m_cameras.size()) {
        mp_activeCamera = m_cameras[cameraNumber];
        return true;
    }
    return false;
}

void cgf::Scene::setPipeline(Pipeline* p_pipeline) {
    mp_pipeline = p_pipeline;
}

void cgf::Scene::init(GLuint shaderProgram) {
    m_MVPLocation = glGetUniformLocation(shaderProgram, "gMVP");
    m_worldLocation = glGetUniformLocation(shaderProgram, "gWorld");
    m_eyeLocation = glGetUniformLocation(shaderProgram, "gEye");

    if(     m_MVPLocation == 0xFFFFFFFF ||
            m_worldLocation == 0xFFFFFFFF ||
            m_eyeLocation == 0xFFFFFFFF) {
        fprintf(stderr, "Warning: couldn't find all provided locations in Scene! \n");
    }
}

void cgf::Scene::render() {

    // update active camera
    mp_activeCamera->update();

    // update pipeline
    mp_pipeline->setCamera(mp_activeCamera->getPosition(), mp_activeCamera->getTarget(), mp_activeCamera->getUp());

    // set different model matrices
    glUniformMatrix4fv(m_MVPLocation, 1, GL_FALSE, value_ptr(mp_pipeline->getTransformation()));
    glUniformMatrix4fv(m_worldLocation, 1, GL_FALSE, value_ptr(mp_pipeline->getWorldTransformation()));
    glm::vec3 eyeWorldPos = mp_activeCamera->getPosition();
    glUniform3f(m_eyeLocation, eyeWorldPos.x, eyeWorldPos.y, eyeWorldPos.z);

    // update lights
    for (Light* light : m_lights) {
        light->update();
    }

    // draw geometry
    for (Mesh* mesh : m_meshes) {
        mesh->render();
    }

}
