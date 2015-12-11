//
// Created by Windrian on 03.12.2015.
//

#ifndef SRCCMAKE_MESH_H
#define SRCCMAKE_MESH_H

#include <string>
#include <GL/gl3w.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm.hpp>

#include "../../util/Constants.h"
#include "Texture.h"


namespace cgf {
    class Mesh {

    public:
        Mesh();

        ~Mesh();

        struct Vertex {
            glm::vec3 m_pos;
            glm::vec2 m_tex;
            glm::vec3 m_normal;

            Vertex() { }

            Vertex(const glm::vec3 &pos, const glm::vec2 &tex, const glm::vec3 &normal) {
                m_pos = pos;
                m_tex = tex;
                m_normal = normal;
            }
        };

        bool loadMesh(const std::string &fileName);

        void render();

        void printDebugInfo(std::string whitespace = "");

    private:
        struct MeshEntry {
            MeshEntry();

            ~MeshEntry();

            GLuint VB;
            GLuint IB;
            unsigned int numIndices;
            unsigned int materialIndex;

            void init(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);
        };

        std::string m_fileName;
        std::vector<MeshEntry> m_entries;
        std::vector<cgf::Texture *> m_textures;

        bool initFromScene(const aiScene *pScene, const std::string &fileName);

        void initMesh(unsigned int index, const aiMesh *paiMesh);

        bool initMaterials(const aiScene *pScene, const std::string &fileName);

        void clear();
    };
}


#endif //SRCCMAKE_MESH_H
