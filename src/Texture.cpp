//
// Created by Windrian on 21.11.2015.
//

#include "Texture.h"

cgf::Texture::Texture(GLenum textureTarget, const std::string& fileName) {
    m_textureTarget = textureTarget;
    m_fileName      = fileName;
}

bool cgf::Texture::load() {

    int width, height, channels;
    m_image = stbi_load(m_fileName.c_str(), &width, &height, &channels, 0);
    if (m_image == 0) {
        std::cout << "Error loading texture '" << m_fileName << "': " << stbi_failure_reason() << std::endl;
        return false;
    }

    glGenTextures(1, &m_textureObj);
    glBindTexture(m_textureTarget, m_textureObj);
    glTexImage2D(m_textureTarget, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image);
    glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(m_textureTarget, 0);

    return true;
}

void cgf::Texture::bind(GLenum textureUnit) {
    glActiveTexture(textureUnit);
    glBindTexture(m_textureTarget, m_textureObj);
}