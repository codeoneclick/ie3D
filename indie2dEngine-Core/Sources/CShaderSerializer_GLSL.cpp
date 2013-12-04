//
//  CShaderSerializer_GLSL.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/16/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CShaderSerializer_GLSL.h"
#include "CCommonOS.h"
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
    
    std::string path(Get_BundlePath());
    std::string filename(path);
    filename.append(m_vsFilename);
    
    std::istream* filestream = IResourceSerializer::_LoadData(filename);
    std::stringstream vsStringstream;
    vsStringstream<<filestream->rdbuf();
    std::string vsSourceCode(vsStringstream.str());
    IResourceSerializer::_FreeData(filestream);
    
    filename.assign(path);
    filename.append(m_fsFilename);
    
    filestream = IResourceSerializer::_LoadData(filename);
    std::stringstream fsStringstream;
    fsStringstream<<filestream->rdbuf();
    std::string fsSourceCode(fsStringstream.str());
    IResourceSerializer::_FreeData(filestream);
    
    assert(m_resource != nullptr);
    std::shared_ptr<CShader> shader = std::static_pointer_cast<CShader >(m_resource);
    shader->_Set_SourceCode(vsSourceCode, fsSourceCode);
    m_status = E_SERIALIZER_STATUS_SUCCESS;
}

