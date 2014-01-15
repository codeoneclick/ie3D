//
//  IResourceCommiter.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/16/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IResourceCommiter.h"

IResourceCommiter::IResourceCommiter(const std::string& _guid, std::shared_ptr<IResource> _resource) :
m_guid(_guid),
m_resource(_resource),
m_status(E_COMMITER_STATUS_UNKNOWN)
{
    
}

IResourceCommiter::~IResourceCommiter(void)
{
    
}