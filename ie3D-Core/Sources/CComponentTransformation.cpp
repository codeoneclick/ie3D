//
//  CComponentTransformation.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 1/22/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "CComponentTransformation.h"
#include "CCamera.h"

CComponentTransformation::CComponentTransformation(void) :
m_isMMatrixComputed(false)
{
    CComponentTransformation::setPosition(glm::vec3(0.0f));
    CComponentTransformation::setRotation(glm::vec3(0.0f));
    CComponentTransformation::setScale(glm::vec3(1.0f));
}

CComponentTransformation::~CComponentTransformation(void)
{
    
}

E_COMPONENT_CLASS CComponentTransformation::getClass(void) const
{
    return E_COMPONENT_CLASS_TRANSFORMATION;
}

void CComponentTransformation::setPosition(const glm::vec3& position)
{
    m_position = position;
    m_matrixT = glm::translate(glm::mat4(1.0f), m_position);
    m_isMMatrixComputed = false;
}

void CComponentTransformation::setRotation(const glm::vec3& rotation)
{
    m_rotation = rotation;
    m_matrixR = glm::rotate(glm::mat4(1.0f), m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    m_matrixR = glm::rotate(m_matrixR, m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    m_matrixR = glm::rotate(m_matrixR, m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    m_isMMatrixComputed = false;
}
void CComponentTransformation::setScale(const glm::vec3& scale)
{
    m_scale = scale;
    m_matrixS = glm::scale(glm::mat4(1.0f), m_scale);
    m_isMMatrixComputed = false;
}

glm::vec3 CComponentTransformation::getPosition(void) const
{
    return m_position;
}

glm::vec3 CComponentTransformation::getRotation(void) const
{
    return m_rotation;
}

glm::vec3 CComponentTransformation::getScale(void) const
{
    return m_scale;
}

glm::mat4 CComponentTransformation::getMMatrix(CSharedComponentTransformationRef componentTransformation)
{
    if(!componentTransformation->m_isMMatrixComputed)
    {
        componentTransformation->m_matrixM = componentTransformation->m_matrixT * componentTransformation->m_matrixR * componentTransformation->m_matrixS;
        componentTransformation->m_isMMatrixComputed = true;
    }
    return componentTransformation->m_matrixM;
}

glm::mat4 CComponentTransformation::getMVPMatrix(CSharedComponentTransformationRef componentTransformation, CSharedCameraRef camera)
{
    glm::mat4 matrixMVP = camera->getVPMatrix() * CComponentTransformation::getMMatrix(componentTransformation);
    return std::move(matrixMVP);
}

glm::mat4 CComponentTransformation::getIMVPMatrix(CSharedComponentTransformationRef componentTransformation, CSharedCameraRef camera)
{
    glm::mat4 m_matrixIMVP = camera->getIVPMatrix() * CComponentTransformation::getMMatrix(componentTransformation);
    return std::move(m_matrixIMVP);
}

glm::vec3 CComponentTransformation::getForward(void) const
{
    glm::vec4 vector = m_matrixM[2];
    vector = glm::normalize(vector);
    return glm::vec3(vector.x, vector.y, vector.z);
}

glm::vec3 CComponentTransformation::getUp(void) const
{
    glm::vec4 vector = m_matrixM[1];
    vector = glm::normalize(vector);
    return glm::vec3(vector.x, vector.y, vector.z);
}

glm::vec3 CComponentTransformation::getRight(void) const
{
    glm::vec4 vector = m_matrixM[0];
    vector = glm::normalize(vector);
    return glm::vec3(vector.x, vector.y, vector.z);
}
