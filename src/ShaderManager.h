//
// Created by Windrian on 09.12.2015.
//

#ifndef SRCCMAKE_SHADERMANAGER_H
#define SRCCMAKE_SHADERMANAGER_H


#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <GL/gl3w.h>
#include <cstdlib>



namespace cgf {
    class ShaderManager {

    public:
        void init();
        void addShader(const char *p_shaderFilename, GLenum shaderType);
        void compileShaders();
        GLuint getShaderProgramLocation();

    private:
        GLuint m_shaderProgramLocation;

        struct Shader {
            Shader() { };

            Shader(const char *shaderText, GLenum shaderType) {
                m_shaderName = shaderText;
                m_shaderType = shaderType;
            }

            const char *m_shaderName;
            GLenum m_shaderType;
        };
        std::vector<Shader> m_shaders;

        bool readFile(const char *p_fileName, std::string &outFile);
        void attachShader(const char *p_shaderText, GLenum shaderType);
    };
}


#endif //SRCCMAKE_SHADERMANAGER_H
