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

CShaderLoadingOperation::CShaderLoadingOperation(const std::string& _vsFilename, const std::string& _fsFilename, std::shared_ptr<IResource> _resource) :
IResourceLoadingOperation(std::string().append(_vsFilename).append(_fsFilename), _resource),
m_vsFilename(_vsFilename),
m_fsFilename(_fsFilename)
{
    
}

CShaderLoadingOperation::~CShaderLoadingOperation(void)
{
    
}

void CShaderLoadingOperation::Serialize(void)
{
    assert(m_resource != nullptr);
    m_status = E_RESOURCE_LOADING_OPERATION_STATUS_INPROGRESS;
    m_serializer = std::make_shared<CShaderSerializer_GLSL>(m_vsFilename,
                                                            m_fsFilename,
                                                            m_resource);
    m_serializer->Serialize();
    m_status = m_serializer->Get_Status() == E_SERIALIZER_STATUS_SUCCESS ? E_RESOURCE_LOADING_OPERATION_STATUS_WAITING : E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE;
}

void CShaderLoadingOperation::Commit(void)
{
    assert(m_resource != nullptr);
    assert(m_resource->IsLoaded() == true);
    std::shared_ptr<CShader> shader = std::static_pointer_cast<CShader >(m_resource);
    m_commiter = std::make_shared<CShaderCommiter_GLSL>(m_serializer->Get_Guid(),
                                                        shader->Get_VertexShaderSourceCode(),
                                                        shader->Get_FragmentShaderSourceCode(),
                                                        m_resource);
    m_commiter->Commit();
    m_status = m_commiter->Get_Status() == E_COMMITER_STATUS_SUCCESS ? E_RESOURCE_LOADING_OPERATION_STATUS_SUCCESS : E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE;
    m_resource->_OnLoaded();
}