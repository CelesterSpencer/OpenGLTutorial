//
// Created by Windrian on 08.11.2015.
//

#include "Pipeline.h"

#define M_PI 3.14159265358979323846
#define toRadian(x) ((x) * M_PI / 180.0f)
#define toDegree(x) ((x) * 180.0f / M_PI)



void Pipeline::scale(float scaleX, float scaleY, float scaleZ) {
    m_scale.x = scaleX;
    m_scale.y = scaleY;
    m_scale.z = scaleZ;
}

void Pipeline::position(float x, float y, float z) {
    m_position.x = x;
    m_position.y = y;
    m_position.z = z;
}

void Pipeline::rotation(float rotateX, float rotateY, float rotateZ) {
    m_rotation.x = rotateX;
    m_rotation.y = rotateY;
    m_rotation.z = rotateZ;
}

void Pipeline::perspectiveProjection(float fov, float width, float height, float zNear, float zFar) {
    m_fov = fov;
    m_width = width;
    m_height = height;
    m_zNear = zNear;
    m_zFar = zFar;
}



void Pipeline::initScaleTransform(glm::mat4x4 &transform, float scaleX, float scaleY, float scaleZ) {
    transform[0][0] = scaleX; transform[0][1] = 0.0f;   transform[0][2] = 0.0f;   transform[0][3] = 0.0f;
    transform[1][0] = 0.0f;   transform[1][1] = scaleY; transform[1][2] = 0.0f;   transform[1][3] = 0.0f;
    transform[2][0] = 0.0f;   transform[2][1] = 0.0f;   transform[2][2] = scaleZ; transform[2][3] = 0.0f;
    transform[3][0] = 0.0f;   transform[3][1] = 0.0f;   transform[3][2] = 0.0f;   transform[3][3] = 1.0f;
}

void Pipeline::initRotationTransform(glm::mat4x4 &transform, float rotateX, float rotateY, float rotateZ)  {
    glm::mat4x4 rx, ry, rz;

    const float x = toRadian(rotateX);
    const float y = toRadian(rotateY);
    const float z = toRadian(rotateZ);

    rx[0][0] = 1.0f; rx[0][1] = 0.0f   ; rx[0][2] = 0.0f    ; rx[0][3] = 0.0f;
    rx[1][0] = 0.0f; rx[1][1] = cosf(x); rx[1][2] = -sinf(x); rx[1][3] = 0.0f;
    rx[2][0] = 0.0f; rx[2][1] = sinf(x); rx[2][2] = cosf(x) ; rx[2][3] = 0.0f;
    rx[3][0] = 0.0f; rx[3][1] = 0.0f   ; rx[3][2] = 0.0f    ; rx[3][3] = 1.0f;

    ry[0][0] = cosf(y); ry[0][1] = 0.0f; ry[0][2] = -sinf(y); ry[0][3] = 0.0f;
    ry[1][0] = 0.0f   ; ry[1][1] = 1.0f; ry[1][2] = 0.0f    ; ry[1][3] = 0.0f;
    ry[2][0] = sinf(y); ry[2][1] = 0.0f; ry[2][2] = cosf(y) ; ry[2][3] = 0.0f;
    ry[3][0] = 0.0f   ; ry[3][1] = 0.0f; ry[3][2] = 0.0f    ; ry[3][3] = 1.0f;

    rz[0][0] = cosf(z); rz[0][1] = -sinf(z); rz[0][2] = 0.0f; rz[0][3] = 0.0f;
    rz[1][0] = sinf(z); rz[1][1] = cosf(z) ; rz[1][2] = 0.0f; rz[1][3] = 0.0f;
    rz[2][0] = 0.0f   ; rz[2][1] = 0.0f    ; rz[2][2] = 1.0f; rz[2][3] = 0.0f;
    rz[3][0] = 0.0f   ; rz[3][1] = 0.0f    ; rz[3][2] = 0.0f; rz[3][3] = 1.0f;

    transform = rz * ry * rx;
}

void Pipeline::initTranslationTransform(glm::mat4x4 &transform, float x, float y, float z) {
    transform[0][0] = 1.0f; transform[0][1] = 0.0f; transform[0][2] = 0.0f; transform[0][3] = x;
    transform[1][0] = 0.0f; transform[1][1] = 1.0f; transform[1][2] = 0.0f; transform[1][3] = y;
    transform[2][0] = 0.0f; transform[2][1] = 0.0f; transform[2][2] = 1.0f; transform[2][3] = z;
    transform[3][0] = 0.0f; transform[3][1] = 0.0f; transform[3][2] = 0.0f; transform[3][3] = 1.0f;
}

void Pipeline::initPerspectiveProjection(glm::mat4x4& m) {
    float ar = m_width/m_height;
    float zNear = m_zNear;
    float zFar = m_zFar;
    float zRange = zNear - zFar;
    float tanHalfFOV = tanf(toRadian(m_fov/2.0));

    m[0][0] = 1.0f / (tanHalfFOV * ar);
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[0][3] = 0.0f;

    m[1][0] = 0.0f;
    m[1][1] = 1.0f / tanHalfFOV;
    m[1][2] = 0.0f;
    m[1][3] = 0.0f;

    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = (-zNear - zFar) / zRange;
    m[2][3] = 2.0f * zFar * zNear / zRange;

    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = 1.0f;
    m[3][3] = 0.0f;
}





glm::mat4x4 Pipeline::getTransformation() {
    glm::mat4x4 scaleTransform, rotationTransform, translationTransform, perspectiveProjectionTransform;
    initScaleTransform(scaleTransform, m_scale.x, m_scale.y, m_scale.z);
    initRotationTransform(rotationTransform, m_rotation.x, m_rotation.y, m_rotation.z);
    initTranslationTransform(translationTransform, m_position.x, m_position.y, m_position.z);
    initPerspectiveProjection(perspectiveProjectionTransform);

    m_WTransform = perspectiveProjectionTransform * translationTransform * rotationTransform * scaleTransform;
    return m_WTransform;
}