//
// Created by Windrian on 11.12.2015.
//

#ifndef SRCCMAKE_MATERIAL_H
#define SRCCMAKE_MATERIAL_H


#include <GL/gl3w.h>


namespace cgf {
    class Material {
        virtual void init(GLuint shaderProgramLocation) =0;
        virtual void update() =0;
    };
}

#endif //SRCCMAKE_MATERIAL_H
