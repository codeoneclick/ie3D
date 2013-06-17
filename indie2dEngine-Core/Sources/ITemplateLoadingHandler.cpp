//
//  ITemplateLoadingHandler.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "ITemplateLoadingHandler.h"

CTemplateLoadingCommands::CTemplateLoadingCommands(void) :
m_templateLoadedCommand(nullptr)
{
    
}

CTemplateLoadingCommands::~CTemplateLoadingCommands(void)
{
    m_templateLoadedCommand = nullptr;
}

void CTemplateLoadingCommands::_ConnectTemplateLoadedCommand(const __TEMPLATE_LOADED_COMMAND &_command)
{
    assert(_command != nullptr);
    m_templateLoadedCommand = _command;
}

void CTemplateLoadingCommands::_ExecuteTemplateLoadedCommand(std::shared_ptr<ITemplate> _template)
{
    assert(m_templateLoadedCommand != nullptr);
    m_templateLoadedCommand(_template);
}

ITemplateLoadingHandler::ITemplateLoadingHandler(void)
{
    ITemplateLoadingHandler::_ConnectCommands();
}

ITemplateLoadingHandler::~ITemplateLoadingHandler(void)
{
    
}

void ITemplateLoadingHandler::_ConnectCommands(void)
{
    m_commands._ConnectTemplateLoadedCommand(std::bind(&ITemplateLoadingHandler::_OnTemplateLoaded, this, std::placeholders::_1));
}