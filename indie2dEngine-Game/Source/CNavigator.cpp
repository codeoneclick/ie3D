//
//  CNavigator.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/23/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CNavigator.h"

CNavigator::CNavigator(f32 _moveForwardSpeed,
                       f32 _moveBackwardSpeed,
                       f32 _strafeSpeed,
                       f32 _steerSpeed)
{
    m_moveForwardSpeed = _moveForwardSpeed;
    m_moveBackwardSpeed = _moveBackwardSpeed;
    m_strafeSpeed = _strafeSpeed;
    m_steerSpeed = _steerSpeed;
    
    m_position = glm::vec3(0.0f, 0.0f, 0.0f);
    m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
}

CNavigator::~CNavigator(void)
{
    
}

void CNavigator::Set_Position(const glm::vec3 &_position)
{
    m_position = _position;
}

bool CNavigator::MoveForward(void)
{
    m_position = glm::vec3(m_position.x + cosf(-m_rotation.y) * m_moveForwardSpeed, 0.0f, m_position.z + sinf(-m_rotation.y) * m_moveForwardSpeed);
    return true;
}

bool CNavigator::MoveBackward(void)
{
    m_position = glm::vec3(m_position.x - cosf(-m_rotation.y) * m_moveBackwardSpeed, 0.0f, m_position.z - sinf(-m_rotation.y) * m_moveBackwardSpeed);
    return true;
}

bool CNavigator::MoveLeft(void)
{
    m_position = glm::vec3(m_position.x - sinf(m_rotation.y) * m_strafeSpeed, 0.0f, m_position.z - cosf(m_rotation.y) * m_strafeSpeed);
    return true;
}

bool CNavigator::MoveRight(void)
{
    m_position = glm::vec3(m_position.x + sinf(m_rotation.y) * m_strafeSpeed, 0.0f, m_position.z + cosf(m_rotation.y) * m_strafeSpeed);
    return true;
}

void CNavigator::SteerLeft(void)
{
    m_rotation.y += m_steerSpeed;
    m_rotation = glm::vec3(0.0f, m_rotation.y, 0.0f);
}

void CNavigator::SteerRight(void)
{
    m_rotation.y -= m_steerSpeed;
    m_rotation = glm::vec3(0.0f, m_rotation.y, 0.0f);
}
