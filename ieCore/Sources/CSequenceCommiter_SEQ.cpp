//
//  CSequenceCommiter_SEQ.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/13/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CSequenceCommiter_SEQ.h"
#include "CSequence.h"

CSequenceCommiter_SEQ::CSequenceCommiter_SEQ(const std::string& _guid, std::shared_ptr<IResource> _resource) :
IResourceCommiter(_guid, _resource)
{
    
}

CSequenceCommiter_SEQ::~CSequenceCommiter_SEQ(void)
{
    
}

void CSequenceCommiter_SEQ::Commit(void)
{
    m_status = E_COMMITER_STATUS_INPROGRESS;
    assert(m_resource != nullptr);
    
    std::shared_ptr<CSequence> sequence = std::static_pointer_cast<CSequence >(m_resource);
    sequence->_BindSequence();
    
    m_status = E_COMMITER_STATUS_SUCCESS;
}