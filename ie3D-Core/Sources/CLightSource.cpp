//
//  CLightSource.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CLightSource.h"

CLightSource::CLightSource(f32 fov, f32 near, f32 far, f32 aspect) :
m_position(glm::vec3(0.0)),
m_lookAt(glm::vec3(0.0))
{
    m_projection = glm::perspective(fov, aspect, near, far);
}

CLightSource::~CLightSource(void)
{
    
}

void CLightSource::setPosition(const glm::vec3 &position)
{
    m_position = position;
}

glm::vec3 CLightSource::getPosition(void) const
{
    return m_position;
}

void CLightSource::setLookAt(const glm::vec3& lookAt)
{
    m_lookAt = lookAt;
}

glm::vec3 CLightSource::getLookAt(void) const
{
    return m_lookAt;
}

glm::mat4x4 CLightSource::getViewMatrix(void) const
{
    return m_view;
}

glm::mat4x4 CLightSource::getProjectionMatrix(void) const
{
    return m_projection;
}

void CLightSource::onSceneUpdate(f32 deltatime)
{
    m_view = glm::lookAt(m_position, m_lookAt, glm::vec3(0.0, 1.0, 0.0));
}
