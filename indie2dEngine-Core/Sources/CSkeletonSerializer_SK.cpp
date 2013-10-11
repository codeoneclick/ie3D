//
//  CSkeletonSerializer_SK.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/13/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CSkeletonSerializer_SK.h"
#include "CSkeleton.h"
#include "CCommonOS.h"

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
    
    std::string path(Get_BundlePath());
    std::string filename(path);
    filename.append(m_filename);
    
    std::ifstream filestream;
    filestream.open(filename.c_str(), std::ifstream::in | std::ifstream::binary);
    if (!filestream.is_open())
    {
        m_status = E_SERIALIZER_STATUS_FAILURE;
        return;
    }
    
    std::shared_ptr<CSkeleton> skeleton = std::static_pointer_cast<CSkeleton >(m_resource);
    skeleton->_Serialize(filestream);
    
    filestream.close();
    m_status = E_SERIALIZER_STATUS_SUCCESS;
}