// standard headers
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <string>

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
#include "Pipeline.h"
#include "Camera.h"
#include "TrackballCamera.h"
#include "Texture.h"


// --------------------------------------------------------
// global variables
// --------------------------------------------------------
const char* p__vertexShaderFileName = "shader/shader.vert";
const char* p__fragmentShaderFileName = "shader/shader.frag";
GLuint VBO;
GLuint IBO;
GLuint gWorldLocation;
GLuint gSampler;
cgf::Texture* pTexture = NULL;
cgf::Camera* m_camera;

int WINDOW_WIDTH = 1024;
int WINDOW_HEIGHT = 768;

struct Vertex {
    glm::vec3 m_pos;
    glm::vec2 m_tex;

    Vertex() {}

    Vertex(glm::vec3 pos, glm::vec2 tex)
    {
        m_pos = pos;
        m_tex = tex;
    }
};



// --------------------------------------------------------
// glfw callbacks
// --------------------------------------------------------
void errorCallback(int error, const char* description) {
    fputs(description, stderr);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } else {
        m_camera->onKeyboard(key);
    }
}

void cursorCallback(GLFWwindow* window, double xPos, double yPos) {
    m_camera->onMouse(xPos, yPos);
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
// gui
// --------------------------------------------------------
void showGUI() {
    {
        ImVec4 clear_color = ImColor(1, 1, 1);
        static float f = 0.0f;
        ImGui::Text("Hello, world!");
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::ColorEdit3("clear color", (float*)&clear_color);
        if (ImGui::Button("Test Window")) {
            std::cout << "Button was pressed" << std::endl;
        }
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}



// --------------------------------------------------------
// drawing
// --------------------------------------------------------
void createVertexBuffer() {
    Vertex vertices[4] = { Vertex(glm::vec3(-1.0f, -1.0f, 0.5773f),  glm::vec2(0.0f, 0.0f)),
                           Vertex(glm::vec3(0.0f, -1.0f, -1.15475f), glm::vec2(0.5f, 0.0f)),
                           Vertex(glm::vec3(1.0f, -1.0f, 0.5773f),   glm::vec2(1.0f, 0.0f)),
                           Vertex(glm::vec3(0.0f, 1.0f, 0.0f),       glm::vec2(0.5f, 1.0f)) };


    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void createIndexBuffer() {
    unsigned int indices[] = { 0, 3, 1,
                               1, 3, 2,
                               2, 3, 0,
                               0, 1, 2 };

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void drawGeometry(GLFWwindow* window) {
    // clear everything first
    glClear(GL_COLOR_BUFFER_BIT);

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
    glUniformMatrix4fv(gWorldLocation, 1, GL_FALSE, value_ptr(p.getTransformation()));

    // update attribute
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    pTexture->bind(GL_TEXTURE0);

    // draw stuff
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    // disable attributes after they have been used
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}



// --------------------------------------------------------
// util
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



// --------------------------------------------------------
// setup shader
// --------------------------------------------------------
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
    if(shaderProgram == 0) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

    // load shaders from external files
    std::string vs, fs;
    if(!readFile(p__vertexShaderFileName, vs)) {
        exit(1);
    }
    if(!readFile(p__fragmentShaderFileName, fs)) {
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
    gWorldLocation = glGetUniformLocation(shaderProgram, "gWorld");
    if(gWorldLocation == 0xFFFFFFFF) {
        fprintf(stderr, "Cannot find location of property within the shaders");
        exit(1);
    }

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
        glfwSetKeyCallback(window, keyCallback);
        glfwSetCursorPosCallback(window, cursorCallback);
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
            glFrontFace(GL_CW);
            glCullFace(GL_BACK);
            glEnable(GL_CULL_FACE);
            createVertexBuffer();
            createIndexBuffer();
            compileShaders();

            // set sampler and load texture
            glUniform1i(gSampler, 0);
            pTexture = new cgf::Texture(GL_TEXTURE_2D, "textures/test.png");
            if (!pTexture->load()) {
                return 1;
            }

            // set initial camera parameters
            glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
            cgf::TrackballCamera camera = cgf::TrackballCamera(window, WINDOW_WIDTH, WINDOW_HEIGHT, 10, cameraTarget);
            m_camera = &camera;

            // setup ImGui binding
            ImGui_ImplGlfwGL3_Init(window, false);

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