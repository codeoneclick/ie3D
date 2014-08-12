//
//  CDEUIToSceneCommands.cpp
//  ie3D-Demo
//
//  Created by sergey.sergeev on 8/12/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CDEUIToSceneCommands.h"

CDEUIToSceneCommands::CDEUIToSceneCommands(void) :
m_setNavigationCommand(nullptr)
{
    
}

CDEUIToSceneCommands::~CDEUIToSceneCommands(void)
{
    
}

void CDEUIToSceneCommands::connectSetNavigationCommand(const __SET_NAVIGATION_COMMAND& command)
{
    m_setNavigationCommand = command;
}

void CDEUIToSceneCommands::executeSetNavigationCommand(E_UI_NAVIGATION_COMMAND direction)
{
    if(m_setNavigationCommand != nullptr)
    {
        m_setNavigationCommand(direction);
    }
}
