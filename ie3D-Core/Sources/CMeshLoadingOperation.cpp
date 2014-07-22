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
#include "CMeshSerializer_MDL_01.h"
#include "CMeshCommiter_MDL_01.h"
#include "CSkeletonSerializer_SK.h"
#include "CSkeletonCommiter_SK.h"
#include "CSequenceSerializer_SEQ.h"
#include "CSequenceCommiter_SEQ.h"
#include "IResource.h"
#include "CMesh.h"

CMeshLoadingOperation::CMeshLoadingOperation(const std::string& filename,
                                             ISharedResourceRef resource) :
IResourceLoadingOperation(filename, resource),
m_filename(filename)
{
    
}

CMeshLoadingOperation::~CMeshLoadingOperation(void)
{
    
}

void CMeshLoadingOperation::serialize(void)
{
    assert(m_resource != nullptr);
    m_status = E_RESOURCE_LOADING_OPERATION_STATUS_INPROGRESS;
    if(m_filename.find(".mdl_01") != std::string::npos)
    {
        m_serializer = std::make_shared<CMeshSerializer_MDL_01>(m_filename,
                                                                m_resource);
        m_serializer->serialize();
    }
    else if(m_filename.find(".mdl") != std::string::npos)
    {
        std::string meshFilename = m_filename;
        meshFilename.append("_mesh");
        m_serializer = std::make_shared<CMeshSerializer_MDL>(meshFilename,
                                                             m_resource);
        m_serializer->serialize();
        
        m_status = m_serializer->getStatus() == E_SERIALIZER_STATUS_SUCCESS ? E_RESOURCE_LOADING_OPERATION_STATUS_WAITING : E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE;
        
        std::string skeletonFilename = m_filename;
        skeletonFilename.append("_sk");
        m_serializer = std::make_shared<CSkeletonSerializer_SK>(skeletonFilename,
                                                                m_resource);
        m_serializer->serialize();
        
        m_status = m_serializer->getStatus() == E_SERIALIZER_STATUS_SUCCESS && m_status != E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE ? E_RESOURCE_LOADING_OPERATION_STATUS_WAITING : E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE;
        
        std::string sequenceFilename = m_filename;
        sequenceFilename.append("_anim");
        m_serializer = std::make_shared<CSequenceSerializer_SEQ>(sequenceFilename,
                                                                 m_resource);
        m_serializer->serialize();
    }
    else
    {
        assert(false);
    }
    m_status = m_serializer->getStatus() == E_SERIALIZER_STATUS_SUCCESS && m_status != E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE ? E_RESOURCE_LOADING_OPERATION_STATUS_WAITING : E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE;
}

void CMeshLoadingOperation::commit(void)
{
    assert(m_resource != nullptr);
    assert(m_resource->isLoaded() == true);
    
    if(m_filename.find(".mdl") != std::string::npos)
    {
        m_commiter = std::make_shared<CMeshCommiter_MDL>(m_filename,
                                                         m_resource);
        m_commiter->commit();
        m_status = m_commiter->getStatus() == E_COMMITER_STATUS_SUCCESS ? E_RESOURCE_LOADING_OPERATION_STATUS_WAITING : E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE;
        
        
        m_commiter = std::make_shared<CSkeletonCommiter_SK>(m_filename,
                                                            m_resource);
        m_commiter->commit();
        
        m_status = m_serializer->getStatus() == E_SERIALIZER_STATUS_SUCCESS && m_status != E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE ? E_RESOURCE_LOADING_OPERATION_STATUS_WAITING : E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE;
        
        m_commiter = std::make_shared<CSequenceCommiter_SEQ>(m_filename,
                                                             m_resource);
        m_commiter->commit();
    }
    else if(m_filename.find(".mdl_01") != std::string::npos)
    {
        m_commiter = std::make_shared<CMeshCommiter_MDL_01>(m_filename,
                                                            m_resource);
        m_commiter->commit();
    }
    else
    {
        assert(false);
    }

    
    m_status = m_serializer->getStatus() == E_SERIALIZER_STATUS_SUCCESS && m_status != E_RESOURCE_LOADING_OPERATION_STATUS_WAITING ? E_RESOURCE_LOADING_OPERATION_STATUS_SUCCESS : E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE;
    
    IResourceLoadingOperation::onResourceLoaded();
}

