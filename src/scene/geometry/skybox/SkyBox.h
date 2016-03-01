//
// Created by Windrian on 18.12.2015.
//

#ifndef SRCCMAKE_SKYBOX_H
#define SRCCMAKE_SKYBOX_H


#include <string>

#include <GL/gl3w.h>
#include <gtc/type_ptr.hpp>

#include "scene/camera/Camera.h"
#include "scene/Pipeline.h"
#include "scene/geometry/skybox/CubemapTexture.h"
#include "scene/geometry/Mesh.h"
#include "ShaderManager.h"


namespace cgf {
    class SkyBox {

    public:
        SkyBox(Camera *pCamera, ShaderManager* shaderManager);

        ~SkyBox();

        bool setup(const std::string &Directory,
                  const std::string &PosXFilename, const std::string &NegXFilename,
                  const std::string &PosYFilename, const std::string &NegYFilename,
                  const std::string &PosZFilename, const std::string &NegZFilename);

        bool init();
        void render();

    private:
        Camera* mp_camera;
        CubemapTexture* mp_cubemapTex;
        Mesh* mp_mesh;

        ShaderManager* mp_shaderManager;
        GLuint m_WVPLocation;
        GLuint m_textureLocation;

        void setWVP(const glm::mat4& wvp);
        void setTextureUnit(unsigned int TextureUnit);
    };
}


#endif //SRCCMAKE_SKYBOX_H
