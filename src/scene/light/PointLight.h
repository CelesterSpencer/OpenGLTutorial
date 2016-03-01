//
// Created by Windrian on 08.12.2015.
//

#ifndef SRCCMAKE_POINTLIGHT_H
#define SRCCMAKE_POINTLIGHT_H


#include <iostream>
#include <string>
#include <glm.hpp>

#include "Light.h"
#include "io/Parameterizable.h"

#include "util/Constants.h"


namespace cgf {
    class PointLight : public Light, public Parameterizable {

    public:
        PointLight(std::string name, glm::vec3 position, glm::vec3 color);
        ~PointLight();

        // inherited from Light
        virtual void init(GLuint shaderProgramLocation) override;
        virtual void update() override;

    private:
        std::string m_name;
        glm::vec3 m_color;
        glm::vec3 m_position;

        GLuint m_colorLocation;
        GLuint m_positionLocation;

    };
}


#endif //SRCCMAKE_POINTLIGHT_H
