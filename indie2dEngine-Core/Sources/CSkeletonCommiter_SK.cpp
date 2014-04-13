//
//  CSkeletonCommiter_SK.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/13/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CSkeletonCommiter_SK.h"
#include "CSkeleton.h"

CSkeletonCommiter_SK::CSkeletonCommiter_SK(const std::string& guid, ISharedResourceRef resource) :
IResourceCommiter(guid, resource)
{
    
}

CSkeletonCommiter_SK::~CSkeletonCommiter_SK(void)
{
    
}

void CSkeletonCommiter_SK::commit(void)
{
    m_status = E_COMMITER_STATUS_INPROGRESS;
    assert(m_resource != nullptr);
    m_status = E_COMMITER_STATUS_SUCCESS;
}