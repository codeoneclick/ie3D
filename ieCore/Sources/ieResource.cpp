//
//  ieResource.cpp
//  ieCore
//
//  Created by sergey.sergeev on 4/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "ieResource.h"

ieResource::ieResource(E_RESOURCE_CLASS resourceClass, const std::string& guid) :
m_resourceClass(resourceClass),
m_guid(guid)
{
    
}

ieResource::~ieResource(void)
{
    
}

void ieResource::addOwner(ieSharedObjectRef owner)
{
    assert(m_owners.count(owner) == 0);
    m_owners.insert(owner);
}

void ieResource::removeOwner(ieSharedObjectRef owner)
{
    m_owners.erase(owner);
}

const ui32 ieResource::getOwnersCount(void) const
{
    return m_owners.size();
}

std::string ieResource::getGUID(void) const
{
    return m_guid;
}