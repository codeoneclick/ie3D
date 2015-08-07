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
m_currentRotation(glm::vec3(0.0)),
m_moveState(E_NAVIGATION_MOVE_NONE),
m_steerState(E_NAVIGATION_STEER_NONE)
{
    
}

void CGameObjectNavigator::update(f32 deltatime)
{
    if(m_box2dBody)
    {
        b2Vec2 velocity = b2Vec2(0.0f, 0.0f);
        switch (m_moveState)
        {
            case E_NAVIGATION_MOVE_FORWARD:
            {
                velocity.x += sinf(glm::radians(m_currentRotation.y)) * m_moveForwardSpeed;
                velocity.y += cosf(glm::radians(m_currentRotation.y)) * m_moveForwardSpeed;
            }
                break;
                
            case E_NAVIGATION_MOVE_BACKWARD:
            {
                velocity.x -= sinf(glm::radians(m_currentRotation.y)) * m_moveBackwardSpeed;
                velocity.y -= cosf(glm::radians(m_currentRotation.y)) * m_moveBackwardSpeed;
            }
                break;
                
            case E_NAVIGATION_MOVE_LEFT:
            {
                velocity.x -= sinf(glm::radians(-m_currentRotation.y)) * m_strafeSpeed;
                velocity.y -= cosf(glm::radians(-m_currentRotation.y)) * m_strafeSpeed;
            }
                break;
                
            case E_NAVIGATION_MOVE_RIGHT:
            {
                velocity.x += sinf(glm::radians(-m_currentRotation.y)) * m_strafeSpeed;
                velocity.y += cosf(glm::radians(-m_currentRotation.y)) * m_strafeSpeed;
            }
                break;
                
            case E_NAVIGATION_MOVE_NONE:
            {
                
            }
                break;
                
            default:
            {
                assert(false);
            }
                break;
        }
        
        m_box2dBody->SetAwake(true);
        m_box2dBody->SetLinearVelocity(velocity);
        m_moveState = E_NAVIGATION_MOVE_NONE;
    }
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

void CGameObjectNavigator::moveForward(void)
{
    m_moveState = E_NAVIGATION_MOVE_FORWARD;
}

void CGameObjectNavigator::moveBackward(void)
{
    m_moveState = E_NAVIGATION_MOVE_BACKWARD;
}

void CGameObjectNavigator::moveLeft(void)
{
    m_moveState = E_NAVIGATION_MOVE_LEFT;
}

void CGameObjectNavigator::moveRight(void)
{
   m_moveState = E_NAVIGATION_MOVE_RIGHT;
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
    glm::vec2 precomputedRotation = m_landscape != nullptr ? m_landscape->getAngles(position) : glm::vec2(0.0, 0.0);
    m_currentRotation.x = precomputedRotation.x;
    m_currentRotation.z = precomputedRotation.y;
    CGameObjectNavigator::notifyHandlersAboutRotationChanged();
}

void CGameObjectNavigator::onBox2dCollision(void)
{
    
}

void CGameObjectNavigator::onBox2dPositionChanged(const glm::vec3& position)
{
    glm::vec3 precomputedPosition = position;
    
    if(precomputedPosition.x < m_minBound.x ||
       precomputedPosition.z < m_minBound.z ||
       precomputedPosition.x > m_maxBound.x ||
       precomputedPosition.z > m_maxBound.z)
    {
        return;
    }
    
    precomputedPosition.y = m_landscape != nullptr ? m_landscape->getHeight(precomputedPosition) : 0.0;
    m_currentPosition = precomputedPosition;
    
    m_box2dBody->SetTransform(b2Vec2(precomputedPosition.x, precomputedPosition.z), m_box2dBody->GetAngle());
    CGameObjectNavigator::notifyHandlersAboutPositionChanged();
    CGameObjectNavigator::updateRotationOnHeightmapSurface(precomputedPosition);
}

void CGameObjectNavigator::onBox2dRotationYChanged(f32 angle)
{
    
}

glm::vec2 CGameObjectNavigator::getBox2dCenter(void) const
{
    return glm::vec2(m_currentPosition.x,
                     m_currentPosition.z);
}

std::tuple<glm::vec2, glm::vec2> CGameObjectNavigator::getBox2dBoundingBox(void) const
{
    std::tuple<glm::vec2, glm::vec2> boundingBox;
    for(const auto& iterator : m_handlers)
    {
        boundingBox = iterator->getBox2dBouningBox();
    }
    return boundingBox;
}