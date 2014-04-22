//
//  CSequenceCommiter_SEQ.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/13/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CSequenceCommiter_SEQ.h"

CSequenceCommiter_SEQ::CSequenceCommiter_SEQ(const std::string& guid, ISharedResourceRef resource) :
IResourceCommiter(guid, resource)
{
    
}

CSequenceCommiter_SEQ::~CSequenceCommiter_SEQ(void)
{
    
}

void CSequenceCommiter_SEQ::commit(void)
{
    m_status = E_COMMITER_STATUS_INPROGRESS;
    assert(m_resource != nullptr);
    m_status = E_COMMITER_STATUS_SUCCESS;
}