// standard headers
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <string.h>

// glew, include it before gl.h and glfw.h
#include <GL/gl3w.h>

// glfw
#include <GLFW/glfw3.h>

// glm
#include <glm.hpp>
#include <gtc/type_ptr.hpp>

// imgui
#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"

// internal includes
#include "scene/Pipeline.h"
#include "scene/camera/Camera.h"
#include "scene/camera/TrackballCamera.h"
#include "scene/geometry/Texture.h"
#include "scene/geometry/Mesh.h"


// --------------------------------------------------------
// global variables
// --------------------------------------------------------
// shaders
std::string m_vertexShaderFileName;
std::string m_fragmentShaderFileName;
GLuint shaderProgramLocation;

// transformation matrices
GLuint gMVPLocation;
GLuint gWorldLocation;

// light handles
enum LightMode {
    DIRECTIONAL_LIGHT,
    POINT_LIGHT
}m_lightMode = LightMode::DIRECTIONAL_LIGHT ;

GLuint gDirLightColorLocation;
GLuint gDirLightAmbientIntensityLocation;
GLuint gDirLightDirectionLocation;
GLuint gDirLightDiffuseIntensityLocation;
GLuint gEyeWorldPosLocation;
GLuint gMatSpecularIntensityLocation;
GLuint gMatSpecularPowerLocation;
GLuint gLightPositionLocation;
GLuint gConstantAttLocation;
GLuint gLinearAttLocation;
GLuint gExpAttLocation;
GLuint gRimLightColorLocation;
GLuint gRimLightRimExponentialLocation;

// material
struct Material {
    float m_specularIntensity;
    float m_specularPower;

    Material() {}
    Material(float specularIntensity, float specularPower) {
        m_specularIntensity = specularIntensity;
        m_specularPower = specularPower;
    }
} m_material;

// texture
GLuint gSampler;
cgf::Texture* pTexture = NULL;

// mesh
Mesh* m_mesh;

// camera
cgf::Camera* m_camera;

// mouse
bool mouseBtnPressed = false;

// window
float WINDOW_WIDTH = 1024;
float WINDOW_HEIGHT = 768;

// light sources
struct DirectionalLight {
    glm::vec3 color;
    float ambientIntensity;
    glm::vec3 direction;
    float diffuseIntensity;
} m_directionalLight;
struct PointLight {
    glm::vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
    glm::vec3 position;
    struct {
        float constant;
        float linear;
        float exponential;
    } attenuation;
} m_pointLight;
struct RimLight {
    glm::vec3 color;
    float rimExponential;
} m_rimLight;




// --------------------------------------------------------
// glfw callbacks
// --------------------------------------------------------
void resizeCallback(GLFWwindow* window, int width, int height) {
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
    glViewport(0,0, width, height);
}

void errorCallback(int error, const char* description) {
    fputs(description, stderr);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    // check whether ImGUI is handling this
    ImGuiIO& io = ImGui::GetIO();
    if ( io.WantCaptureKeyboard ) {
        return;
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } else {
        m_camera->onKeyboard(key);
    }
}

void cursorCallback(GLFWwindow* window, double xPos, double yPos) {

    // check whether ImGUI is handling this
    ImGuiIO& io = ImGui::GetIO();
    if ( io.WantCaptureMouse ) {
        return;
    }

    m_camera->onMouse(xPos, yPos);
}

void mouseBtnCallback(GLFWwindow* window, int button, int action, int mods) {
    m_camera->onMouseButton(button, action);
}

void mouseWheelCallback(GLFWwindow* window, double xOffset, double yOffset) {
    m_camera->onMouseScroll(xOffset, yOffset);
}



// --------------------------------------------------------
// debug
// --------------------------------------------------------
void printOpenGLInfo() {
    // check opengl version
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
}



// --------------------------------------------------------
// setup shader
// --------------------------------------------------------
bool readFile(const char* p__fileName, std::string& outFile) {

    std::ifstream f(p__fileName);

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
        printf("File error in %s", p__fileName);
    }

    return status;
}

