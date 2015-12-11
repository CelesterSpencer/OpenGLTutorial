//
// Created by Windrian on 03.12.2015.
//

#include "Mesh.h"

cgf::Mesh::MeshEntry::MeshEntry() {
    VB = INVALID_GL_VALUE;
    IB = INVALID_GL_VALUE;
    numIndices  = 0;
    materialIndex = INVALID_MATERIAL;
}

cgf::Mesh::MeshEntry::~MeshEntry() {
    if (VB != INVALID_GL_VALUE) {
        glDeleteBuffers(1, &VB);
    }

    if (IB != INVALID_GL_VALUE) {
        glDeleteBuffers(1, &IB);
    }
}

/**
 * generate and bind buffers
 */
void cgf::Mesh::MeshEntry::init(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    numIndices = indices.size();
    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
}



cgf::Mesh::Mesh() {
    m_fileName = "MESH_NOT_SET_UP_YET";
}

cgf::Mesh::~Mesh() {
    clear();
}

void cgf::Mesh::clear() {
    // clear all textures
    for (unsigned int i = 0 ; i < m_textures.size() ; i++) {
        SAFE_DELETE(m_textures[i]);
    }
}

bool cgf::Mesh::loadMesh(const std::string& fileName) {
    // release the previously loaded mesh, if it exists
    clear();

    // set filename for debug
    m_fileName = fileName;

    bool ret = false;
    Assimp::Importer importer;

    // get assimp-scene from file
    const aiScene* pScene = importer.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals);

    // initialize assimp-scene
    if(pScene) {
        ret = initFromScene(pScene, fileName);
    } else {
        printf("Error parsing '%s': '%s'\n", fileName.c_str(), importer.GetErrorString());
    }

    return ret;
}

void cgf::Mesh::initMesh(unsigned int index, const aiMesh* paiMesh) {
    m_entries[index].materialIndex = paiMesh->mMaterialIndex;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    const aiVector3D zero3D(0.0f, 0.0f, 0.0f);

    std::cout << "mesh has " << paiMesh->mNumVertices << " vertices" << std::endl;

    // iterate over all vertices
    // assign position, normal and if present also texcoords to every vertex and add it to vertices
    for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
        const aiVector3D* pPos = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &zero3D; // just use the first texture coordinate

        Vertex v(glm::vec3(pPos->x, pPos->y, pPos->z),
                glm::vec2(pTexCoord->x, pTexCoord->y),
                glm::vec3(pNormal->x, pNormal->y, pNormal->z));

        vertices.push_back(v);
    }

    // iterate over all faces to create the index array
    for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
        const aiFace& face = paiMesh->mFaces[i];
        assert(face.mNumIndices == 3);  // check if polygon is a triangle
        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }

    std::cout << "indices size is now " << indices.size() << std::endl;
    // initialize meshentry
    m_entries[index].init(vertices, indices);

}

bool cgf::Mesh::initMaterials(const aiScene *pScene, const std::string &fileName) {

    // Extract the directory part from the file name
    std::string::size_type SlashIndex = fileName.find_last_of("/");
    std::string Dir;
    if (SlashIndex == std::string::npos) {
        Dir = ".";
    }
    else if (SlashIndex == 0) {
        Dir = "/";
    }
    else {
        Dir = fileName.substr(0, SlashIndex);
    }

    // return status shows if loading textures had been successful
    bool ret = true;

    // load all textures used by this model
    for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
        const aiMaterial* pMaterial = pScene->mMaterials[i];

        m_textures[i] = NULL;

        // check if any diffuse texture is available
        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString path;

            // load diffuse texture
            // here it is assumed, that the texture and the model are in the same subdirectory
            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                std::string fullPath = Dir + "/" + path.data;
                m_textures[i] = new cgf::Texture(GL_TEXTURE_2D, fullPath.c_str());

                if (!m_textures[i]->load()) {
                    printf("Error loading texture '%s' \n", fullPath.c_str());
                    delete m_textures[i];
                    m_textures[i] = NULL;
                    ret = false;
                } else {
                    printf("Loaded texture '%s'\n", fullPath.c_str());
                }
            }
        }

        // if a model doesn't contain any textures then load a default white texture
        if (!m_textures[i]) {
            std::cout << "Loading default white texture, because no texture has been provided" << std::endl;
            m_textures[i] = new cgf::Texture(GL_TEXTURE_2D, "./textures/debug/white.png");
            ret = m_textures[i]->load();
        }
    }

    return ret;
}

bool cgf::Mesh::initFromScene(const aiScene *pScene, const std::string &fileName) {
    m_entries.resize(pScene->mNumMeshes);
    m_textures.resize(pScene->mNumMaterials);

    // initialize the meshes in the scene one by one
    for (unsigned int i = 0; i < m_entries.size(); i++) {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        initMesh(i, paiMesh);
    }

    return initMaterials(pScene, fileName);
}

void cgf::Mesh::render() {
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // bind vertex and index buffers and texture and execute draw command
    for (unsigned int i = 0; i < m_entries.size(); i++) {
        glBindBuffer(GL_ARRAY_BUFFER, m_entries[i].VB);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_entries[i].IB);

        // fetch and bind texture
        const unsigned int materialIndex = m_entries[i].materialIndex;
        if (materialIndex < m_textures.size() && m_textures[materialIndex]) {
            m_textures[materialIndex]->bind(GL_TEXTURE0);
        }

        glDrawElements(GL_TRIANGLES, m_entries[i].numIndices, GL_UNSIGNED_INT, 0);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

void cgf::Mesh::printDebugInfo(std::string whitespace) {
    std::cout << whitespace << "Mesh:"  << std::endl;
    whitespace += "|  ";
    std::cout << whitespace << "Name: '" << m_fileName << "':" << std::endl;
    std::cout << whitespace  << "Number of Entries: " << m_entries.size() << std::endl;
    for (int i = 0; i < m_entries.size(); i++) {
        std::string whitespace2 = whitespace + "|  ";
        std::cout << whitespace2  << "Entry:" << std::endl;
        whitespace2 += "|  ";
        std::cout << whitespace2  << "Number of Indices: " << m_entries[i].numIndices << std::endl;
        std::cout << whitespace2  << "MaterialIndex: " << m_entries[i].materialIndex << std::endl;
        std::cout << whitespace2  << "VB Handle: " << m_entries[i].VB << std::endl;
        std::cout << whitespace2  << "IB Handle: " << m_entries[i].IB << std::endl;
    }
    std::cout << whitespace  << "Number of Textures: " << m_textures.size() << std::endl;
    for (int i = 0; i < m_textures.size(); i++) {
        m_textures[i]->printDebugInfo(whitespace + "|  ");
    }
}