//
//  IResource.cpp
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IResource.h"
#include "IResourceLoadingHandler.h"

IResource::IResource(E_RESOURCE_CLASS _class, const std::string& _guid) :
m_class(_class),
m_guid(_guid),
m_status(E_RESOURCE_STATUS_UNLOADED)
{

}

IResource::~IResource(void)
{
    m_handlers.clear();
}

void IResource::RegisterResourceLoadingHandler(std::shared_ptr<IResourceLoadingHandler> _handler)
{
    if(_handler == nullptr)
    {
        return;
    }
    if((m_status & E_RESOURCE_STATUS_LOADED) && (m_status & E_RESOURCE_STATUS_COMMITED))
    {
        _handler->_Get_Commands()._ExecuteLoadedResourceCommand(shared_from_this(), true);
    }
    else
    {
        m_handlers.insert(_handler);
    }
}

void IResource::ExecuteResourceLoadingHandlers(void)
{
    for(auto handler : m_handlers)
    {
        handler->_Get_Commands()._ExecuteLoadedResourceCommand(shared_from_this(), true);
    }
}