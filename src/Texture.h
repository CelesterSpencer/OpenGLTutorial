//
// Created by Windrian on 21.11.2015.
//

#ifndef SRCCMAKE_TEXTURE_H
#define SRCCMAKE_TEXTURE_H

#include <string>
#include <iostream>
#include "stb_image.h"
#include <GL/gl3w.h>

namespace cgf {
    class Texture {

    public:
        Texture(GLenum textureTarget, const std::string& fileName);

        bool load();
        void bind(GLenum textureUnit);

        void printDebugInfo(std::string whitespace = "");

    private:
        std::string m_fileName;
        GLenum m_textureTarget;
        GLuint m_textureObj;
        unsigned char* m_image;
    };
}


#endif //SRCCMAKE_TEXTURE_H
