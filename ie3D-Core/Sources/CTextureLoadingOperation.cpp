//
//  CTextureLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/22/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CTextureLoadingOperation.h"
#include "CTextureSerializer_PVR.h"
#include "CTextureCommiter_PVR.h"
#include "CTextureSerializer_PNG.h"
#include "CTextureCommiter_PNG.h"
#include "IResource.h"
#include "CTexture.h"

CTextureLoadingOperation::CTextureLoadingOperation(const std::string& filename, ISharedResourceRef resource) :
IResourceLoadingOperation(filename, resource),
m_filename(filename)
{
    
}

CTextureLoadingOperation::~CTextureLoadingOperation(void)
{
    
}

void CTextureLoadingOperation::serialize(void)
{
    assert(m_resource != nullptr);
    m_status = E_RESOURCE_LOADING_OPERATION_STATUS_INPROGRESS;
    
    if(m_filename.find(".pvr") != std::string::npos)
    {
        m_serializer = std::make_shared<CTextureSerializer_PVR>(m_filename,
                                                                m_resource);
    }
    else if(m_filename.find(".png") != std::string::npos)
    {
        m_serializer = std::make_shared<CTextureSerializer_PNG>(m_filename,
                                                                m_resource);
    }
    else
    {
        assert(false);
    }
    
    m_serializer->serialize();
    m_status = m_serializer->getStatus() == E_SERIALIZER_STATUS_SUCCESS ? E_RESOURCE_LOADING_OPERATION_STATUS_WAITING : E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE;
}

void CTextureLoadingOperation::commit(void)
{
    assert(m_resource != nullptr);
    assert(m_resource->isLoaded() == true);
    CSharedTexture texture = std::static_pointer_cast<CTexture>(m_resource);
    
    if(m_filename.find(".pvr") != std::string::npos)
    {
        m_commiter = std::make_shared<CTextureCommiter_PVR>(m_serializer->getGuid(),
                                                            m_resource);
    }
    else if(m_filename.find(".png") != std::string::npos)
    {
        m_commiter = std::make_shared<CTextureCommiter_PNG>(m_serializer->getGuid(),
                                                            m_resource);
    }
    else
    {
        assert(false);
    }
    
    m_commiter->commit();
    m_status = m_commiter->getStatus() == E_COMMITER_STATUS_SUCCESS ? E_RESOURCE_LOADING_OPERATION_STATUS_SUCCESS : E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE;
    IResourceLoadingOperation::onResourceLoaded();
}