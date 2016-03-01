//
// Created by Windrian on 11.12.2015.
//

#include "BasicMaterial.h"

cgf::BasicMaterial::BasicMaterial(float ambientIntensity, float diffuseIntensity, float specularIntensity, float specularPower) : Material("Basicmaterial") {
    m_ambientIntensity = ambientIntensity;
    m_diffuseIntensity = diffuseIntensity;
    m_specularIntensity = specularIntensity;
    m_specularPower = specularPower;

    Parameterizable::addProperty(PropertiesObject("Ambient", &m_ambientIntensity, PropertiesObject::FLOAT));
    Parameterizable::addProperty(PropertiesObject("Diffuse", &m_diffuseIntensity, PropertiesObject::FLOAT));
    Parameterizable::addProperty(PropertiesObject("Specular", &m_specularIntensity, PropertiesObject::FLOAT));
    Parameterizable::addProperty(PropertiesObject("Power", &m_specularPower, PropertiesObject::FLOAT));
}

void cgf::BasicMaterial::init(GLuint shaderProgramLocation) {
    m_ambientIntensityLocation = glGetUniformLocation(shaderProgramLocation, "gBasicMaterial.ambientIntensity");
    m_diffuseIntensityLocation = glGetUniformLocation(shaderProgramLocation, "gBasicMaterial.diffuseIntensity");
    m_specularIntensityLocation = glGetUniformLocation(shaderProgramLocation, "gBasicMaterial.specularIntensity");
    m_specularPowerLocation = glGetUniformLocation(shaderProgramLocation, "gBasicMaterial.specularPower");

    if (    m_ambientIntensityLocation == INVALID_UNIFORM_LOCATION ||
            m_diffuseIntensityLocation == INVALID_UNIFORM_LOCATION ||
            m_specularIntensityLocation == INVALID_UNIFORM_LOCATION ||
            m_specularPowerLocation == INVALID_UNIFORM_LOCATION) {
        std::cout << "Warning: Unable to find all locations for basic material" << std::endl;
    }
}

void cgf::BasicMaterial::update() {
    glUniform1f(m_ambientIntensityLocation, m_ambientIntensity);
    glUniform1f(m_diffuseIntensityLocation, m_diffuseIntensity);
    glUniform1f(m_specularIntensityLocation, m_specularIntensity);
    glUniform1f(m_specularPowerLocation, m_specularPower);
}