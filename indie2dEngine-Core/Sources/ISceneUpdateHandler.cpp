//
//  ISceneUpdateHandler.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/23/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "ISceneUpdateHandler.h"

CSceneUpdateCommands::CSceneUpdateCommands(void) :
m_sceneUpdateCommand(nullptr)
{
    
}

CSceneUpdateCommands::~CSceneUpdateCommands(void)
{
    m_sceneUpdateCommand = nullptr;
}

void CSceneUpdateCommands::_ConnectSceneUpdateCommand(const __SCENE_UPDATE_COMMAND &_command)
{
    assert(_command != nullptr);
    m_sceneUpdateCommand = _command;
}

void CSceneUpdateCommands::_ExecuteSceneUpdateCommand(f32 _deltatime)
{
    assert(m_sceneUpdateCommand != nullptr);
    m_sceneUpdateCommand(_deltatime);
}

ISceneUpdateHandler::ISceneUpdateHandler(void)
{
    ISceneUpdateHandler::_ConnectCommands();
}

ISceneUpdateHandler::~ISceneUpdateHandler(void)
{
    
}

void ISceneUpdateHandler::_ConnectCommands(void)
{
    m_commands._ConnectSceneUpdateCommand(std::bind(&ISceneUpdateHandler::_OnSceneUpdate, this, std::placeholders::_1));
}