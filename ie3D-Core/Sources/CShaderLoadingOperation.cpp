//
//  CShaderLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CShaderLoadingOperation.h"
#include "CShader.h"
#include "CShaderSerializer_GLSL.h"
#include "CShaderCommiter_GLSL.h"

CShaderLoadingOperation::CShaderLoadingOperation(const std::string& vsFilename,
                                                 const std::string& fsFilename,
                                                 ISharedResourceRef resource) :
IResourceLoadingOperation(std::string().append(vsFilename).append(fsFilename), resource),
m_vsFilename(vsFilename),
m_fsFilename(fsFilename)
{
    
}

CShaderLoadingOperation::~CShaderLoadingOperation(void)
{
    
}

void CShaderLoadingOperation::serialize(void)
{
    assert(m_resource != nullptr);
    m_status = E_RESOURCE_LOADING_OPERATION_STATUS_INPROGRESS;
    m_serializer = std::make_shared<CShaderSerializer_GLSL>(m_vsFilename,
                                                            m_fsFilename,
                                                            m_resource);
    m_serializer->serialize();
    m_status = m_serializer->getStatus() == E_SERIALIZER_STATUS_SUCCESS ? E_RESOURCE_LOADING_OPERATION_STATUS_WAITING : E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE;
}

void CShaderLoadingOperation::commit(void)
{
    assert(m_resource != nullptr);
    assert(m_resource->isLoaded() == true);
    std::shared_ptr<CShader> shader = std::static_pointer_cast<CShader>(m_resource);
    m_commiter = std::make_shared<CShaderCommiter_GLSL>(m_serializer->getGuid(),
                                                        m_vsFilename,
                                                        m_fsFilename,
                                                        shader->getVSSourceCode(),
                                                        shader->getFSSourceCode(),
                                                        m_resource);
    m_commiter->commit();
    m_status = m_commiter->getStatus() == E_COMMITER_STATUS_SUCCESS ? E_RESOURCE_LOADING_OPERATION_STATUS_SUCCESS : E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE;
    IResourceLoadingOperation::onResourceLoaded();
}