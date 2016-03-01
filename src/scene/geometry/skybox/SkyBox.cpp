//
// Created by Windrian on 18.12.2015.
//

#include "SkyBox.h"

cgf::SkyBox::SkyBox(Camera* pCamera, ShaderManager* shaderManager) {
    mp_camera = pCamera;
    mp_cubemapTex = NULL;
    mp_mesh = NULL;
    mp_shaderManager = shaderManager;
}

cgf::SkyBox::~SkyBox() {

}

bool cgf::SkyBox::setup(const std::string& Directory,
                  const std::string& PosXFilename, const std::string& NegXFilename,
                  const std::string& PosYFilename, const std::string& NegYFilename,
                  const std::string& PosZFilename, const std::string& NegZFilename) {
    if (!init()) {
        printf("Error initializing the skybox technique\n");
        return false;
    }

    setTextureUnit(0);

    mp_cubemapTex = new CubemapTexture(Directory, PosXFilename, NegXFilename, PosYFilename, NegYFilename, PosZFilename, NegZFilename);
    if (!mp_cubemapTex->load()) {
        return false;
    }

    mp_mesh = new Mesh();
    return mp_mesh->loadMesh("../model/skygeometry/sphere.obj");
}

void cgf::SkyBox::render() {
    GLint OldCullFaceMode;
    glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
    GLint OldDepthFuncMode;
    glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

    glCullFace(GL_FRONT);
    glDepthFunc(GL_LEQUAL);

    Pipeline p;
    p.scale(20.0f, 20.0f, 20.0f);
    p.rotation(0.0f, 0.0f, 0.0f);
    p.position(mp_camera->getPosition().x, mp_camera->getPosition().y, mp_camera->getPosition().z);
    p.setCamera(mp_camera->getPosition(), mp_camera->getTarget(), mp_camera->getUp());
    p.perspectiveProjection(mp_camera->getCameraangle(), mp_camera->getImageplaneWidth(), mp_camera->getImageplaneHeight(), mp_camera->getNear(), mp_camera->getFar());
    setWVP(p.getWorldTransformation());
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, value_ptr(p.getWorldTransformation()));
    mp_cubemapTex->bind(GL_TEXTURE0);
    mp_mesh->render();

    glCullFace(OldCullFaceMode);
    glDepthFunc(OldDepthFuncMode);
}

bool cgf::SkyBox::init() {
    // set and compile skybox shaders
    mp_shaderManager->init();
    mp_shaderManager->addShader("shader/skybox.vert", GL_VERTEX_SHADER);
    mp_shaderManager->addShader("shader/skybox.frag", GL_FRAGMENT_SHADER);
    mp_shaderManager->compileShaders();

    m_WVPLocation = glGetUniformLocation(mp_shaderManager->getShaderProgramLocation(), "gWVP");
    m_textureLocation = glGetUniformLocation(mp_shaderManager->getShaderProgramLocation(), "gCubemapTexture");

    if (m_WVPLocation == INVALID_UNIFORM_LOCATION ||
        m_textureLocation == INVALID_UNIFORM_LOCATION) {
        return false;
    }

    return true;
}

void cgf::SkyBox::setWVP(const glm::mat4& wvp) {
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, value_ptr(wvp));
}


void cgf::SkyBox::setTextureUnit(unsigned int TextureUnit) {
    glUniform1i(m_textureLocation, TextureUnit);
}
