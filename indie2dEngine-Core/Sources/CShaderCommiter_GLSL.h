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
    
    ui32 _Compile(const std::string& _sourceCode, GLenum _shader);
    ui32 _Link(ui32 _vsHandle, ui32 _fsHandle);
    
public:
    
    CShaderCommiter_GLSL(const std::string& _guid, const std::string& _vsSourceCode, const std::string& _fsSourceCode, std::shared_ptr<IResource> _resource);
    ~CShaderCommiter_GLSL(void);
    
    void Commit(void);
    
    inline std::string Get_StatusMessage(void)
    {
        return m_message;
    };
};

#endif 
