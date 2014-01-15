//
//  IGameLoopHandler.cpp
//  indi2dEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IGameLoopHandler.h"

CGameLoopCommands::CGameLoopCommands(void) :
m_gameLoopUpdateCommand(nullptr)
{

}

CGameLoopCommands::~CGameLoopCommands(void)
{
    m_gameLoopUpdateCommand = nullptr;
}

void CGameLoopCommands::_ConnectGameLoopUpdateCommand(const __GAME_LOOP_UPDATE_COMMAND &_command)
{
    assert(_command != nullptr);
    m_gameLoopUpdateCommand = _command;
}

void CGameLoopCommands::_ExecuteGameLoopUpdateCommand(f32 _deltatime)
{
    assert(m_gameLoopUpdateCommand != nullptr);
    m_gameLoopUpdateCommand(_deltatime);
}

IGameLoopHandler::IGameLoopHandler(void)
{
    IGameLoopHandler::_ConnectCommands();
}

IGameLoopHandler::~IGameLoopHandler(void)
{
    
}

void IGameLoopHandler::_ConnectCommands(void)
{
    m_commands._ConnectGameLoopUpdateCommand(std::bind(&IGameLoopHandler::_OnGameLoopUpdate, this, std::placeholders::_1));
}