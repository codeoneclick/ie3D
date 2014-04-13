//
//  CShaderCommiter_GLSL.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CShaderCommiter_GLSL_h
#define CShaderCommiter_GLSL_h

#include "IResourceCommiter.h"

class CShaderCommiter_GLSL final : public IResourceCommiter
{
private:
    
protected:
    
    std::string m_vsSourceCode;
    std::string m_fsSourceCode;
    std::string m_message;
    
    ui32 compile(const std::string& sourceCode, GLenum shaderType);
    ui32 link(ui32 vsHandle, ui32 fsHandle);
    
public:
    
    CShaderCommiter_GLSL(const std::string& guid,
                         const std::string& vsSourceCode,
                         const std::string& fsSourceCode,
                         ISharedResourceRef resource);
    ~CShaderCommiter_GLSL(void);
    
    std::string getStatusMessage(void);
    
    void commit(void);
};

#endif 
