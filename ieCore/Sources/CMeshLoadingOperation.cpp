//
//  CMeshLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/26/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CMeshLoadingOperation.h"
#include "CMesh.h"
#include "CMeshSerializer_MDL.h"
#include "CMeshCommiter_MDL.h"

CMeshLoadingOperation::CMeshLoadingOperation(const std::string& _filename, std::shared_ptr<IResource> _resource) :
IResourceLoadingOperation(_filename, _resource),
m_filename(_filename)
{
    
}

CMeshLoadingOperation::~CMeshLoadingOperation(void)
{
    
}

void CMeshLoadingOperation::Serialize(void)
{
    assert(m_resource != nullptr);
    m_status = E_RESOURCE_LOADING_OPERATION_STATUS_INPROGRESS;
    m_serializer = std::make_shared<CMeshSerializer_MDL>(m_filename,
                                                         m_resource);
    m_serializer->Serialize();
    m_status = m_serializer->Get_Status() == E_SERIALIZER_STATUS_SUCCESS ? E_RESOURCE_LOADING_OPERATION_STATUS_WAITING : E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE;
}

void CMeshLoadingOperation::Commit(void)
{
    assert(m_resource != nullptr);
    assert(m_resource->IsLoaded() == true);
    std::shared_ptr<CMesh> mesh = std::static_pointer_cast<CMesh >(m_resource);
    m_commiter = std::make_shared<CMeshCommiter_MDL>(m_serializer->Get_Guid(),
                                                    mesh->_Get_Header(),
                                                    m_resource);
    m_commiter->Commit();
    m_status = m_commiter->Get_Status() == E_COMMITER_STATUS_SUCCESS ? E_RESOURCE_LOADING_OPERATION_STATUS_SUCCESS : E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE;
    m_resource->_OnLoaded();
}