//
//  CDEUIToSceneCommands.cpp
//  ie3D-Demo
//
//  Created by sergey.sergeev on 8/12/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CDEUIToSceneCommands.h"

CDEUIToSceneCommands::CDEUIToSceneCommands(void) :
m_setCharacterMoveStateCommand(nullptr),
m_setCharacterSteerStateCommand(nullptr)
{
    
}

CDEUIToSceneCommands::~CDEUIToSceneCommands(void)
{
    
}

void CDEUIToSceneCommands::connectSetCharacterMoveStateCommand(const __SET_CHARACTER_MOVE_STATE_COMMAND& command)
{
    m_setCharacterMoveStateCommand = command;
}

void CDEUIToSceneCommands::executeSetCharacterMoveStateCommand(E_CHARACTER_CONTROLLER_MOVE_STATE state)
{
    if(m_setCharacterMoveStateCommand != nullptr)
    {
        m_setCharacterMoveStateCommand(state);
    }
}

void CDEUIToSceneCommands::connectSetCharacterSteerStateCommand(const __SET_CHARACTER_STEER_STATE_COMMAND& command)
{
    m_setCharacterSteerStateCommand = command;
}

void CDEUIToSceneCommands::executeSetCharacterSteerStateCommand(E_CHARACTER_CONTROLLER_STEER_STATE state)
{
    if(m_setCharacterSteerStateCommand != nullptr)
    {
        m_setCharacterSteerStateCommand(state);
    }
}
