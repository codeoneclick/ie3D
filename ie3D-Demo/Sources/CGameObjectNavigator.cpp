//
//  CGameObjectNavigator.cpp
//  ie3D-Demo
//
//  Created by sergey.sergeev on 8/12/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CGameObjectNavigator.h"
#include "CLandscape.h"

CGameObjectNavigator::CGameObjectNavigator(f32 moveForwardSpeed,
                                           f32 moveBackwardSpeed,
                                           f32 strafeSpeed,
                                           f32 steerSpeed,
                                           CSharedLandscapeRef landscape,
                                           const glm::vec3& maxBound,
                                           const glm::vec3& minBound) :
m_moveForwardSpeed(moveForwardSpeed),
m_moveBackwardSpeed(moveBackwardSpeed),
m_strafeSpeed(strafeSpeed),
m_steerSpeed(steerSpeed),
m_landscape(landscape),
m_maxBound(maxBound),
m_minBound(minBound),
m_currentPosition(glm::vec3(0.0)),
m_currentRotation(glm::vec3(0.0))
{
    
}

void CGameObjectNavigator::setPosition(const glm::vec3& position)
{
    m_currentPosition = position;
    CGameObjectNavigator::notifyHandlersAboutPositionChanged();
}

void CGameObjectNavigator::setRotation(const glm::vec3& rotation)
{
    m_currentRotation = rotation;
    CGameObjectNavigator::notifyHandlersAboutRotationChanged();
}

bool CGameObjectNavigator::moveForward(void)
{
    glm::vec3 precomputedPosition = glm::vec3(m_currentPosition.x + sinf(glm::radians(m_currentRotation.y)) * m_moveForwardSpeed,
                                              0.0,
                                              m_currentPosition.z + cosf(glm::radians(m_currentRotation.y)) * m_moveForwardSpeed);
    
    if(precomputedPosition.x < m_minBound.x ||
       precomputedPosition.z < m_minBound.z ||
       precomputedPosition.x > m_maxBound.x ||
       precomputedPosition.z > m_maxBound.z)
    {
        return false;
    }

    precomputedPosition.y = m_landscape != nullptr ? m_landscape->getHeight(precomputedPosition) : 0.0;
    m_currentPosition = precomputedPosition;
    
    CGameObjectNavigator::notifyHandlersAboutPositionChanged();
    CGameObjectNavigator::updateRotationOnHeightmapSurface(precomputedPosition);
    return true;
}

bool CGameObjectNavigator::moveBackward(void)
{
    glm::vec3 precomputedPosition = glm::vec3(m_currentPosition.x - sinf(glm::radians(m_currentRotation.y)) * m_moveBackwardSpeed,
                                              0.0f,
                                              m_currentPosition.z - cosf(glm::radians(m_currentRotation.y)) * m_moveBackwardSpeed);
    
    if(precomputedPosition.x < m_minBound.x ||
       precomputedPosition.z < m_minBound.z ||
       precomputedPosition.x > m_maxBound.x ||
       precomputedPosition.z > m_maxBound.z)
    {
        return false;
    }
    
    precomputedPosition.y = m_landscape != nullptr ? m_landscape->getHeight(precomputedPosition) : 0.0;
    m_currentPosition = precomputedPosition;
    
    CGameObjectNavigator::notifyHandlersAboutPositionChanged();
    CGameObjectNavigator::updateRotationOnHeightmapSurface(precomputedPosition);
    return true;
}

bool CGameObjectNavigator::moveLeft(void)
{
    glm::vec3 precomputedPosition = glm::vec3(m_currentPosition.x - sinf(glm::radians(-m_currentRotation.y)) * m_strafeSpeed,
                                              0.0f,
                                              m_currentPosition.z - cosf(glm::radians(-m_currentRotation.y)) * m_strafeSpeed);
    
    if(precomputedPosition.x < m_minBound.x ||
       precomputedPosition.z < m_minBound.z ||
       precomputedPosition.x > m_maxBound.x ||
       precomputedPosition.z > m_maxBound.z)
    {
        return false;
    }
    
    precomputedPosition.y = m_landscape != nullptr ? m_landscape->getHeight(precomputedPosition) : 0.0;
    m_currentPosition = precomputedPosition;
    
    CGameObjectNavigator::notifyHandlersAboutPositionChanged();
    CGameObjectNavigator::updateRotationOnHeightmapSurface(precomputedPosition);
    return true;
}

bool CGameObjectNavigator::moveRight(void)
{
    glm::vec3 precomputedPosition = glm::vec3(m_currentPosition.x + sinf(glm::radians(-m_currentRotation.y)) * m_strafeSpeed,
                                              0.0f,
                                              m_currentPosition.z + cosf(glm::radians(-m_currentRotation.y)) * m_strafeSpeed);
    
    if(precomputedPosition.x < m_minBound.x ||
       precomputedPosition.z < m_minBound.z ||
       precomputedPosition.x > m_maxBound.x ||
       precomputedPosition.z > m_maxBound.z)
    {
        return false;
    }
    
    precomputedPosition.y = m_landscape != nullptr ? m_landscape->getHeight(precomputedPosition) : 0.0;
    m_currentPosition = precomputedPosition;
    
    CGameObjectNavigator::notifyHandlersAboutPositionChanged();
    CGameObjectNavigator::updateRotationOnHeightmapSurface(precomputedPosition);
    return true;
}

void CGameObjectNavigator::steerLeft(void)
{
    m_currentRotation.y += m_steerSpeed;
    CGameObjectNavigator::notifyHandlersAboutRotationChanged();
}

void CGameObjectNavigator::steerRight(void)
{
    m_currentRotation.y -= m_steerSpeed;
    CGameObjectNavigator::notifyHandlersAboutRotationChanged();
}

void CGameObjectNavigator::addNavigatorHandler(ISharedGameObjectNavigatorHandlerRef handler)
{
    m_handlers.insert(handler);
}

void CGameObjectNavigator::removeNavigatorHadler(ISharedGameObjectNavigatorHandlerRef handler)
{
    m_handlers.erase(handler);
}

void CGameObjectNavigator::notifyHandlersAboutPositionChanged(void)
{
    for(const auto& iterator : m_handlers)
    {
        iterator->onPositionChanged(m_currentPosition);
    }
}

void CGameObjectNavigator::notifyHandlersAboutRotationChanged(void)
{
    for(const auto& iterator : m_handlers)
    {
        iterator->onRotationChanged(m_currentRotation);
    }
}

void CGameObjectNavigator::updateRotationOnHeightmapSurface(const glm::vec3& position)
{
    glm::vec2 precomputedRotation = m_landscape != nullptr ? m_landscape->getAngleOnHeightmapSurface(position) : glm::vec2(0.0, 0.0);
    m_currentRotation.x = precomputedRotation.x;
    m_currentRotation.z = precomputedRotation.y;
    CGameObjectNavigator::notifyHandlersAboutRotationChanged();
}