void addShader(GLuint shaderProgram, const char* p__shaderText, GLenum shaderType) {
    GLuint shaderObj = glCreateShader(shaderType);
    if (shaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", shaderType);
        exit(0);
    }

    const GLchar* p[1];
    p[0] = p__shaderText;
    GLint Lengths[1];
    Lengths[0] = strlen(p__shaderText);
    glShaderSource(shaderObj, 1, p, Lengths);
    glCompileShader(shaderObj);
    GLint success;
    glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
    if(!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(shaderObj, sizeof(InfoLog), NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", shaderType, InfoLog);
    }

    // attach compiled shader object to the program object
    glAttachShader(shaderProgram, shaderObj);
}

void compileShaders() {
    // setup shader program
    GLuint shaderProgram = glCreateProgram();
    shaderProgramLocation = shaderProgram;
    if(shaderProgram == 0) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

    // load shaders from external files
    std::string vs, fs;
    if(!readFile(m_vertexShaderFileName.c_str(), vs)) {
        exit(1);
    }
    if(!readFile(m_fragmentShaderFileName.c_str(), fs)) {
        exit(1);
    }
    addShader(shaderProgram, vs.c_str(), GL_VERTEX_SHADER);
    addShader(shaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

    // check for program related errors
    GLint success = 0;
    GLchar errorLog[1024] = {0};

    // linking was successful?
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", errorLog);
        exit(1);
    }
    // is shader code valid?
    glValidateProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", errorLog);
        exit(1);
    }

    // linking the shaders together
    glUseProgram(shaderProgram);

    // now variables within the shader program can be querryed
    gMVPLocation = glGetUniformLocation(shaderProgram, "gMVP");
    gWorldLocation = glGetUniformLocation(shaderProgram, "gWorld");
    if(gMVPLocation == 0xFFFFFFFF) {
        fprintf(stderr, "Cannot find location of property within the shaders");
        exit(1);
    }
    if(gWorldLocation == 0xFFFFFFFF) {
        fprintf(stderr, "Cannot find location of property within the shaders");
        exit(1);
    }

}



// --------------------------------------------------------
// setup lighting
// --------------------------------------------------------
/*void calculateNormals(unsigned int* pIndices, unsigned int indexCount, Vertex* pVertices, unsigned int vertexCount) {
    for (unsigned int i = 0 ; i < indexCount ; i += 3) {
        unsigned int Index0 = pIndices[i];
        unsigned int Index1 = pIndices[i + 1];
        unsigned int Index2 = pIndices[i + 2];
        glm::vec3 v1 = pVertices[Index1].m_pos - pVertices[Index0].m_pos;
        glm::vec3 v2 = pVertices[Index2].m_pos - pVertices[Index0].m_pos;
        glm::vec3 Normal = glm::cross(v1, v2);
        Normal = glm::normalize(Normal);

        pVertices[Index0].m_normal += Normal;
        pVertices[Index1].m_normal += Normal;
        pVertices[Index2].m_normal += Normal;
    }

    for (unsigned int i = 0 ; i < vertexCount ; i++) {
        pVertices[i].m_normal = glm::normalize(pVertices[i].m_normal);
    }
}*/

void setupDirectionalLight() {
    m_lightMode = LightMode::DIRECTIONAL_LIGHT;

    m_vertexShaderFileName = "shader/lighting.vert";
    m_fragmentShaderFileName = "shader/lighting.frag";

    compileShaders();

    m_directionalLight.color = glm::vec3(1.0f,1.0f,1.0f);
    m_directionalLight.ambientIntensity = 0.5f;
    m_directionalLight.direction = glm::vec3(4.0f, -3.0f, 0.0f);
    m_directionalLight.diffuseIntensity = 0.4f;

    gDirLightColorLocation = glGetUniformLocation(shaderProgramLocation, "gDirectionalLight.color");
    gDirLightAmbientIntensityLocation = glGetUniformLocation(shaderProgramLocation, "gDirectionalLight.ambientIntensity");
    gDirLightDirectionLocation = glGetUniformLocation(shaderProgramLocation, "gDirectionalLight.direction");
    gDirLightDiffuseIntensityLocation = glGetUniformLocation(shaderProgramLocation, "gDirectionalLight.diffuseIntensity");
    gEyeWorldPosLocation = glGetUniformLocation(shaderProgramLocation, "gEyeWorldPos");
    gMatSpecularIntensityLocation = glGetUniformLocation(shaderProgramLocation, "gMatSpecularIntensity");
    gMatSpecularPowerLocation = glGetUniformLocation(shaderProgramLocation, "gSpecularPower");

    if (gDirLightColorLocation == 0xffffffff ||
            gDirLightAmbientIntensityLocation == 0xffffffff ||
            gDirLightDirectionLocation == 0xffffffff ||
            gDirLightDiffuseIntensityLocation == 0xffffffff ||
            gEyeWorldPosLocation == 0xffffffff ||
            gMatSpecularIntensityLocation == 0xffffffff ||
            gMatSpecularPowerLocation  == 0xffffffff) {
        fprintf(stderr, "Warning! Unable to get the location light color\n");
    }
}

