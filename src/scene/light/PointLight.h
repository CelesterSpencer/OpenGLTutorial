//
// Created by Windrian on 08.12.2015.
//

#ifndef SRCCMAKE_POINTLIGHT_H
#define SRCCMAKE_POINTLIGHT_H


#include "Light.h"

#include <string>
#include <glm.hpp>

#include "util/Constants.h"


class PointLight: public Light {

public:
    PointLight(std::string name, glm::vec3 position, glm::vec3 color);
    ~PointLight();

    virtual void init(GLuint shaderProgramLocation);
    virtual void update();

private:
    std::string m_name;
    glm::vec3 m_color;
    glm::vec3 m_position;

    GLuint m_colorLocation;
    GLuint m_positionLocation;

};


#endif //SRCCMAKE_POINTLIGHT_H
