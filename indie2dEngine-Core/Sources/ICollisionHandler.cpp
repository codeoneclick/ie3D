//
//  CCollisionHandler.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/19/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "ICollisionHandler.h"

CCollisionCommands::CCollisionCommands(void) :
m_getTargetsCommand(nullptr),
m_collisionCommand(nullptr)
{
    
}

CCollisionCommands::~CCollisionCommands(void)
{
    m_getTargetsCommand = nullptr;
    m_collisionCommand = nullptr;
}

void CCollisionCommands::_ConnectGetTargetsCommand(const __GET_TARGETS_COMMAND &_command)
{
    assert(_command != nullptr);
    m_getTargetsCommand = _command;
}

void CCollisionCommands::_ConnectCollisionCommand(const __COLLISION_COMMAND &_command)
{
    assert(_command != nullptr);
    m_collisionCommand = _command;
}

std::vector<std::shared_ptr<IGameObject> > CCollisionCommands::_ExecuteGetTargetsCommand(void)
{
    assert(m_getTargetsCommand != nullptr);
    return m_getTargetsCommand();
}

void CCollisionCommands::_ExecuteCollisionCommand(const glm::vec3 &_position, std::shared_ptr<IGameObject> _target)
{
    assert(m_collisionCommand != nullptr);
    m_collisionCommand(_position, _target);
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
    m_commands._ConnectGetTargetsCommand(std::bind(&ICollisionHandler::_OnGetTargets, this));
    m_commands._ConnectCollisionCommand(std::bind(&ICollisionHandler::_OnCollision, this, std::placeholders::_1, std::placeholders::_2));
}