void setDirectionalLight(DirectionalLight &light) {
    glUniform3f(gDirLightColorLocation, light.color.x, light.color.y, light.color.z);
    glUniform1f(gDirLightAmbientIntensityLocation, light.ambientIntensity);
    glm::vec3 direction = light.direction;
    glm::normalize(direction);
    glUniform3f(gDirLightDirectionLocation, direction.x, direction.y, direction.z);
    glUniform1f(gDirLightDiffuseIntensityLocation, light.diffuseIntensity);
}

void setupPointLight() {
    m_lightMode = LightMode::POINT_LIGHT;

    m_vertexShaderFileName = "shader/pointlight.vert";
    m_fragmentShaderFileName = "shader/pointlight.frag";

    compileShaders();

    m_pointLight.color = glm::vec3(1.0f,1.0f,1.0f);
    m_pointLight.ambientIntensity = 0.5f;
    m_pointLight.position = glm::vec3(4.0f, 5.0f, 1.0f);
    m_pointLight.diffuseIntensity = 0.4f;
    m_pointLight.attenuation.constant = 1;
    m_pointLight.attenuation.linear = 0.4;
    m_pointLight.attenuation.exponential = 0.3;

    gDirLightColorLocation = glGetUniformLocation(shaderProgramLocation, "gPointLight.color");
    gDirLightAmbientIntensityLocation = glGetUniformLocation(shaderProgramLocation, "gPointLight.ambientIntensity");
    gDirLightDiffuseIntensityLocation = glGetUniformLocation(shaderProgramLocation, "gPointLight.diffuseIntensity");
    gLightPositionLocation = glGetUniformLocation(shaderProgramLocation, "gPointLight.position");
    gConstantAttLocation = glGetUniformLocation(shaderProgramLocation, "gPointLight.attenuation.constant");
    gLinearAttLocation = glGetUniformLocation(shaderProgramLocation, "gPointLight.attenuation.linear");
    gExpAttLocation = glGetUniformLocation(shaderProgramLocation, "gPointLight.attenuation.exponential");

    gEyeWorldPosLocation = glGetUniformLocation(shaderProgramLocation, "gEyeWorldPos");
    gMatSpecularIntensityLocation = glGetUniformLocation(shaderProgramLocation, "gMatSpecularIntensity");
    gMatSpecularPowerLocation = glGetUniformLocation(shaderProgramLocation, "gSpecularPower");

    if (gDirLightColorLocation == 0xffffffff ||
        gDirLightAmbientIntensityLocation == 0xffffffff ||
        gLightPositionLocation == 0xffffffff ||
        gConstantAttLocation == 0xffffffff ||
        gLinearAttLocation == 0xffffffff ||
        gExpAttLocation == 0xffffffff ||
        gDirLightDiffuseIntensityLocation == 0xffffffff ||
        gEyeWorldPosLocation == 0xffffffff ||
        gMatSpecularIntensityLocation == 0xffffffff ||
        gMatSpecularPowerLocation  == 0xffffffff) {
        fprintf(stderr, "Warning! Unable to get the location of point light\n");
    }
}

void setPointLight(PointLight &light) {
    glUniform3f(gDirLightColorLocation, light.color.x, light.color.y, light.color.z);
    glUniform1f(gDirLightAmbientIntensityLocation, light.ambientIntensity);
    glUniform1f(gDirLightDiffuseIntensityLocation, light.diffuseIntensity);
    glUniform3f(gLightPositionLocation, light.position.x, light.position.y, light.position.z);
    glUniform1f(gConstantAttLocation, light.attenuation.constant);
    glUniform1f(gLinearAttLocation, light.attenuation.linear);
    glUniform1f(gExpAttLocation, light.attenuation.exponential);
}

void setupRimLight() {
    m_rimLight.color = glm::vec3(0.0f, 0.0f, 1.0f);
    m_rimLight.rimExponential = 1.0f;

    gRimLightColorLocation = glGetUniformLocation(shaderProgramLocation, "gRimLight.color");
    gRimLightRimExponentialLocation = glGetUniformLocation(shaderProgramLocation, "gRimLight.rimExponential");

    if (gRimLightColorLocation == 0xffffffff ||
        gRimLightRimExponentialLocation == 0xffffffff) {
        fprintf(stderr, "Warning! Unable to get the location of the rim light\n");
    }
}

