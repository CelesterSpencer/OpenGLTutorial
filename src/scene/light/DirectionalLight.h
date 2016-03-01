//
// Created by Windrian on 08.12.2015.
//

#ifndef SRCCMAKE_DIRECTIONALLIGHT_H
#define SRCCMAKE_DIRECTIONALLIGHT_H


#include "Light.h"

#include <string>
#include <glm.hpp>
#include <iostream>

#include "util/Constants.h"


class DirectionalLight: public Light {

public:
    DirectionalLight(std::string name, glm::vec3 direction, glm::vec3 color);
    ~DirectionalLight();

    virtual void init(GLuint shaderProgramLocation);
    virtual void update();

private:
    std::string m_name;
    glm::vec3 m_color;
    glm::vec3 m_direction;
    GLuint m_colorLocation;
    GLuint m_directionLocation;
};


#endif //SRCCMAKE_DIRECTIONALLIGHT_H
