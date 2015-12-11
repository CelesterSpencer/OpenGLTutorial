//
// Created by Windrian on 11.12.2015.
//

#ifndef SRCCMAKE_BASICMATERIAL_H
#define SRCCMAKE_BASICMATERIAL_H


#include "Material.h"

#include <iostream>

#include "util/Constants.h"


namespace cgf {
    class BasicMaterial : public Material {

    public:
        BasicMaterial(float ambientIntensity, float diffuseIntensity, float specularIntensity, float specularPower);

        // inherited from Material
        virtual void init(GLuint shaderProgramLocation) override;
        virtual void update() override;

    private:
        float m_ambientIntensity;
        float m_diffuseIntensity;
        float m_specularIntensity;
        float m_specularPower;

        GLuint m_ambientIntensityLocation;
        GLuint m_diffuseIntensityLocation;
        GLuint m_specularIntensityLocation;
        GLuint m_specularPowerLocation;

    };
}


#endif //SRCCMAKE_BASICMATERIAL_H
