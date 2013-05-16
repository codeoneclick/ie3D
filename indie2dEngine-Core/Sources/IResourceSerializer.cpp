//
//  IResourceSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/16/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IResourceSerializer.h"

IResourceSerializer::IResourceSerializer(const std::string& _guid) :
m_guid(_guid),
m_status(E_SERIALIZER_STATUS_UNKNOWN)
{
    
}

IResourceSerializer::~IResourceSerializer(void)
{
    
}