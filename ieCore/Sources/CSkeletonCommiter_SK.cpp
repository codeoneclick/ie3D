//
//  CSkeletonCommiter_SK.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/13/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CSkeletonCommiter_SK.h"
#include "CSkeleton.h"

CSkeletonCommiter_SK::CSkeletonCommiter_SK(const std::string& _guid, std::shared_ptr<IResource> _resource) :
IResourceCommiter(_guid, _resource)
{
    
}

CSkeletonCommiter_SK::~CSkeletonCommiter_SK(void)
{
    
}

void CSkeletonCommiter_SK::Commit(void)
{
    m_status = E_COMMITER_STATUS_INPROGRESS;
    assert(m_resource != nullptr);
    
    std::shared_ptr<CSkeleton> skeleton = std::static_pointer_cast<CSkeleton>(m_resource);
    skeleton->_BindSkeleton();

    m_status = E_COMMITER_STATUS_SUCCESS;
}