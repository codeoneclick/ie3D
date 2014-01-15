//
//  CSequenceLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/13/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CSequenceLoadingOperation.h"
#include "CSequence.h"
#include "CSequenceSerializer_SEQ.h"
#include "CSequenceCommiter_SEQ.h"

CSequenceLoadingOperation::CSequenceLoadingOperation(const std::string& _filename, std::shared_ptr<IResource> _resource) :
IResourceLoadingOperation(_filename, _resource),
m_filename(_filename)
{
    
}

CSequenceLoadingOperation::~CSequenceLoadingOperation(void)
{
    
}

void CSequenceLoadingOperation::Serialize(void)
{
    assert(m_resource != nullptr);
    m_status = E_RESOURCE_LOADING_OPERATION_STATUS_INPROGRESS;
    m_serializer = std::make_shared<CSequenceSerializer_SEQ>(m_filename,
                                                             m_resource);
    m_serializer->Serialize();
    m_status = m_serializer->Get_Status() == E_SERIALIZER_STATUS_SUCCESS ? E_RESOURCE_LOADING_OPERATION_STATUS_WAITING : E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE;
}

void CSequenceLoadingOperation::Commit(void)
{
    assert(m_resource != nullptr);
    assert(m_resource->IsLoaded() == true);
    m_commiter = std::make_shared<CSequenceCommiter_SEQ>(m_serializer->Get_Guid(),
                                                        m_resource);
    m_commiter->Commit();
    m_status = m_commiter->Get_Status() == E_COMMITER_STATUS_SUCCESS ? E_RESOURCE_LOADING_OPERATION_STATUS_SUCCESS : E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE;
    m_resource->_OnLoaded();
}