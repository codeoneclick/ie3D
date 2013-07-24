//
//  IMoveControllerHandler.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/23/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IMoveControllerHandler.h"

CMoveControllerCommands::CMoveControllerCommands(void) :
m_moveControllerUpdateCommand(nullptr)
{
    
}

CMoveControllerCommands::~CMoveControllerCommands(void)
{
    m_moveControllerUpdateCommand = nullptr;
}

void CMoveControllerCommands::_ConnectMoveControllerUpdateCommand(const __MOVE_CONTROLLER_UPDATE_COMMAND &_command)
{
    assert(_command != nullptr);
    m_moveControllerUpdateCommand = _command;
}

void CMoveControllerCommands::_ExecuteMoveControllerUpdateCommand(ui32 _direction)
{
    assert(m_moveControllerUpdateCommand != nullptr);
    return m_moveControllerUpdateCommand(_direction);
}

IMoveControllerHandler::IMoveControllerHandler(void)
{
    IMoveControllerHandler::_ConnectCommands();
}

IMoveControllerHandler::~IMoveControllerHandler(void)
{
    
}

void IMoveControllerHandler::_ConnectCommands(void)
{
    m_commands._ConnectMoveControllerUpdateCommand(std::bind(&IMoveControllerHandler::_OnMoveControllerUpdate, this, std::placeholders::_1));
}
