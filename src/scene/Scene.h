//
// Created by Windrian on 08.12.2015.
//

#ifndef SRCCMAKE_SCENE_H
#define SRCCMAKE_SCENE_H


#include <vector>
#include <gtc/type_ptr.hpp>

#include "light/Light.h"
#include "geometry/Mesh.h"
#include "camera/Camera.h"
#include "scene/Pipeline.h"

namespace cgf {
    class Scene {

    public:
        Scene();
        ~Scene();

        void addLight(Light* p_light);
        void addMesh(Mesh* p_mesh);
        void addCamera(Camera* p_camera);
        bool setActiveCamera(int cameraNumber);
        void setPipeline(Pipeline* p_pipeline);

        void init(GLuint shaderProgram);
        void render();

    private:
        GLuint m_MVPLocation;
        GLuint m_worldLocation;
        GLuint m_eyeLocation;

        std::vector<Light*> m_lights;
        std::vector<Mesh*> m_meshes;
        std::vector<Camera*> m_cameras;
        Camera* mp_activeCamera;
        Pipeline* mp_pipeline;
    };
}


#endif //SRCCMAKE_SCENE_H
