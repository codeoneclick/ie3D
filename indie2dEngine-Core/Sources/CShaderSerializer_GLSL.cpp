//
//  CShaderSerializer_GLSL.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/16/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CShaderSerializer_GLSL.h"
#include "CCommonOS.h"

CShaderSerializer_GLSL::CShaderSerializer_GLSL(const std::string& _vsFilename, const std::string& _fsFilename) :
IResourceSerializer(std::string().append(_vsFilename).append(_fsFilename)),
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
    
    std::ifstream filestream;
    filestream.open(filename.c_str());
    if (!filestream.is_open())
    {
        m_status = E_SERIALIZER_STATUS_FAILURE;
        return;
    }
    filestream.seekg(0, std::ios::end);
    i32 lenght = filestream.tellg();
    filestream.seekg(0, std::ios::beg);
	char* data = new char[lenght];
	filestream.read(data, lenght);
    filestream.close();
    
    assert(data != nullptr);
    m_vsSourceCode.assign(data);
    
    filename.assign(path);
    filename.append(m_fsFilename);
    
    filestream.open(filename.c_str());
    if (!filestream.is_open())
    {
        m_status = E_SERIALIZER_STATUS_FAILURE;
        return;
    }
    filestream.seekg(0, std::ios::end);
    lenght = filestream.tellg();
    filestream.seekg(0, std::ios::beg);
	data = new char[lenght];
	filestream.read(data, lenght);
    filestream.close();
    
    assert(data != nullptr);
    m_fsSourceCode.assign(data);
    
    m_status = E_SERIALIZER_STATUS_SUCCESS;
}

