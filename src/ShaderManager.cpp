//
// Created by Windrian on 09.12.2015.
//


#include "ShaderManager.h"

void cgf::ShaderManager::init() {
    // setup shader program
    m_shaderProgramLocation = glCreateProgram();
    if(m_shaderProgramLocation == 0) {
        std::cerr << "Error initializing shader manager!" << std::endl;
        exit(1);
    }
}

void cgf::ShaderManager::addShader(const char* p_shaderFilename, GLenum shaderType) {
    Shader shader(p_shaderFilename, shaderType);
    m_shaders.push_back(shader);
}

void cgf::ShaderManager::compileShaders() {
    // load shaders from external files
    for (Shader shader : m_shaders) {
        std::string shaderContent;
        if(!readFile(shader.m_shaderName, shaderContent)) {
            std::cerr << "Error reading " << shader.m_shaderName << std::endl;
            exit(1);
        }
        std::cout << "Attach " << shader.m_shaderName << std::endl;
        attachShader(shaderContent.c_str(), shader.m_shaderType);
    }

    // check for program related errors
    GLint success = 0;
    GLchar errorLog[1024] = {0};

    // was linking successful?
    glLinkProgram(m_shaderProgramLocation);
    glGetProgramiv(m_shaderProgramLocation, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_shaderProgramLocation, sizeof(errorLog), NULL, errorLog);
        std::cerr << "Error linking shader program: " << errorLog << std::endl;
        exit(1);
    }
    // is shader code valid?
    glValidateProgram(m_shaderProgramLocation);
    glGetProgramiv(m_shaderProgramLocation, GL_VALIDATE_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_shaderProgramLocation, sizeof(errorLog), NULL, errorLog);
        std::cerr << "Error linking shader program: " << errorLog << std::endl;
        exit(1);
    }

    // linking the shaders together
    glUseProgram(m_shaderProgramLocation);

}

GLuint cgf::ShaderManager::getShaderProgramLocation() {
    return m_shaderProgramLocation;
}

bool cgf::ShaderManager::readFile(const char* p_fileName, std::string& outFile) {

    std::ifstream f(p_fileName);

    bool status = false;
    if (f.is_open()) {
        std::string line;
        while(getline(f, line)) {
            outFile.append(line);
            outFile.append("\n");
        }

        f.close();
        status = true;
    } else {
        std::cerr << "No file " << p_fileName << " found at the specified destination! Name must be 'shader/shadername.vert' or 'shader/shadername.frag' etc" << std::endl;
    }

    return status;
}

void cgf::ShaderManager::attachShader(const char* p_shaderText, GLenum shaderType) {
    // create shader
    GLuint shaderObj = glCreateShader(shaderType);
    if (shaderObj == 0) {
        std::cerr << "Error creating shader type " << shaderType << std::endl;
        exit(0);
    }

    // compile shader code
    const GLchar* p[1];
    p[0] = p_shaderText;
    GLint Lengths[1];
    Lengths[0] = strlen(p_shaderText);
    glShaderSource(shaderObj, 1, p, Lengths);
    glCompileShader(shaderObj);
    GLint success;
    glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
    if(!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(shaderObj, sizeof(InfoLog), NULL, InfoLog);
        std::cerr << "Error compiling shader type " << shaderType << std::endl;
    }

    // attach compiled shader object to the program object
    glAttachShader(m_shaderProgramLocation, shaderObj);
}