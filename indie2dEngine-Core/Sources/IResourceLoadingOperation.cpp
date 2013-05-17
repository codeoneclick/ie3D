//
//  IResourceLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IResourceLoadingOperation.h"

IResourceLoadingOperation::IResourceLoadingOperation(const std::string& _guid) :
m_guid(_guid),
m_status(E_RESOURCE_LOADING_OPERATION_STATUS_UNKNOWN),
m_serializer(nullptr),
m_commiter(nullptr)
{
    
}

IResourceLoadingOperation::~IResourceLoadingOperation(void)
{
    
}

