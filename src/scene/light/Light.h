//
// Created by Windrian on 08.12.2015.
//

#ifndef SRCCMAKE_LIGHT_H
#define SRCCMAKE_LIGHT_H


#include <GL/gl3w.h>


class Light {

public:

    virtual void init(GLuint shaderProgramLocation) =0;
    virtual void update() =0;

private:

};
#endif //SRCCMAKE_LIGHT_H
