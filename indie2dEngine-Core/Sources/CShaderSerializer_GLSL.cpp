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

CShaderSerializer_GLSL::CShaderSerializer_GLSL(const std::string& vsFilename,
                                               const std::string& fsFilename,
                                               ISharedResourceRef resource) :
IResourceSerializer(std::string().append(vsFilename).append(fsFilename), resource),
m_vsFilename(vsFilename),
m_fsFilename(fsFilename)
{
    
}

CShaderSerializer_GLSL::~CShaderSerializer_GLSL(void)
{
    
}

void CShaderSerializer_GLSL::serialize(void)
{
    IResourceSerializer::onResourceDataSerializationStatusChanged(nullptr, E_RESOURCE_DATA_STATUS_STARTED);
    assert(m_resource != nullptr);
    m_status = E_SERIALIZER_STATUS_INPROGRESS;
    
    std::shared_ptr<std::istream> filestream = IResourceSerializer::openStream(m_vsFilename);
    std::stringstream vsStringstream;
    vsStringstream<<filestream->rdbuf();
    std::string vsSourceCode(vsStringstream.str());
    IResourceSerializer::closeStream(filestream);
    
    filestream = IResourceSerializer::openStream(m_fsFilename);
    std::stringstream fsStringstream;
    fsStringstream<<filestream->rdbuf();
    std::string fsSourceCode(fsStringstream.str());
    IResourceSerializer::closeStream(filestream);
    
    assert(m_resource != nullptr);
    std::shared_ptr<CShader> shader = std::static_pointer_cast<CShader>(m_resource);
    std::shared_ptr<CShaderData> sharedData = std::make_shared<CShaderData>(m_vsFilename,
                                                                            m_fsFilename,
                                                                            vsSourceCode,
                                                                            fsSourceCode);
    IResourceSerializer::onResourceDataSerializationStatusChanged(sharedData, E_RESOURCE_DATA_STATUS_PROGRESS);
    m_status = E_SERIALIZER_STATUS_SUCCESS;
    IResourceSerializer::onResourceDataSerializationStatusChanged(nullptr, E_RESOURCE_DATA_STATUS_FINISHED);
}

