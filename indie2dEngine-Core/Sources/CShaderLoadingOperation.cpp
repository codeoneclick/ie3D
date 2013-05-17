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

CShaderLoadingOperation::CShaderLoadingOperation(const std::string& _vsFilename, const std::string& _fsFilename) :
IResourceLoadingOperation(std::string().append(_vsFilename).append(_fsFilename)),
m_vsFilename(_vsFilename),
m_fsFilename(_fsFilename)
{
    
}

CShaderLoadingOperation::~CShaderLoadingOperation(void)
{
    
}

std::shared_ptr<IResource> CShaderLoadingOperation::Start(void)
{
    m_status = E_RESOURCE_LOADING_OPERATION_STATUS_INPROGRESS;
    m_serializer = std::make_shared<CShaderSerializer_GLSL>(m_vsFilename, m_fsFilename);
    m_serializer->Serialize();
    if(m_serializer->Get_Status() == E_SERIALIZER_STATUS_SUCCESS)
    {
        m_commiter = std::make_shared<CShaderCommiter_GLSL>(m_serializer->Get_Guid(),
                                                            std::static_pointer_cast<CShaderSerializer_GLSL >(m_serializer)->Get_VertexShaderSourceCode(),
                                                            std::static_pointer_cast<CShaderSerializer_GLSL >(m_serializer)->Get_FragmentShaderSourceCode());
        m_commiter->Commit();
        if(m_commiter->Get_Status() == E_COMMITER_STATUS_SUCCESS)
        {
            std::shared_ptr<CShader> shader = std::make_shared<CShader>(m_commiter->Get_Guid());
            shader->Link(std::static_pointer_cast<CShaderCommiter_GLSL >(m_commiter)->Get_Handle());
            m_status = E_RESOURCE_LOADING_OPERATION_STATUS_SUCCESS;
            return shader;
        }
    }
    m_status = E_RESOURCE_LOADING_OPERATION_STATUS_FAILURE;
    return nullptr;
}