//
//  CCollisionHandler.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/19/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "ICollisionHandler.h"

CCollisionCommands::CCollisionCommands(void) :
m_getTargetCommand(nullptr),
m_collisionCommand(nullptr)
{
    
}

CCollisionCommands::~CCollisionCommands(void)
{
    m_getTargetCommand = nullptr;
    m_collisionCommand = nullptr;
}

void CCollisionCommands::_ConnectGetTargetCommand(const __GET_TARGET_COMMAND &_command)
{
    assert(_command != nullptr);
    m_getTargetCommand = _command;
}

void CCollisionCommands::_ConnectCollisionCommand(const __COLLISION_COMMAND &_command)
{
    assert(_command != nullptr);
    m_collisionCommand = _command;
}

std::shared_ptr<IGameObject> CCollisionCommands::_ExecuteGetTargetCommand(void)
{
    assert(m_getTargetCommand != nullptr);
    return m_getTargetCommand();
}

void CCollisionCommands::_ExecuteCollisionCommand(const glm::vec3 &_position)
{
    assert(m_collisionCommand != nullptr);
    m_collisionCommand(_position);
}

ICollisionHandler::ICollisionHandler(void)
{
    ICollisionHandler::_ConnectCommands();
}

ICollisionHandler::~ICollisionHandler(void)
{
    
}

void ICollisionHandler::_ConnectCommands(void)
{
    m_commands._ConnectGetTargetCommand(std::bind(&ICollisionHandler::_OnGetTarget, this));
    m_commands._ConnectCollisionCommand(std::bind(&ICollisionHandler::_OnCollision, this, std::placeholders::_1));
}
