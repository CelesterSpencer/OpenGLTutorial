//
// Created by Windrian on 09.12.2015.
//

#include "InteractionManager.h"


cgf::InteractionManager::InteractionManager() {

}

cgf::InteractionManager::~InteractionManager() {

}

void cgf::InteractionManager::init(GLFWwindow* window) {

    std::function<void (int,int,int,int)> keyCallback = std::bind(
                &cgf::InteractionManager::keyCallback, // the method I want to bind
                this, 								   // the object instance I want to call it from
                std::placeholders::_1,				   // forward arguments
                std::placeholders::_2,
                std::placeholders::_3,
                std::placeholders::_4
    );
    setKeyCallback(window, keyCallback);

    std::function<void (int,int,int)> mouseBtnCallback = std::bind(
            &cgf::InteractionManager::mouseBtnCallback, // the method I want to bind
            this, 								        // the object instance I want to call it from
            std::placeholders::_1,				        // forward arguments
            std::placeholders::_2,
            std::placeholders::_3
    );
    setMouseButtonCallback(window, mouseBtnCallback);

    std::function<void (double, double)> cursorCallback = std::bind(
            &cgf::InteractionManager::cursorCallback, // the method I want to bind
            this, 								      // the object instance I want to call it from
            std::placeholders::_1,				      // forward arguments
            std::placeholders::_2
    );
    setCursorPosCallback(window, cursorCallback);

    std::function<void (double, double)> scrollCallback = std::bind(
            &cgf::InteractionManager::mouseWheelCallback, // the method I want to bind
            this, 								          // the object instance I want to call it from
            std::placeholders::_1,				          // forward arguments
            std::placeholders::_2
    );
    setScrollCallback(window, scrollCallback);
}

void cgf::InteractionManager::addInteractable(Interactable* interactable) {
    m_interactables.push_back(interactable);
}



void cgf::InteractionManager::setKeyCallback(GLFWwindow* window, std::function<void (int, int, int, int)> func) {
    static std::function<void (int, int, int, int)> func_bounce = func;
    glfwSetKeyCallback(window, [] (GLFWwindow* w, int k, int s, int a, int m) {
        func_bounce(k, s, a, m);
    });
}

void cgf::InteractionManager::setMouseButtonCallback(GLFWwindow* window, std::function<void (int, int, int)> func) {
    static std::function<void (int, int, int)> func_bounce = func;
    glfwSetMouseButtonCallback(window, [] (GLFWwindow* w, int b, int a, int m) {
        func_bounce(b, a, m);
    });
}

void cgf::InteractionManager::setCursorPosCallback(GLFWwindow* window, std::function<void (double, double)> func) {
    static std::function<void (double, double)> func_bounce = func;
    glfwSetCursorPosCallback(window, [] (GLFWwindow* w, double x, double y) {
        func_bounce(x, y);
    });
}

void cgf::InteractionManager::setScrollCallback(GLFWwindow* window, std::function<void (double, double)> func) {
    static std::function<void (double, double)> func_bounce = func;
    glfwSetScrollCallback(window, [] (GLFWwindow* w, double x, double y) {
        func_bounce(x, y);
    });
}



void cgf::InteractionManager::keyCallback(int key, int scancode, int action, int mods) {
    // continue until one interactable consumed the input or none of the interactable consumed it
    for (Interactable* interactable : m_interactables) {
        if (interactable->onKeyboard(key)) return;
    }
}

void cgf::InteractionManager::mouseBtnCallback(int button, int action, int mods) {
    // continue until one interactable consumed the input or none of the interactable consumed it
    for (Interactable* interactable : m_interactables) {
        if (interactable->onMouseButton(button, action)) return;
    }
}

void cgf::InteractionManager::cursorCallback(double xPos, double yPos) {
    // continue until one interactable consumed the input or none of the interactable consumed it
    for (Interactable* interactable : m_interactables) {
        if (interactable->onMouse(xPos, yPos)) return;
    }
}

void cgf::InteractionManager::mouseWheelCallback(double xOffset, double yOffset) {
    // continue until one interactable consumed the input or none of the interactable consumed it
    for (Interactable* interactable : m_interactables) {
        if (interactable->onMouseScroll(xOffset, yOffset)) return;
    }
}