void setRimLight(RimLight &light) {
    glUniform3f(gRimLightColorLocation, light.color.x, light.color.y, light.color.z);
    glUniform1f(gRimLightRimExponentialLocation, light.rimExponential);
}


// --------------------------------------------------------
// gui
// --------------------------------------------------------
void showGUI() {

    static bool bla = true;
    static bool menu = true;

    ImGui::Begin("About ImGui", &bla, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Select Light source");
    const char* items[] = {"Directional Light", "Point Light"};
    static int currentItem = 0;
    static int oldItem = 0;

    ImGui::Combo("combo", &currentItem, items, 2);
    if (currentItem == 0) {
        if (currentItem != oldItem) setupDirectionalLight();
        ImGui::Text("Ambient Light");
        ImGui::SliderFloat("r", &m_directionalLight.color.x, 0.0f, 1.0f);
        ImGui::SliderFloat("g", &m_directionalLight.color.y, 0.0f, 1.0f);
        ImGui::SliderFloat("b", &m_directionalLight.color.z, 0.0f, 1.0f);
        ImGui::SliderFloat("alpha", &m_directionalLight.ambientIntensity, 0.0f, 1.0f);
        ImGui::Text("Diffuse Light");
        ImGui::SliderFloat("diffuse intensity", &m_directionalLight.diffuseIntensity, 0.0f, 1.0f);
        ImGui::Text("Specular Light");
        ImGui::SliderFloat("specular intensity", &m_material.m_specularIntensity, 0.0f, 1.0f);
        ImGui::SliderFloat("specular power", &m_material.m_specularPower, 0.0f, 50.0f);
    } else if (currentItem == 1) {
        if (currentItem != oldItem) setupPointLight();
        if (currentItem != oldItem) setupRimLight();
        ImGui::Text("Ambient Light");
        ImGui::SliderFloat("r", &m_pointLight.color.x, 0.0f, 1.0f);
        ImGui::SliderFloat("g", &m_pointLight.color.y, 0.0f, 1.0f);
        ImGui::SliderFloat("b", &m_pointLight.color.z, 0.0f, 1.0f);
        ImGui::SliderFloat("alpha", &m_pointLight.ambientIntensity, 0.0f, 1.0f);
        ImGui::Text("Diffuse Light");
        ImGui::SliderFloat("diffuse intensity", &m_pointLight.diffuseIntensity, 0.0f, 1.0f);
        ImGui::Text("Specular Light");
        ImGui::SliderFloat("specular intensity", &m_material.m_specularIntensity, 0.0f, 1.0f);
        ImGui::SliderFloat("specular power", &m_material.m_specularPower, 0.0f, 50.0f);
        ImGui::SliderFloat("Linear attenuation", &m_pointLight.attenuation.linear, 0.0f, 1.0f);
        ImGui::SliderFloat("Exp attenuation", &m_pointLight.attenuation.exponential, 0.0f, 1.0f);

    }
    ImGui::Text("Rim Light");
    ImGui::SliderFloat("rim r", &m_rimLight.color.x, 0.0f, 1.0f);
    ImGui::SliderFloat("rim g", &m_rimLight.color.y, 0.0f, 1.0f);
    ImGui::SliderFloat("rim b", &m_rimLight.color.z, 0.0f, 1.0f);
    ImGui::SliderFloat("rim exp", &m_rimLight.rimExponential, 1.0f, 50.0f);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    oldItem = currentItem;
    ImGui::End();
}



// --------------------------------------------------------
// material
// --------------------------------------------------------
void setupMaterial() {
    m_material.m_specularIntensity = 1.0f;
    m_material.m_specularPower = 32.0f;
}

void setMaterial(Material& material) {
    glUniform1f(gMatSpecularIntensityLocation, material.m_specularIntensity);
    glUniform1f(gMatSpecularPowerLocation, material.m_specularPower);
}



// --------------------------------------------------------
// drawing
// --------------------------------------------------------
/*void createVertexBuffer(unsigned int* pIndices, unsigned int IndexCount) {
    Vertex vertices[4] = { Vertex(glm::vec3(-1.0f, -1.0f, 0.5773f),  glm::vec2(0.0f, 0.0f)),
                           Vertex(glm::vec3(0.0f, -1.0f, -1.15475f), glm::vec2(0.5f, 0.0f)),
                           Vertex(glm::vec3(1.0f, -1.0f, 0.5773f),   glm::vec2(1.0f, 0.0f)),
                           Vertex(glm::vec3(0.0f, 1.0f, 0.0f),       glm::vec2(0.5f, 1.0f)) };

    calculateNormals(pIndices, IndexCount, vertices, 4);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void createIndexBuffer() {
    unsigned int indices[] = { 0, 3, 1,
                               1, 3, 2,
                               2, 3, 0,
                               0, 1, 2 };

    createVertexBuffer(indices, 12);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}*/

void drawGeometry(GLFWwindow* window) {
    // clear everything first
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static float scale = 0.0f;
    scale += 0.01f;

    // create world matrix
    Pipeline p;
    p.scale(.2, .2, .2);
    p.position(.0f, 0.0f, 0.0f);
    p.rotation(0.0f, scale, 0.0f);
    p.perspectiveProjection(30.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.1f, 100.0f);
    m_camera->update();
    p.setCamera(m_camera->getPosition(), m_camera->getTarget(), m_camera->getUp());
    glUniformMatrix4fv(gMVPLocation, 1, GL_FALSE, value_ptr(p.getTransformation()));
    glUniformMatrix4fv(gWorldLocation, 1, GL_FALSE, value_ptr(p.getWorldTransformation()));

    // update light
    switch(m_lightMode) {
        case LightMode::DIRECTIONAL_LIGHT:
            setDirectionalLight(m_directionalLight);
            break;
        case LightMode::POINT_LIGHT:
            setPointLight(m_pointLight);
            break;
    }
    setRimLight(m_rimLight);

    setMaterial(m_material);
    glm::vec3 eyeWorldPos = m_camera->getPosition();
    glUniform3f(gEyeWorldPosLocation, eyeWorldPos.x, eyeWorldPos.y, eyeWorldPos.z);

    // draw geometry
    m_mesh->render();
}



// --------------------------------------------------------
// main loop
// --------------------------------------------------------
int main() {

    // init glfw
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(1);
    }

    // set error callback function
    glfwSetErrorCallback(errorCallback);

    // create glfw window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGLTutorial", NULL, NULL);
    if (window) {

        // set glfw context and relevant callbacks
        glfwMakeContextCurrent(window);

        // setup ImGui binding
        ImGui_ImplGlfwGL3_Init(window, true);

        // set glfw callbacks
        glfwSetWindowSizeCallback(window, resizeCallback);
        glfwSetKeyCallback(window, keyCallback);
        glfwSetCursorPosCallback(window, cursorCallback);
        glfwSetMouseButtonCallback(window, mouseBtnCallback);
        glfwSetScrollCallback(window, mouseWheelCallback);

        // initialize gl3w in order being able to use the opengl features
        if (!gl3wInit()) {

            if (!gl3wIsSupported(3,2)) {
                std::cout << "OpenGL 3.2 not supported" << std::endl;
                return -1;
            }

            // print opengl version and shader version
            printOpenGLInfo();

            // setup stuff
            glClearColor(0,0,0,0);
            glFrontFace(GL_CCW);
            glCullFace(GL_BACK);
            glEnable(GL_CULL_FACE);
            glEnable(GL_DEPTH_TEST);

            // setup light
            switch(m_lightMode) {
                case LightMode::DIRECTIONAL_LIGHT:
                    setupDirectionalLight();
                    break;
                case LightMode::POINT_LIGHT:
                    setupPointLight();
                    setupRimLight();
                    break;
            }

            // compile shaders
            compileShaders();

            // setup material for lighting
            setupMaterial();

            // setup mesh
            m_mesh = new Mesh();
            if (!m_mesh->loadMesh("./model/bunny/bunny.obj")) {
                return 1;
            }
            m_mesh->printDebugInfo();

            // set initial camera parameters
            glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
            cgf::TrackballCamera camera = cgf::TrackballCamera(window, WINDOW_WIDTH, WINDOW_HEIGHT, 10, cameraTarget);
            m_camera = &camera;

            // run until user closes the window or presses ALT+F4
            while (!glfwWindowShouldClose(window)) {

                // let glfw check for key events
                glfwPollEvents();

                // display GUI
                ImGui_ImplGlfwGL3_NewFrame();
                showGUI();

                // actually drawing stuff
                drawGeometry(window);

                // render gui
                ImGui::Render();

                // swap buffers after new frame is filled successfully
                glfwSwapBuffers(window);

            }
        }
    }

    // shutdown imgui
    ImGui_ImplGlfwGL3_Shutdown();
    // terminate glfw window
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;

}