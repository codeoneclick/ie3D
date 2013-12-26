//
//  CShaderSerializer_GLSL.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/16/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CShaderSerializer_GLSL.h"
#include "CShader.h"
#include "CResourceAccessor.h"

CShaderSerializer_GLSL::CShaderSerializer_GLSL(const std::string& _vsFilename, const std::string& _fsFilename, std::shared_ptr<IResource> _resource) :
IResourceSerializer(std::string().append(_vsFilename).append(_fsFilename), _resource),
m_vsFilename(_vsFilename),
m_fsFilename(_fsFilename)
{
    
}

CShaderSerializer_GLSL::~CShaderSerializer_GLSL(void)
{
    
}

void CShaderSerializer_GLSL::Serialize(void)
{
    m_status = E_SERIALIZER_STATUS_INPROGRESS;
    
    std::istream* filestream = IResourceSerializer::_LoadData(m_vsFilename);
    std::stringstream vsStringstream;
    vsStringstream<<filestream->rdbuf();
    std::string vsSourceCode(vsStringstream.str());
    IResourceSerializer::_FreeData(filestream);
    
    filestream = IResourceSerializer::_LoadData(m_fsFilename);
    std::stringstream fsStringstream;
    fsStringstream<<filestream->rdbuf();
    std::string fsSourceCode(fsStringstream.str());
    IResourceSerializer::_FreeData(filestream);
    
    assert(m_resource != nullptr);
    std::shared_ptr<CShader> shader = std::static_pointer_cast<CShader >(m_resource);
    shader->Set_SourceCode(m_vsFilename,
                           vsSourceCode,
                           m_fsFilename,
                           fsSourceCode);
    m_status = E_SERIALIZER_STATUS_SUCCESS;
}

