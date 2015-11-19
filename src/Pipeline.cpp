//
// Created by Windrian on 08.11.2015.
//

#include "Pipeline.h"



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
    transform[0][0] = scaleX; transform[1][0] = 0.0f;   transform[2][0] = 0.0f;   transform[3][0] = 0.0f;
    transform[0][1] = 0.0f;   transform[1][1] = scaleY; transform[2][1] = 0.0f;   transform[3][1] = 0.0f;
    transform[0][2] = 0.0f;   transform[1][2] = 0.0f;   transform[2][2] = scaleZ; transform[3][2] = 0.0f;
    transform[0][3] = 0.0f;   transform[1][3] = 0.0f;   transform[2][3] = 0.0f;   transform[3][3] = 1.0f;
}

void Pipeline::initRotationTransform(glm::mat4x4 &transform, float rotateX, float rotateY, float rotateZ)  {
    glm::mat4x4 rx, ry, rz;

    const float x = glm::radians(rotateX);
    const float y = glm::radians(rotateY);
    const float z = glm::radians(rotateZ);

    rx[0][0] = 1.0f; rx[1][0] = 0.0f       ; rx[2][0] = 0.0f        ; rx[3][0] = 0.0f;
    rx[0][1] = 0.0f; rx[1][1] = glm::cos(x); rx[2][1] = -glm::sin(x); rx[3][1] = 0.0f;
    rx[0][2] = 0.0f; rx[1][2] = glm::sin(x); rx[2][2] = glm::cos(x) ; rx[3][2] = 0.0f;
    rx[0][3] = 0.0f; rx[1][3] = 0.0f       ; rx[2][3] = 0.0f        ; rx[3][3] = 1.0f;

    ry[0][0] = glm::cos(y) ; ry[1][0] = 0.0f; ry[2][0] = glm::sin(y) ; ry[3][0] = 0.0f;
    ry[0][1] = 0.0f        ; ry[1][1] = 1.0f; ry[2][1] = 0.0f        ; ry[3][1] = 0.0f;
    ry[0][2] = -glm::sin(y); ry[1][2] = 0.0f; ry[2][2] = glm::cos(y) ; ry[3][2] = 0.0f;
    ry[0][3] = 0.0f        ; ry[1][3] = 0.0f; ry[2][3] = 0.0f        ; ry[3][3] = 1.0f;

    rz[0][0] = glm::cos(z); rz[1][0] = -glm::sin(z); rz[2][0] = 0.0f; rz[3][0] = 0.0f;
    rz[0][1] = glm::sin(z); rz[1][1] = glm::cos(z) ; rz[2][1] = 0.0f; rz[3][1] = 0.0f;
    rz[0][2] = 0.0f       ; rz[1][2] = 0.0f        ; rz[2][2] = 1.0f; rz[3][2] = 0.0f;
    rz[0][3] = 0.0f       ; rz[1][3] = 0.0f        ; rz[2][3] = 0.0f; rz[3][3] = 1.0f;

    transform = rz * ry * rx;
}

void Pipeline::initTranslationTransform(glm::mat4x4 &transform, float x, float y, float z) {
    transform[0][0] = 1.0f; transform[1][0] = 0.0f; transform[2][0] = 0.0f; transform[3][0] = x;
    transform[0][1] = 0.0f; transform[1][1] = 1.0f; transform[2][1] = 0.0f; transform[3][1] = y;
    transform[0][2] = 0.0f; transform[1][2] = 0.0f; transform[2][2] = 1.0f; transform[3][2] = z;
    transform[0][3] = 0.0f; transform[1][3] = 0.0f; transform[2][3] = 0.0f; transform[3][3] = 1;
}

