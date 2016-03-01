// standard headers
#include <stdio.h>
#include <fstream>

// glew, include it before gl.h and glfw.h
#include <GL/gl3w.h>

// glfw
#include <GLFW/glfw3.h>

// glm
#include <glm.hpp>
#include <gtc/type_ptr.hpp>

// internal includes
#include "scene/Pipeline.h"
#include "scene/camera/Camera.h"
#include "scene/camera/TrackballCamera.h"
#include "scene/geometry/Texture.h"
#include "scene/geometry/Mesh.h"
#include "scene/light/PointLight.h"
#include "scene/geometry/material/BasicMaterial.h"
#include "scene/Scene.h"
#include "io/GuiManager.h"
#include "InteractionManager.h"
#include "ShaderManager.h"
#include "scene/geometry/skybox/SkyBox.h"


// window
float WINDOW_WIDTH = 1024;
float WINDOW_HEIGHT = 768;

//_________________________________________________//
//_________________HelperMethods___________________//
//_________________________________________________//
void errorCallback(int error, const char* description) {
    fputs(description, stderr);
}

void resizeCallback(GLFWwindow* window, int width, int height) {
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
    glViewport(0,0, width, height);
}

GLFWwindow* initGlfw() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(1);
    }
    // set error callback function
    glfwSetErrorCallback(errorCallback);

    // multisampling
    glfwWindowHint(GLFW_SAMPLES, 1);


    // create glfw window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGLTutorial", NULL, NULL);
    if (!window) {
        fprintf(stderr, "GLFW Window couldn't be initialized\n");
        exit(1);
    }

    // set glfw context and relevant callbacks
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, resizeCallback);
    return window;
}

void initGl3w() {
    if (gl3wInit()) {
        fprintf(stderr, "GLEW couldn't be initialized\n");
        exit(1);
    }

    // check for glew features
    if (!gl3wIsSupported(3,2)) {
        std::cout << "OpenGL 3.2 not supported" << std::endl;
        exit(1);
    }

    glEnable(GL_MULTISAMPLE);

}

void printOpenGLInfo() {
    // check opengl version
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void setOpenGLFeatues() {
    glClearColor(0,0,0,0);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}



int main() {

    //_________________________________________________//
    //_________________Initalization___________________//
    //_________________________________________________//

    // init glfw
    GLFWwindow* window = initGlfw();

    // initialize gl3w
    initGl3w();



    //_________________________________________________//
    //___________________Setup_________________________//
    //_________________________________________________//

    // OpenGL
    printOpenGLInfo();
    setOpenGLFeatues();

    // compile shaders
    cgf::ShaderManager shaderManager;
    shaderManager.init();
    shaderManager.addShader("shader/shader.vert", GL_VERTEX_SHADER);
    shaderManager.addShader("shader/shader.frag", GL_FRAGMENT_SHADER);
    shaderManager.compileShaders();

    // init scene
    cgf::Scene scene;
    scene.init(shaderManager.getShaderProgramLocation());

    // setup and add light
    cgf::PointLight pointLight("gPointLight", glm::vec3(4, 5, 1), glm::vec3(1,1,1));
    scene.addLight(&pointLight);
    pointLight.init(shaderManager.getShaderProgramLocation());

    // load and add mesh
    cgf::Mesh mesh;
    mesh.loadMesh("./model/bunny/bunny.obj");
    scene.addMesh(&mesh);
    cgf::BasicMaterial material(0.33f, 0.6f, 0.3f, 50.0f);
    material.init(shaderManager.getShaderProgramLocation());

    // setup and add camera
    glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
    cgf::TrackballCamera camera = cgf::TrackballCamera(window, WINDOW_WIDTH, WINDOW_HEIGHT, 20, cameraTarget);
    scene.addCamera(&camera);
    scene.setActiveCamera(0);

    // setup and add pipeline
    Pipeline p;
    p.scale(.2, .2, .2);
    p.position(.0f, 0.0f, 0.0f);
    p.rotation(0.0f, 0.0f, 0.0f);
    p.perspectiveProjection(30.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.1f, 100.0f);
    scene.setPipeline(&p);

    // setup skydome
/*    cgf::SkyBox skyBox(&camera, &shaderManager);
    skyBox.setup("model/skygeometry/",
                 "sp3left.jpg", "sp3right.jpg",
                 "sp3top.jpg", "sp3bot.jpg",
                 "sp3front.jpg", "sp3back.jpg");*/

    // init guimanager
    cgf::GuiManager guiManager(window);
    guiManager.addPropertiesCollection(pointLight.getPropertiesCollection());
    guiManager.addPropertiesCollection(material.getPropertiesCollection());

    // init interactionmanager
    cgf::InteractionManager interactionManager;
    interactionManager.init(window);
    interactionManager.addInteractable(&camera);

    float rotation = 0.0f;
    // run until user closes the window or presses ALT+F4
    while (!glfwWindowShouldClose(window)) {
        // clear everythig first
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // let glfw check for key events
        glfwPollEvents();

        // render scene
        rotation += 0.01f;
        p.rotation(.0f, rotation, 0.0f);
        material.update();
        scene.render();

        // draw gui
        guiManager.render();
        // swap buffers after new frame is filled successfully
        glfwSwapBuffers(window);
    }
    // terminate glfw window
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;

}