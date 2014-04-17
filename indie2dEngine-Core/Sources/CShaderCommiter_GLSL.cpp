//
//  CShaderCommiter_GLSL.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CShaderCommiter_GLSL.h"
#include "CShader.h"

CShaderCommiter_GLSL::CShaderCommiter_GLSL(const std::string& guid,
                                           const std::string& vsSourceCode,
                                           const std::string& fsSourceCode,
                                           ISharedResourceRef resource) :
IResourceCommiter(guid, resource),
m_vsSourceCode(vsSourceCode),
m_fsSourceCode(fsSourceCode)
{
    
}

CShaderCommiter_GLSL::~CShaderCommiter_GLSL(void)
{
    
}

ui32 CShaderCommiter_GLSL::compile(const std::string &sourceCode,
                                   GLenum shaderType)
{
    ui32 handle = glCreateShader(shaderType);
    const char* data = sourceCode.c_str();
    glShaderSource(handle, 1, &data, 0);
    glCompileShader(handle);
    
    i32 success;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
    
    if (success == GL_FALSE)
    {
        GLchar message[256];
        glGetShaderInfoLog(handle, sizeof(message), 0, &message[0]);
        m_message = message;
#if defined(__NDK__)
        NDK_LOG("Error: GLSL guid %s", m_guid.c_str());
        NDK_LOG("Error: GLSL sourcecode %s", sourceCode.c_str());
        NDK_LOG("Error: GLSL compile %s", m_message.c_str());
#endif
        std::cout<<message<<std::endl;
        m_status = E_COMMITER_STATUS_FAILURE;
#if defined(__IOS__)
        assert(false);
#endif
    }
    return handle;
}

ui32 CShaderCommiter_GLSL::link(ui32 vsHandle,
                                ui32 fsHandle)
{
    ui32 handle = glCreateProgram();
    glAttachShader(handle, vsHandle);
    glAttachShader(handle, fsHandle);
    glLinkProgram(handle);
    
    i32 success;
    glGetProgramiv(handle, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLchar message[256];
        glGetProgramInfoLog(handle, sizeof(message), 0, &message[0]);
        m_message = message;
        std::cout<<m_message<<std::endl;
        m_status = E_COMMITER_STATUS_FAILURE;
#if defined(__IOS__)
        assert(false);
#endif
    }
    return handle;
}

void CShaderCommiter_GLSL::commit(void)
{
    IResourceCommiter::onResourceDataCommitStatusChanged(nullptr, E_RESOURCE_DATA_STATUS_STARTED);
    std::string message = "";
    m_status = E_COMMITER_STATUS_INPROGRESS;
    ui32 vsHandle = CShaderCommiter_GLSL::compile(m_vsSourceCode, GL_VERTEX_SHADER);
    if(m_status == E_COMMITER_STATUS_FAILURE)
    {
        message = "<font color=\"red\"><b>VERTEX SHADER COMPILE ERROR:</b><br>" + m_message + "</font><br>";
        m_message = message;
        return;
    }
    else
    {
        message = "<font color=\"green\"><b>VERTEX SHADER COMPILE SUCCESS.</b></font><br>";
    }
    ui32 fsHandle = CShaderCommiter_GLSL::compile(m_fsSourceCode, GL_FRAGMENT_SHADER);
    if(m_status == E_COMMITER_STATUS_FAILURE)
    {
        message += "<font color=\"red\"><b>FRAGMENT SHADER COMPILE ERROR:</b><br>" + m_message + "</font><br>";
        m_message = message;
        return;
    }
    else
    {
        message += "<font color=\"green\"><b>FRAGMENT SHADER COMPILE SUCCESS.</b></font><br>";
    }
    
    ui32 shaderId = CShaderCommiter_GLSL::link(vsHandle, fsHandle);
    if(m_status == E_COMMITER_STATUS_FAILURE)
    {
        message += "<font color=\"red\"><b>SHADER LINKING ERROR:</b><br>" + m_message + "</font>";
        m_message = message;
        return;
    }
    else
    {
        message += "<font color=\"green\"><b>SHADER LINKING SUCCESS.</b></font>";
    }
    m_message = message;
    assert(m_resource != nullptr);
    assert(m_resource->isLoaded() == true);
    std::shared_ptr<CShader> shader = std::static_pointer_cast<CShader>(m_resource);
    IResourceCommiter::onResourceDataCommitStatusChanged(std::make_shared<CShaderData>(shaderId), E_RESOURCE_DATA_STATUS_PROGRESS);
    m_status = m_status == E_COMMITER_STATUS_INPROGRESS ? E_COMMITER_STATUS_SUCCESS : E_COMMITER_STATUS_FAILURE;
    IResourceCommiter::onResourceDataCommitStatusChanged(nullptr, E_RESOURCE_DATA_STATUS_FINISHED);
}

