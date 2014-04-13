//
//  IResourceCommiter.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/16/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IResourceCommiter.h"
#include "IResource.h"

IResourceCommiter::IResourceCommiter(const std::string& guid, ISharedResourceRef resource) :
m_guid(guid),
m_resource(resource),
m_status(E_COMMITER_STATUS_UNKNOWN)
{
    
}

IResourceCommiter::~IResourceCommiter(void)
{
    
}

void IResourceCommiter::onResourceDataCommited(ISharedResourceDataRef resourceData)
{
    m_resource->onResourceDataCommited(resourceData,
                                       E_RESOURCE_DATA_STATUS_PROGRESS);
}

std::string IResourceCommiter::getGuid(void) const
{
    return m_guid;
}

E_COMMITER_STATUS IResourceCommiter::getStatus(void) const
{
    return m_status;
}
