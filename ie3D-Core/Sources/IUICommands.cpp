//
//  IUICommands.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 2/12/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "IUICommands.h"

IUICommands::~IUICommands(void)
{
    m_commands.clear();
}

void IUICommands::addCommand( const std::string& guid, ISharedCommandRef command)
{
    m_commands.insert(std::make_pair(guid, command));
};


void IUICommands::removeCommand(const std::string& guid)
{
    auto command = m_commands.find(guid);
    if(command != m_commands.end())
    {
        m_commands.erase(command);
    }
}