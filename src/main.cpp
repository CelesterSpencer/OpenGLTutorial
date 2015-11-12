// standard headers
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <string>

// glew, include it before gl.h and glfw.h
#define GLEW_STATIC
#include <GL/glew.h>

// glfw
#include <GLFW/glfw3.h>

// glm
#include <glm.hpp>
#include <gtc/type_ptr.hpp>

// internal includes
#include "Pipeline.h"


// --------------------------------------------------------
// global variables
// --------------------------------------------------------
const char* p__vertexShaderFileName = "shader/shader.vert";
const char* p__fragmentShaderFileName = "shader/shader.frag";
GLuint VBO;
GLuint IBO;
GLuint gWorldLocation;

int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;



// --------------------------------------------------------
// glfw callbacks
// --------------------------------------------------------
void errorCallback(int error, const char* description) {
    fputs(description, stderr);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}



// --------------------------------------------------------
// debug
// --------------------------------------------------------
void printOpenGLInfo() {
    // check opengl version
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version: %s\n", version);
}



// --------------------------------------------------------
// drawing
// --------------------------------------------------------
void createVertexBuffer() {
    glm::vec3 vertices[4];
    vertices[0] = glm::vec3(-1.0f, -1.0f, 0.0f);
    vertices[1] = glm::vec3(0.0f, -1.0f, 1.0f);
    vertices[2] = glm::vec3(1.0f, -1.0f, 0.0f);
    vertices[3] = glm::vec3(0.0f, 1.0f, 0.0f);

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

    static float scale = 1.0f;
    scale += 0.001f;

    // create world matrix
    Pipeline p;
    p.scale(sinf(scale * 0.1f), sinf(scale * 0.1f), sinf(scale * 0.1f));
    p.position(0.0f, 0.0f, sinf(scale));
    p.rotation(sinf(scale) * 90.0f, sinf(scale) * 90.0f, sinf(scale) * 90.0f);
    p.perspectiveProjection(30.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 1000.0f);
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, glm::value_ptr(p.getTransformation()));

    // update attribute
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // draw stuff
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    // swap buffers after new frame is filled successfully
    glfwSwapBuffers(window);
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
        glfwMakeContextCurrent(window);
        glfwSetKeyCallback(window, keyCallback);

        // initialize glew in order being able to use the opengl features
        glewExperimental = GL_TRUE;
        GLenum res = glewInit();
        if (res == GLEW_OK) {

            // print opengl version and shader version
            printOpenGLInfo();

            // setup stuff
            glClearColor(0,0,0,0);
            createVertexBuffer();
            createIndexBuffer();
            compileShaders();

            static float scale = 0.0f;

            // run until user closes the window or presses ALT+F4
            while (!glfwWindowShouldClose(window)) {

                // manipulate geometry per frame
                scale += 0.001f;

                // actually drawing stuff
                drawGeometry(window);

                // disable vertex attribute when it is not immediately used by the shader
                glDisableVertexAttribArray(0);

                // let glfw check for key events
                glfwPollEvents();
            }
        }
    }

    // terminate glfw window
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;

}