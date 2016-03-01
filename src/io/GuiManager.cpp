//
// Created by Windrian on 08.12.2015.
//

#include "GuiManager.h"

cgf::GuiManager::GuiManager(GLFWwindow* window) {
    ImGui_ImplGlfwGL3_Init(window, true);
    m_doesConsumeInput = true;
}

cgf::GuiManager::~GuiManager() {
    ImGui_ImplGlfwGL3_Shutdown();
}

void extract(std::string label, int* p_val) {
    ImGui::DragInt(label.c_str(), p_val, 1);
}

void extract(std::string label, float* p_val) {
    ImGui::DragFloat(label.c_str(), p_val, 1);
}

void extract(std::string label, glm::vec3* p_val) {
    float vec3[3];
    vec3[0] = p_val->x;
    vec3[1] = p_val->y;
    vec3[2] = p_val->z;
    ImGui::InputFloat3(label.c_str(), vec3);
    p_val->x = vec3[0];
    p_val->y = vec3[1];
    p_val->z = vec3[2];
}

void cgf::GuiManager::render() {

    static bool showWindow = false;

    // set new frame
    ImGui_ImplGlfwGL3_NewFrame();

    ImGui::Begin("Menu", &showWindow, ImGuiWindowFlags_AlwaysAutoResize);
    // iterate over all property collections
    for (cgf::PropertiesCollection* p_propertiesCollection : m_propertiesCollections) {
        if(ImGui::CollapsingHeader(p_propertiesCollection->getName().c_str())) {
            // iterate over all property objects in the collection
            for (PropertiesObject p_propertiesObject : p_propertiesCollection->getPropertyObjects()) {
                switch (p_propertiesObject.m_type) {
                    case PropertiesObject::PropertyType::INT :
                        extract(p_propertiesObject.m_label, static_cast<int*>(p_propertiesObject.mp_value));
                        break;
                    case PropertiesObject::PropertyType::FLOAT :
                        extract(p_propertiesObject.m_label, static_cast<float*>(p_propertiesObject.mp_value));
                        break;
                    case PropertiesObject::PropertyType::VEC3 :
                        extract(p_propertiesObject.m_label, static_cast<glm::vec3*>(p_propertiesObject.mp_value));
                        break;
                }
            }
        }
    }
    ImGui::End();

    // finally show gui
    ImGui::Render();
}

void cgf::GuiManager::addPropertiesCollection(cgf::PropertiesCollection* p_propertiesCollection) {
    std::cout << "add properties collection" << std::endl;
    m_propertiesCollections.push_back(p_propertiesCollection);
    std::cout << "add properties collection end" << std::endl;
}

void cgf::GuiManager::removePropertiesCollection(cgf::PropertiesCollection *p_propertiesCollection) {
    for( std::vector<cgf::PropertiesCollection *>::iterator iter = m_propertiesCollections.begin(); iter != m_propertiesCollections.end(); ++iter ) {
        if( *iter == p_propertiesCollection) {
            m_propertiesCollections.erase( iter );
            break;
        }
    }
}

bool cgf::GuiManager::onKeyboard(int key) {
    if (m_doesConsumeInput) {
        ImGuiIO& io = ImGui::GetIO();
        return io.WantCaptureKeyboard;
    }
    return false;
}

bool cgf::GuiManager::onMouse(int x, int y) {
    if (m_doesConsumeInput) {
        ImGuiIO& io = ImGui::GetIO();
        return io.WantCaptureMouse;
    }
    return false;
}

bool cgf::GuiManager::onMouseScroll(double xOffset, double yOffset) {
    if (m_doesConsumeInput) {

    }
    return false;
}

bool cgf::GuiManager::onMouseButton(int button, int action) {
    if (m_doesConsumeInput) {

    }
    return false;
}