//
//  CAnimationSequenceLoadingOperation.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 11/13/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CAnimationSequenceLoadingOperation.h"
#include "CSequenceSerializer_SEQ.h"
#include "CSequenceCommiter_SEQ.h"
#include "IResource.h"
#include "CAnimationSequence.h"

CAnimationSequenceLoadingOperation::CAnimationSequenceLoadingOperation(const std::string& filename,
                                                                       ISharedResourceRef resource) :
IResourceLoadingOperation(filename, resource),
m_filename(filename)
{
    
}

CAnimationSequenceLoadingOperation::~CAnimationSequenceLoadingOperation(void)
{
    
}

void CAnimationSequenceLoadingOperation::serialize(void)
{
    assert(m_resource != nullptr);
    
    m_status = E_RESOURCE_LOADING_OPERATION_STATUS_INPROGRESS;
    m_serializer = std::make_shared<CSequenceSerializer_SEQ>(m_filename,
                                                             m_resource);
    m_serializer->serialize();
    m_status = m_serializer->getStatus() == E_SERIALIZER_STATUS_SUCCESS ? E_RESOURCE_LOADING_OPERATION_STATUS_WAITING : E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE;
}

void CAnimationSequenceLoadingOperation::commit(void)
{
    assert(m_resource != nullptr);
    assert(m_resource->isLoaded() == true);
    
    m_commiter = std::make_shared<CSequenceCommiter_SEQ>(m_filename,
                                                         m_resource);
    m_commiter->commit();
    m_status = m_commiter->getStatus() == E_COMMITER_STATUS_SUCCESS ? E_RESOURCE_LOADING_OPERATION_STATUS_SUCCESS : E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE;
    IResourceLoadingOperation::onResourceLoaded();
}