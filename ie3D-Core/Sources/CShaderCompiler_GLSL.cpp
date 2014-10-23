//
//  CShaderCompiler_GLSL.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 8/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CShaderCompiler_GLSL.h"
#include "IResourceSerializer.h"

bool CShaderCompiler_GLSL::m_isVSCommonShaderLoaded = false;
bool CShaderCompiler_GLSL::m_isFSCommonShaderLoaded = false;

std::string CShaderCompiler_GLSL::m_vsShaderCommon = "";
std::string CShaderCompiler_GLSL::m_fsShaderCommon = "";

std::string CShaderCompiler_GLSL::getCommonShader(const std::string& filename)
{
    std::string shaderCommon;
    E_SERIALIZER_STATUS status = E_SERIALIZER_STATUS_UNKNOWN;
    std::shared_ptr<std::istream> filestream = IResourceSerializer::openStream(filename, &status);
    std::stringstream stringstream;
    if(status != E_SERIALIZER_STATUS_FAILURE)
    {
        stringstream<<filestream->rdbuf();
        shaderCommon = stringstream.str();
        IResourceSerializer::closeStream(filestream);
    }
    else
    {
        assert(false);
    }
    return shaderCommon;
}

std::string CShaderCompiler_GLSL::getVSCommonShader(void)
{
    if(!m_isVSCommonShaderLoaded)
    {
        m_vsShaderCommon = CShaderCompiler_GLSL::getCommonShader("shaderCommon.vert");
        m_isVSCommonShaderLoaded = true;
    }
    return m_vsShaderCommon;
}

std::string CShaderCompiler_GLSL::getFSCommonShader(void)
{
    if(!m_isFSCommonShaderLoaded)
    {
        m_fsShaderCommon = CShaderCompiler_GLSL::getCommonShader("shaderCommon.frag");
        m_isFSCommonShaderLoaded = true;
    }
    return m_fsShaderCommon;
}

ui32 CShaderCompiler_GLSL::compile(const std::string& sourceCode, GLenum shaderType, std::string* outMessage, bool* outSuccess)
{
    ui32 handle = glCreateShader(shaderType);
    
    std::string shaderCommon;
    if(shaderType == GL_VERTEX_SHADER)
    {
        shaderCommon = CShaderCompiler_GLSL::getVSCommonShader();
    }
    else if(shaderType == GL_FRAGMENT_SHADER)
    {
        shaderCommon = CShaderCompiler_GLSL::getFSCommonShader();
    }
    
    std::string define = "";
#if defined(__OPENGL_30__)
    
#if defined(__OSX__)
    
    define.append("#version 410\n");
    
#elif defined(__IOS__)
    
    define.append("#version 300 es\n");
    
#endif
#endif
    
#if defined(__OSX__)
    
    define.append("#define __OSX__\n");
    
#elif defined(__IOS__)
    
    define.append("#define __IOS__\n");
    if(g_highPerformancePlatforms.count(getPlatform()) != 0)
    {
        define.append("#define __IOS_HIGH_PERFORMANCE__\n");
    }
    
#endif
    
#if defined(__OPENGL_30__)
    
    define.append("#define __OPENGL_30__\n");
    
#endif
    
    define.append(shaderCommon);
    
    char* shaderData = const_cast<char*>(sourceCode.c_str());
    char* defineData = const_cast<char*>(define.c_str());
    char* sources[2] = { defineData, shaderData};
    glShaderSource(handle, 2, sources, NULL);
    glCompileShader(handle);
    
    i32 success;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
    
    if(!success)
    {
        i32 messageSize = 0;
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &messageSize);
        GLchar *messageString = new GLchar[messageSize];
        memset(messageString, NULL, messageSize * sizeof(GLchar));
        glGetShaderInfoLog(handle, messageSize, NULL, messageString);
        *outMessage = messageString;
    }
    else
    {
        outMessage = nullptr;
    }
    *outSuccess = success;
    return handle;
}

ui32 CShaderCompiler_GLSL::link(ui32 vsHandle, ui32 fsHandle, std::string* outMessage, bool* outSuccess)
{
    ui32 handle = glCreateProgram();
    glAttachShader(handle, vsHandle);
    glAttachShader(handle, fsHandle);
    glLinkProgram(handle);
    
    i32 success;
    glGetProgramiv(handle, GL_LINK_STATUS, &success);
    
    if(!success)
    {
        i32 messageSize = 0;
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &messageSize);
        GLchar *messageString = new GLchar[messageSize];
        memset(messageString, NULL, messageSize * sizeof(GLchar));
        glGetShaderInfoLog(handle, messageSize, NULL, messageString);
        *outMessage = messageString;
    }
    else
    {
        outMessage = nullptr;
    }
    *outSuccess = success;
    return handle;
}