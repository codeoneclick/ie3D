//
//  CMeshLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/26/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CMeshLoadingOperation.h"
#include "CMeshSerializer_MDL.h"
#include "CMeshCommiter_MDL.h"
#include "CSkeletonSerializer_SK.h"
#include "CSkeletonCommiter_SK.h"
#include "CSequenceSerializer_SEQ.h"
#include "CSequenceCommiter_SEQ.h"
#include "IResource.h"
#include "CMesh.h"

CMeshLoadingOperation::CMeshLoadingOperation(const std::string& meshFilename,
                                             const std::string& skeletonFilename,
                                             const std::string& sequenceFilename,
                                             ISharedResourceRef resource) :
IResourceLoadingOperation(meshFilename, resource),
m_meshFilename(meshFilename),
m_skeletonFilename(skeletonFilename),
m_sequenceFilename(sequenceFilename)
{
    
}

CMeshLoadingOperation::~CMeshLoadingOperation(void)
{
    
}

void CMeshLoadingOperation::serialize(void)
{
    assert(m_resource != nullptr);
    m_status = E_RESOURCE_LOADING_OPERATION_STATUS_INPROGRESS;
    m_serializer = std::make_shared<CMeshSerializer_MDL>(m_meshFilename,
                                                         m_resource);
    m_serializer->serialize();
    
    m_status = m_serializer->getStatus() == E_SERIALIZER_STATUS_SUCCESS ? E_RESOURCE_LOADING_OPERATION_STATUS_WAITING : E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE;
    
    m_serializer = std::make_shared<CSkeletonSerializer_SK>(m_skeletonFilename,
                                                            m_resource);
    m_serializer->serialize();
    
    m_status = m_serializer->getStatus() == E_SERIALIZER_STATUS_SUCCESS && m_status != E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE ? E_RESOURCE_LOADING_OPERATION_STATUS_WAITING : E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE;
    
    m_serializer = std::make_shared<CSequenceSerializer_SEQ>(m_sequenceFilename,
                                                             m_resource);
    m_serializer->serialize();
    
    m_status = m_serializer->getStatus() == E_SERIALIZER_STATUS_SUCCESS && m_status != E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE ? E_RESOURCE_LOADING_OPERATION_STATUS_WAITING : E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE;
}

void CMeshLoadingOperation::commit(void)
{
    assert(m_resource != nullptr);
    assert(m_resource->isLoaded() == true);
    CSharedMesh mesh = std::static_pointer_cast<CMesh>(m_resource);
    m_commiter = std::make_shared<CMeshCommiter_MDL>(m_meshFilename,
                                                     m_resource);
    m_commiter->commit();
    m_status = m_commiter->getStatus() == E_COMMITER_STATUS_SUCCESS ? E_RESOURCE_LOADING_OPERATION_STATUS_WAITING : E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE;
    
    
    m_commiter = std::make_shared<CSkeletonCommiter_SK>(m_skeletonFilename,
                                                        m_resource);
    m_commiter->commit();
    
    m_status = m_serializer->getStatus() == E_SERIALIZER_STATUS_SUCCESS && m_status != E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE ? E_RESOURCE_LOADING_OPERATION_STATUS_WAITING : E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE;
    
    m_commiter = std::make_shared<CSequenceCommiter_SEQ>(m_serializer->getGuid(),
                                                         m_resource);
    m_commiter->commit();
    
    m_status = m_serializer->getStatus() == E_SERIALIZER_STATUS_SUCCESS && m_status != E_RESOURCE_LOADING_OPERATION_STATUS_WAITING ? E_RESOURCE_LOADING_OPERATION_STATUS_SUCCESS : E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE;
    
    IResourceLoadingOperation::onResourceLoaded();
}

