//
//  CCharacterController.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/23/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CCharacterController.h"
#include "CNavigator.h"
#include "CCamera.h"
#include "IGameObject.h"

CCharacterController::CCharacterController(void) :
m_navigator(nullptr),
m_camera(nullptr),
m_character(nullptr)
{
    
}

CCharacterController::~CCharacterController(void)
{
    
}

void CCharacterController::Set_Position(const glm::vec3& _position)
{
    assert(m_navigator != nullptr);
    m_position = _position;
    m_navigator->Set_Position(m_position);
};

void CCharacterController::Set_Rotation(const glm::vec3& _rotation)
{
    assert(m_navigator != nullptr);
    m_rotation = _rotation;
    m_navigator->Set_Rotation(m_rotation);
};

void CCharacterController::_OnMoveControllerUpdate(ui32 _direction)
{
    m_moveDirection = static_cast<E_MOVE_CONTROLLER_DIRECTION>(_direction);
    CCharacterController::OnUpdate(0);
}

void CCharacterController::OnUpdate(f32 _deltatime)
{
    assert(m_navigator != nullptr);
    
    switch(m_moveDirection)
    {
        case E_MOVE_CONTROLLER_DIRECTION_NONE:
        {
            
        }
            break;
        case E_MOVE_CONTROLLER_DIRECTION_NORTH:
        {
            m_navigator->MoveBackward();
        }
            break;
        case E_MOVE_CONTROLLER_DIRECTION_SOUTH:
        {
            m_navigator->MoveForward();
        }
            break;
        case E_MOVE_CONTROLLER_DIRECTION_WEST:
        {
            m_navigator->MoveLeft();
        }
            break;
        case E_MOVE_CONTROLLER_DIRECTION_EAST:
        {
            m_navigator->MoveRight();
        }
            break;
        case E_MOVE_CONTROLLER_DIRECTION_NORTH_WEST:
        {
            m_navigator->MoveBackward();
            m_navigator->MoveLeft();
        }
            break;
        case E_MOVE_CONTROLLER_DIRECTION_NORTH_EAST:
        {
            m_navigator->MoveBackward();
            m_navigator->MoveRight();
        }
            break;
        case E_MOVE_CONTROLLER_DIRECTION_SOUTH_WEST:
        {
            m_navigator->MoveForward();
            m_navigator->MoveRight();
        }
            break;
        case E_MOVE_CONTROLLER_DIRECTION_SOUTH_EAST:
        {
            m_navigator->MoveForward();
            m_navigator->MoveLeft();
        }
            break;
    }
    
    m_position = m_navigator->Get_Position();
    m_rotation = m_navigator->Get_Rotation();
    
    assert(m_camera != nullptr);
    m_camera->Set_LookAt(m_position);
    m_camera->Set_Rotation(m_rotation.y);
    
    assert(m_character != nullptr);
    m_character->setPosition(m_position);
    m_character->setRotation(m_rotation);
}
