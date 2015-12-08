//
// Created by Windrian on 08.12.2015.
//

#include "DirectionalLight.h"


DirectionalLight::DirectionalLight(std::string name, glm::vec3 direction, glm::vec3 color) {
    m_name = name;
    m_direction = direction;
    m_color = color;
}

void DirectionalLight::init(GLuint shaderProgramLocation) {
    m_colorLocation = glGetUniformLocation(shaderProgramLocation, (m_name + ".color").c_str());
    m_directionLocation = glGetUniformLocation(shaderProgramLocation, (m_name + ".direction").c_str());

    if (m_colorLocation == INVALID_UNIFORM_LOCATION || m_directionLocation == INVALID_UNIFORM_LOCATION) {
        fprintf(stderr, "Warning! Unable to initialize directional light \n");
    }
}

void DirectionalLight::update() {
    glUniform3f(m_colorLocation, m_color.x, m_color.y, m_color.z);
    glm::vec3 direction = m_direction;
    direction = glm::normalize(direction);
    glUniform3f(m_directionLocation, direction.x, direction.y, direction.z);
}