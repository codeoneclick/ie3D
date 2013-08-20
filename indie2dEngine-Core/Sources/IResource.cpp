//
//  IResource.cpp
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IResource.h"

IResource::IResource(E_RESOURCE_TYPE _resourceType, const std::string& _guid) :
m_resourceType(_resourceType),
m_guid(_guid),
m_status(E_RESOURCE_STATUS_UNLOADED)
{

}

IResource::~IResource(void)
{
    
}