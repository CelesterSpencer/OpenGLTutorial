//
// Created by Windrian on 18.12.2015.
//

#include "CubemapTexture.h"

static const GLenum types[6] = {  GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                                  GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                                  GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                                  GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                                  GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                                  GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

CubemapTexture::CubemapTexture(const std::string& directory,
               const std::string &posXFilename,
               const std::string &negXFilename,
               const std::string &posYFilename,
               const std::string &negYFilename,
               const std::string &posZFilename,
               const std::string &negZFilename) {
    std::string::const_iterator it = directory.end();
    it--;
    std::string BaseDir = (*it == '/') ? directory : directory + "/";

    m_fileNames[0] = BaseDir + posXFilename;
    m_fileNames[1] = BaseDir + negXFilename;
    m_fileNames[2] = BaseDir + posYFilename;
    m_fileNames[3] = BaseDir + negYFilename;
    m_fileNames[4] = BaseDir + posZFilename;
    m_fileNames[5] = BaseDir + negZFilename;

    m_textureObj = 0;
}
CubemapTexture::~CubemapTexture() { }

bool CubemapTexture::load() {
    glGenTextures(1, &m_textureObj);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureObj);

    int width, height, channels;
    unsigned char* p_image;

    int length = sizeof(types) / sizeof(types[0]);

    for (unsigned int i = 0 ; i < length; i++) {

        p_image = stbi_load(m_fileNames[i].c_str(), &width, &height, &channels, 0);
        if (p_image == 0) {
            std::cout << "Error loading texture '" << m_fileNames[i] << "': " << stbi_failure_reason() << std::endl;
            return false;
        }

        glTexImage2D(types[i], 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, p_image);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        delete p_image;
    }

    return true;
}

void CubemapTexture::bind(GLenum textureUnit) {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureObj);
}