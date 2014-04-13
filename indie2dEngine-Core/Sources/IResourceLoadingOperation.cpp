//
//  IResourceLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IResourceLoadingOperation.h"
#include "IResource.h"

IResourceLoadingOperation::IResourceLoadingOperation(const std::string& guid,
                                                     ISharedResourceRef resource) :
m_guid(guid),
m_resource(resource),
m_status(E_RESOURCE_LOADING_OPERATION_STATUS_UNKNOWN),
m_serializer(nullptr),
m_commiter(nullptr)
{
    
}

IResourceLoadingOperation::~IResourceLoadingOperation(void)
{
    
}

std::string IResourceLoadingOperation::getGuid(void) const
{
    return m_guid;
}

E_RESOURCE_LOADING_OPERATION_STATUS IResourceLoadingOperation::getStatus(void) const
{
    return m_status;
}

void IResourceLoadingOperation::onResourceLoaded(void)
{
    m_resource->onResourceLoaded();
}
