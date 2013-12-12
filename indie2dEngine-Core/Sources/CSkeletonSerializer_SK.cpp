//
//  CSkeletonSerializer_SK.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/13/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CSkeletonSerializer_SK.h"
#include "CSkeleton.h"

CSkeletonSerializer_SK::CSkeletonSerializer_SK(const std::string& _filename, std::shared_ptr<IResource> _resource) :
IResourceSerializer(_filename, _resource),
m_filename(_filename)
{
    
}

CSkeletonSerializer_SK::~CSkeletonSerializer_SK(void)
{
    
}

void CSkeletonSerializer_SK::Serialize(void)
{
    assert(m_resource != nullptr);
    m_status = E_SERIALIZER_STATUS_INPROGRESS;
    
    std::istream* filestream = IResourceSerializer::_LoadData(m_filename);
    std::shared_ptr<CSkeleton> skeleton = std::static_pointer_cast<CSkeleton >(m_resource);
    skeleton->_Serialize(filestream);
    IResourceSerializer::_FreeData(filestream);
    
    m_status = E_SERIALIZER_STATUS_SUCCESS;
}