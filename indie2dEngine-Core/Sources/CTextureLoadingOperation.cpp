//
//  CTextureLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/22/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CTextureLoadingOperation.h"
#include "CTexture.h"
#include "CTextureSerializer_PVR.h"
#include "CTextureCommiter_PVR.h"

CTextureLoadingOperation::CTextureLoadingOperation(const std::string& _filename, std::shared_ptr<IResource> _resource) :
IResourceLoadingOperation(_filename, _resource),
m_filename(_filename)
{
    
}

CTextureLoadingOperation::~CTextureLoadingOperation(void)
{
    
}

void CTextureLoadingOperation::Serialize(void)
{
    assert(m_resource != nullptr);
    m_status = E_RESOURCE_LOADING_OPERATION_STATUS_INPROGRESS;
    m_serializer = std::make_shared<CTextureSerializer_PVR>(m_filename,
                                                            m_resource);
    m_serializer->Serialize();
    m_status = m_serializer->Get_Status() == E_SERIALIZER_STATUS_SUCCESS ? E_RESOURCE_LOADING_OPERATION_STATUS_WAITING : E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE;
}

void CTextureLoadingOperation::Commit(void)
{
    assert(m_resource != nullptr);
    assert(m_resource->IsLoaded() == true);
    std::shared_ptr<CTexture> texture = std::static_pointer_cast<CTexture >(m_resource);
    m_commiter = std::make_shared<CTextureCommiter_PVR>(m_serializer->Get_Guid(),
                                                        texture->Get_Header(),
                                                        m_resource);
    m_commiter->Commit();
    m_status = m_commiter->Get_Status() == E_COMMITER_STATUS_SUCCESS ? E_RESOURCE_LOADING_OPERATION_STATUS_SUCCESS : E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE;
    m_resource->_OnLoaded();
}