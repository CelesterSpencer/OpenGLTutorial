//
// Created by Windrian on 08.11.2015.
//

#ifndef SRCCMAKE_PIPELINE_H
#define SRCCMAKE_PIPELINE_H



// includes
#include <glm.hpp>
#define _USE_MATH_DEFINES
#include <math.h>


class Pipeline {

public:
    void scale(float scaleX, float scaleY, float scaleZ);
    void position(float x, float y, float z);
    void rotation(float rotateX, float rotateY, float rotateZ);
    glm::mat4x4 getTransformation();

private:
    glm::vec3 m_scale;
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec4 m_transformation;

    glm::mat4x4 m_WTransform;

    void initScaleTransform(glm::mat4x4 &transform, float scaleX, float scaleY, float scaleZ);
    void initRotationTransform(glm::mat4x4 &transform, float rotateX, float rotateY, float rotateZ);
    void initTranslationTransform(glm::mat4x4 &transform, float x, float y, float z);
};


#endif //SRCCMAKE_PIPELINE_H