//
// Created by Windrian on 08.12.2015.
//

#include "PointLight.h"

cgf::PointLight::PointLight(std::string name, glm::vec3 position, glm::vec3 color) : Parameterizable(name) {
    m_name = name;
    m_position = position;
    m_color = color;

    Parameterizable::addProperty(PropertiesObject("Pos", &m_position, PropertiesObject::VEC3));
    Parameterizable::addProperty(PropertiesObject("Color", &m_color, PropertiesObject::VEC3));
}

cgf::PointLight::~PointLight() {

}

void cgf::PointLight::init(GLuint shaderProgramLocation) {
    m_colorLocation = glGetUniformLocation(shaderProgramLocation, (m_name + ".color").c_str());
    m_positionLocation = glGetUniformLocation(shaderProgramLocation, (m_name + ".position").c_str());

    if (m_colorLocation == INVALID_UNIFORM_LOCATION || m_positionLocation == INVALID_UNIFORM_LOCATION) {
        std::cerr << "Warning: Unable to get the location of point light!" << std::endl;
    }
}

void cgf::PointLight::update() {
    glUniform3f(m_colorLocation, m_color.x, m_color.y, m_color.z);
    glUniform3f(m_positionLocation, m_position.x, m_position.y, m_position.z);
}