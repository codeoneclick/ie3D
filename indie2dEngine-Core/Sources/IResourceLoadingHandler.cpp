//
//  IResourceLoadingHandler.cpp
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IResourceLoadingHandler.h"

CResourceLoadingCommands::CResourceLoadingCommands(void) :
m_resourceLoadedCommand(nullptr)
{
    
}

CResourceLoadingCommands::~CResourceLoadingCommands(void)
{
    m_resourceLoadedCommand = nullptr;
}

void CResourceLoadingCommands::_ConnectLoadedResourceCommand(const _RESOURCE_LOADED_COMMAND &_command)
{
    assert(_command != nullptr);
    m_resourceLoadedCommand = _command;
}

void CResourceLoadingCommands::_ExecuteLoadedResourceCommand(std::shared_ptr<IResource> _resource, bool _success)
{
    assert(m_resourceLoadedCommand != nullptr);
    m_resourceLoadedCommand(_resource, _success);
}

IResourceLoadingHandler::IResourceLoadingHandler(void)
{
    IResourceLoadingHandler::_ConnectCommands();
    std::for_each(m_handlers.begin(), m_handlers.end(), [](_RESOURCE_LOADED_HANDLER _handler)
                  {
                      _handler = nullptr;
                  });

}

IResourceLoadingHandler::~IResourceLoadingHandler(void)
{
    
}

void IResourceLoadingHandler::_ConnectCommands(void)
{
    m_commands._ConnectLoadedResourceCommand(std::bind(&IResourceLoadingHandler::_OnResourceLoaded, this, std::placeholders::_1, std::placeholders::_2));
}

void IResourceLoadingHandler::Listen(const _RESOURCE_LOADED_HANDLER &_handler, E_RESOURCE_CLASS _class)
{
    m_handlers[_class] = _handler;
}