//
//  IResource.cpp
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IResource.h"

IResourceLoadingHandler::IResourceLoadingHandler(void)
{
    
}

IResourceLoadingHandler::~IResourceLoadingHandler(void)
{
    std::for_each(m_resourceLoadingHandlers.begin(), m_resourceLoadingHandlers.end(),
                  [](std::set<RESOURCE_LOADING_HANDLER_FUNCTION>& iterator){
                      iterator.clear();
                  });
}

void IResourceLoadingHandler::onResourceLoaded(ISharedResourceRef resource, bool success)
{
    if(success)
    {
        const auto& iterator = m_resourceLoadingHandlers.at(resource->getResourceClass());
        std::for_each(iterator.begin(), iterator.end(), [resource](RESOURCE_LOADING_HANDLER_FUNCTION function){
            (*function)(resource);
        });
    }
}

void IResourceLoadingHandler::registerResourceLoadingHandler(const RESOURCE_LOADING_HANDLER_FUNCTION& handler,
                                                             E_RESOURCE_CLASS resourceClass)
{
    m_resourceLoadingHandlers.at(resourceClass).insert(handler);
}

void IResourceLoadingHandler::unregisterResourceLoadingHandler(const RESOURCE_LOADING_HANDLER_FUNCTION& handler,
                                                               E_RESOURCE_CLASS resourceClass)
{
    m_resourceLoadingHandlers.at(resourceClass).erase(handler);
}

IResourceData::IResourceData(E_RESOURCE_DATA_CLASS resourceDataClass) :
m_resourceDataClass(resourceDataClass)
{

}

IResourceData::~IResourceData(void)
{
    
}

E_RESOURCE_DATA_CLASS IResourceData::getResourceDataClass(void) const
{
    return m_resourceDataClass;
}

IResource::IResource(E_RESOURCE_CLASS resourceClass, const std::string& guid) :
m_resourceClass(resourceClass),
m_guid(guid),
m_status(E_RESOURCE_STATUS_UNLOADED)
{

}

IResource::~IResource(void)
{
    m_handlers.clear();
}

const std::string& IResource::getGuid(void) const
{
    return m_guid;
}

E_RESOURCE_CLASS IResource::getResourceClass(void) const
{
    return m_resourceClass;
}

bool IResource::isLoaded(void) const
{
    const bool value = 0 != (m_status & E_RESOURCE_STATUS_LOADED);
    return value;
};

bool IResource::isCommited(void) const
{
    const bool value = 0 != (m_status & E_RESOURCE_STATUS_COMMITED);
    return value;
};

void IResource::registerLoadingHandler(ISharedResourceLoadingHandlerRef handler)
{
    assert(handler != nullptr);
    if(IResource::isLoaded() && IResource::isCommited())
    {
        handler->onResourceLoaded(shared_from_this(), true);
    } else {
        m_handlers.insert(handler);
    }
}

void IResource::unregisterLoadingHandler(ISharedResourceLoadingHandlerRef handler)
{
    assert(handler != nullptr);
    m_handlers.erase(handler);
}

void IResource::onResourceLoaded(void)
{
    for(const auto& handler : m_handlers)
    {
        handler->onResourceLoaded(shared_from_this(), true);
    }
}