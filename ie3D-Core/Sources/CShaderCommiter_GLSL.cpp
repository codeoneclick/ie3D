//
//  CShaderCommiter_GLSL.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CShaderCommiter_GLSL.h"
#include "CShader.h"
#include "CShaderCompiler_GLSL.h"

CShaderCommiter_GLSL::CShaderCommiter_GLSL(const std::string& guid,
                                           const std::string& vsFilename,
                                           const std::string& fsFilename,
                                           const std::string& vsSourceCode,
                                           const std::string& fsSourceCode,
                                           ISharedResourceRef resource) :
IResourceCommiter(guid, resource),
m_vsSourceCode(vsSourceCode),
m_fsSourceCode(fsSourceCode),
m_vsFilename(vsFilename),
m_fsFilename(fsFilename)
{
    
}

CShaderCommiter_GLSL::~CShaderCommiter_GLSL(void)
{
    
}

ui32 CShaderCommiter_GLSL::compile(const std::string &sourceCode,
                                   GLenum shaderType)
{
    std::string outMessage = "";
    bool outSuccess = false;
    ui32 handle = CShaderCompiler_GLSL::compile(sourceCode, shaderType, &outMessage, &outSuccess);
    
    std::string messageTemplate = "";
    if(shaderType == GL_VERTEX_SHADER)
    {
        messageTemplate.append("Vertex shader: ").append(m_vsFilename);
    }
    else if(shaderType == GL_FRAGMENT_SHADER)
    {
        messageTemplate.append("Fragment shader: ").append(m_fsFilename);
    }
    if(outMessage.length() != 0)
    {
        std::cout<<messageTemplate<<"\n"<<outMessage<<std::endl;
    }
    
    if (outSuccess == GL_FALSE)
    {
#if defined(__NDK__)
        NDK_LOG("Error: GLSL guid %s", m_guid.c_str());
        NDK_LOG("Error: GLSL sourcecode %s", sourceCode.c_str());
        NDK_LOG("Error: GLSL compile %s", m_message.c_str());
#endif
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
    std::string outMessage = "";
    bool outSuccess = false;
    ui32 handle = CShaderCompiler_GLSL::link(vsHandle, fsHandle, &outMessage, &outSuccess);
    
    if(outMessage.length() != 0)
    {
        std::cout<<"Vertex shader: "<<m_vsFilename<<" Fragment shader: "<<m_fsFilename<<"\n"<<outMessage<<std::endl;
    }
    
    if (outSuccess == GL_FALSE)
    {
        m_status = E_COMMITER_STATUS_FAILURE;
#if defined(__IOS__)
        assert(false);
#endif
    }
    return handle;
}

void CShaderCommiter_GLSL::commit(void)
{
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
    m_status = m_status == E_COMMITER_STATUS_INPROGRESS ? E_COMMITER_STATUS_SUCCESS : E_COMMITER_STATUS_FAILURE;
    IResourceCommiter::onResourceDataCommitFinished(std::make_shared<CShaderData>(shaderId));
}