void Pipeline::initPerspectiveProjection(glm::mat4x4& m) {
    float ar = m_width/m_height;
    float zNear = m_zNear;
    float zFar = m_zFar;
    float zRange = zFar - zNear;
    float tanHalfFOV = glm::tan((m_fov/2.0f));

    m[0][0] = 1.0f / (tanHalfFOV * ar); m[1][0] = 0.0f;                 m[2][0] = 0.0f;                     m[3][0] = 0.0f;
    m[0][1] = 0.0f;                     m[1][1] = 1.0f / tanHalfFOV;    m[2][1] = 0.0f;                     m[3][1] = 0.0f;
    m[0][2] = 0.0f;                     m[1][2] = 0.0f;                 m[2][2] = -(zNear + zFar) / zRange; m[3][2] = -(2.0f * zFar * zNear) / zRange;
    m[0][3] = 0.0f;                     m[1][3] = 0.0f;                 m[2][3] = -1.0f;                    m[3][3] = 0.0f;
}

void Pipeline::initOrthographicProjection(glm::mat4x4& m) {
    float zNear = m_zNear;
    float zFar = m_zFar;
    float zRange = zNear - zFar;

    m[0][0] = 2.0f / m_width;           m[1][0] = 0.0f;                 m[2][0] = 0.0f;                     m[3][0] = 0.0f;
    m[0][1] = 0.0f;                     m[1][1] = 2.0f / m_height;      m[2][1] = 0.0f;                     m[3][1] = 0.0f;
    m[0][2] = 0.0f;                     m[1][2] = 0.0f;                 m[2][2] = 2.0f / zRange;            m[3][2] = (-zFar - zNear) / zRange;
    m[0][3] = 0.0f;                     m[1][3] = 0.0f;                 m[2][3] = 0.0f;                     m[3][3] = 1.0f;
}

void Pipeline::setCamera(glm::vec3 pos, glm::vec3 target, glm::vec3 up) {
    m_camera.pos = pos;
    m_camera.target = target;
    m_camera.up = up;
}

void Pipeline::initCameraTransform(glm::mat4x4& transform, const glm::vec3& view, const glm::vec3& up) {

    glm::vec3 z = view;
    z = glm::normalize(z);

    glm::vec3 y = up;
    y = glm::normalize(y);
    glm::vec3 x = glm::cross(y, z);
    x = glm::normalize(x);

    y = glm::cross(z, x);

    transform[0][0] = x.x; transform[1][0] = x.y; transform[2][0] = x.z; transform[3][0] = 0.0f;
    transform[0][1] = y.x; transform[1][1] = y.y; transform[2][1] = y.z; transform[3][1] = 0.0f;
    transform[0][2] = z.x; transform[1][2] = z.y; transform[2][2] = z.z; transform[3][2] = 0.0f;
    transform[0][3] = 0.0f;transform[1][3] = 0.0f;transform[2][3] = 0.0f;transform[3][3] = 1.0f;

}



glm::mat4x4 Pipeline::getTransformation() {

    glm::mat4x4 scale, rotate, translate, cameraRot, cameraTrans, persProj;
/*    scale = glm::scale(glm::mat4x4(1.0f), m_scale);
    glm::mat4x4 rotateX = glm::rotate(glm::mat4x4(1.0f), m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4x4 rotateY = glm::rotate(glm::mat4x4(1.0f), m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4x4 rotateZ = glm::rotate(glm::mat4x4(1.0f), m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    rotate = rotateZ * rotateY * rotateX;
    translate = glm::translate(glm::mat4x4(1.0f), m_position);
    persProj = glm::perspective(m_fov, m_width/ m_height, m_zNear, m_zFar);
    cameraRot = glm::lookAt(m_camera.pos, m_camera.target, m_camera.up);
    Util::printMat(cameraRot, "lookat glm");*/

    initScaleTransform(scale, m_scale.x, m_scale.y, m_scale.z);
    initRotationTransform(rotate, m_rotation.x, m_rotation.y, m_rotation.z);
    initTranslationTransform(translate, m_position.x, m_position.y, m_position.z);
    initTranslationTransform(cameraTrans, -m_camera.pos.x, -m_camera.pos.y, -m_camera.pos.z);
    initCameraTransform(cameraRot, m_camera.pos - m_camera.target, m_camera.up);
    initPerspectiveProjection(persProj);
    persProj = glm::perspective(glm::radians(m_fov), m_width/ m_height, m_zNear, m_zFar);

    m_WTransform = persProj * cameraRot * cameraTrans *  translate * rotate * scale;

    return m_WTransform;
}