//
// Created by Windrian on 18.12.2015.
//

#ifndef SRCCMAKE_CUBEMAPTEXTURE_H
#define SRCCMAKE_CUBEMAPTEXTURE_H


#include <iostream>

#include <GL/gl3w.h>

#include "stb_image.h"


class CubemapTexture {

public:
    CubemapTexture(const std::string& directory,
                    const std::string &posXFilename,
                    const std::string &negXFilename,
                    const std::string &posYFilename,
                    const std::string &negYFilename,
                    const std::string &posZFilename,
                    const std::string &negZFilename);
    ~CubemapTexture();

    bool load();
    void bind(GLenum textureUnit);

private:
    std::string m_fileNames[6];
    GLuint m_textureObj;

};


#endif //SRCCMAKE_CUBEMAPTEXTURE_H
