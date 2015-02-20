//
//  CComponentTransformation.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 1/22/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "CComponentTransformation.h"
#include "CCamera.h"

CComponentTransformation::CComponentTransformation(void)
{
    CComponentTransformation::setPosition(glm::vec3(0.0f));
    CComponentTransformation::setRotation(glm::vec3(0.0f));
    CComponentTransformation::setScale(glm::vec3(1.0f));
}

CComponentTransformation::~CComponentTransformation(void)
{
    
}

void CComponentTransformation::setCamera(CSharedCameraRef camera)
{
    m_camera = camera;
}

E_COMPONENT_CLASS CComponentTransformation::getClass(void) const
{
    return E_COMPONENT_CLASS_TRANSFORMATION;
}

void CComponentTransformation::setPosition(const glm::vec3& position)
{
    m_position = position;
    m_matrixT = glm::translate(glm::mat4(1.0f), m_position);
    m_isComputed = false;
}

void CComponentTransformation::setRotation(const glm::vec3& rotation)
{
    m_rotation = rotation;
    m_matrixR = glm::rotate(glm::mat4(1.0f), m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    m_matrixR = glm::rotate(m_matrixR, m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    m_matrixR = glm::rotate(m_matrixR, m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    m_isComputed = false;
}
void CComponentTransformation::setScale(const glm::vec3& scale)
{
    m_scale = scale;
    m_matrixS = glm::scale(glm::mat4(1.0f), m_scale);
    m_isComputed = false;
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

glm::mat4 CComponentTransformation::getMMatrix(void)
{
    if(!m_isComputed)
    {
        m_matrixM = m_matrixT * m_matrixR * m_matrixS;
        m_matrixMVP = m_camera->getVPMatrix() * m_matrixM;
        m_matrixIMVP = m_camera->getIVPMatrix() * m_matrixM;
        m_isComputed = true;
    }
    return m_matrixM;
}

glm::mat4 CComponentTransformation::getMVPMatrix(void)
{
    if(!m_isComputed)
    {
        m_matrixM = m_matrixT * m_matrixR * m_matrixS;
        m_matrixMVP = m_camera->getVPMatrix() * m_matrixM;
        m_matrixIMVP = m_camera->getIVPMatrix() * m_matrixM;
        m_isComputed = true;
    }
    return m_matrixMVP;
}

glm::mat4 CComponentTransformation::getIMVPMatrix(void)
{
    if(!m_isComputed)
    {
        m_matrixM = m_matrixT * m_matrixR * m_matrixS;
        m_matrixMVP = m_camera->getVPMatrix() * m_matrixM;
        m_matrixIMVP = m_camera->getIVPMatrix() * m_matrixM;
        m_isComputed = true;
    }
    return m_matrixIMVP;
}
