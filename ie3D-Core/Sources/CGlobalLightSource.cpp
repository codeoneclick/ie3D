//
//  CGlobalLightSource.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 10/20/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CGlobalLightSource.h"

CGlobalLightSource::CGlobalLightSource(f32 fov, f32 near, f32 far) :
m_position(glm::vec3(0.0)),
m_lookAt(glm::vec3(0.0)),
m_distanceToSun(0.0),
m_distanceToLookAt(0.0),
m_angle(0.0),
m_rotationCenter(glm::vec3(0.0))
{
    m_projection = glm::perspective(fov, 1.0f, near, far);
}

CGlobalLightSource::~CGlobalLightSource(void)
{
    
}

glm::vec3 CGlobalLightSource::getPosition(void) const
{
    return m_position;
}

void CGlobalLightSource::setLookAt(const glm::vec3& lookAt)
{
    m_lookAt = lookAt;
}

glm::vec3 CGlobalLightSource::getLookAt(void) const
{
    return m_lookAt;
}

void CGlobalLightSource::setAngle(f32 angle)
{
    m_angle = angle;
}

f32 CGlobalLightSource::getAngle(void) const
{
    return m_angle;
}

glm::mat4x4 CGlobalLightSource::getViewMatrix(void) const
{
    return m_view;
}

glm::mat4x4 CGlobalLightSource::getProjectionMatrix(void) const
{
    return m_projection;
}

void CGlobalLightSource::setDistanceToSun(f32 distance)
{
    m_distanceToSun = distance;
}

void CGlobalLightSource::setDistanceToLookAt(f32 distance)
{
    m_distanceToLookAt = distance;
}

void CGlobalLightSource::setRotationCenter(const glm::vec3& rotationCenter)
{
    m_rotationCenter = rotationCenter;
}

glm::vec3 CGlobalLightSource::getRotationCenter(void) const
{
    return m_rotationCenter;
}

void CGlobalLightSource::onSceneUpdate(f32 deltatime)
{
    m_position = glm::vec3(sinf(m_angle) * -m_distanceToSun + m_rotationCenter.x,
                           cosf(m_angle) * -m_distanceToSun + m_rotationCenter.y,
                           m_rotationCenter.z);
    
    glm::vec3 position(sinf(m_angle) * -m_distanceToLookAt + m_lookAt.x,
                       cosf(m_angle) * -m_distanceToLookAt + m_lookAt.y,
                       m_lookAt.z);
    
    m_view = glm::lookAt(position, m_lookAt, glm::vec3(0.0, 1.0, 0.0));
}
