//
//  CShaderCompiler_GLSL.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 8/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CShaderCompiler_GLSL.h"


ui32 CShaderCompiler_GLSL::compile(const std::string& sourceCode, GLenum shaderType, std::string* outMessage, bool* outSuccess)
{
    ui32 handle = glCreateShader(shaderType);
    
    std::string define = "";
#if defined(__OSX__)
    define = "#define __OSX__\n";
#elif defined(__IOS__)
    define = "#define __IOS__\n";
    if(g_highPerformancePlatforms.count(getPlatform()) != 0)
    {
        define.append("#define __IOS_HIGH_PERFORMANCE__\n");
    }
#else
    define = "";
#endif
    
    char* shaderData = const_cast<char*>(sourceCode.c_str());
    char* defineData = const_cast<char*>(define.c_str());
    char* sources[2] = { defineData, shaderData};
    glShaderSource(handle, 2, sources, NULL);
    glCompileShader(handle);
    
    i32 success;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
    
    GLchar message[1024];
    memset(message, 0x0, 1024 * sizeof(GLchar));
    glGetShaderInfoLog(handle, sizeof(message), 0, &message[0]);
    *outMessage = message;
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
    
    GLchar message[1024];
    memset(message, 0x0, 1024 * sizeof(GLchar));
    glGetProgramInfoLog(handle, sizeof(message), 0, &message[0]);
    *outMessage = message;
    *outSuccess = success;
    return handle;
}