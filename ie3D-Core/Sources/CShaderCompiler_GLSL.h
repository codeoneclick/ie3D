//
//  CShaderCompiler_GLSL.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 8/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CShaderCompiler_GLSL_h
#define CShaderCompiler_GLSL_h

#include "HCommon.h"

class CShaderCompiler_GLSL
{
private:
    
protected:
    
public:
    
    CShaderCompiler_GLSL(void) = default;
    ~CShaderCompiler_GLSL(void) = default;
    
    static ui32 compile(const std::string& sourceCode, GLenum shaderType, std::string* outMessage, bool* outSuccess);
    static ui32 link(ui32 vsHandle, ui32 fsHandle, std::string* outMessage, bool* outSuccess);
};

#endif
