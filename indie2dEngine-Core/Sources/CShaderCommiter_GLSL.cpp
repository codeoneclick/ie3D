//
//  CShaderCommiter_GLSL.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CShaderCommiter_GLSL.h"

CShaderCommiter_GLSL::CShaderCommiter_GLSL(const std::string& _guid, const std::string& _vsSourceCode, const std::string& _fsSourceCode) :
IResourceCommiter(_guid),
m_vsSourceCode(_vsSourceCode),
m_fsSourceCode(_fsSourceCode)
{
    
}

CShaderCommiter_GLSL::~CShaderCommiter_GLSL(void)
{
    
}

ui32 CShaderCommiter_GLSL::_Compile(const std::string &_sourceCode,  GLenum _shader)
{
    ui32 handle = glCreateShader(_shader);
    const char* data = _sourceCode.c_str();
    glShaderSource(handle, 1, &data, 0);
    glCompileShader(handle);
    
    i32 success;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
    
    if (success == GL_FALSE)
    {
        GLchar messages[256];
        glGetShaderInfoLog(handle, sizeof(messages), 0, &messages[0]);
        std::cout<<messages;
        m_status = E_COMMITER_STATUS_FAILURE;
        assert(false);
    }
    return handle;
}

ui32 CShaderCommiter_GLSL::_Link(ui32 _vsHandle, ui32 _fsHandle)
{
    ui32 handle = glCreateProgram();
    glAttachShader(handle, _vsHandle);
    glAttachShader(handle, _fsHandle);
    glLinkProgram(handle);
    
    i32 success;
    glGetProgramiv(handle, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLchar messages[256];
        glGetProgramInfoLog(handle, sizeof(messages), 0, &messages[0]);
        std::cout << messages;
        m_status = E_COMMITER_STATUS_FAILURE;
        assert(false);
    }
    m_status = E_COMMITER_STATUS_SUCCESS;
    return handle;
}

void CShaderCommiter_GLSL::Commit(void)
{
    m_status = E_COMMITER_STATUS_INPROGRESS;
    ui32 vsHandle = _Compile(m_vsSourceCode, GL_VERTEX_SHADER);
    ui32 fsHandle = _Compile(m_fsSourceCode, GL_FRAGMENT_SHADER);
    m_handle = _Link(vsHandle, fsHandle);
}

