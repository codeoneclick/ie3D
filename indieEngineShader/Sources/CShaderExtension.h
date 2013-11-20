//
//  CShaderExtension.h
//  indieEngineShader
//
//  Created by Sergey Sergeev on 11/6/13.
//
//

#ifndef CShaderExtension_h
#define CShaderExtension_h

#include "CShader.h"

class CShaderExtension : public CShader
{
public:
    typedef std::function<void(const std::string& message)> SHADER_COMPILE_MESSAGE_HANDLER;
private:
    
protected:
    
    std::shared_ptr<CShader> m_shader;
    
public:
    
    CShaderExtension(const std::shared_ptr<CShader>& _shader);
    ~CShaderExtension(void);
    
    std::string Get_VertexShaderSourceCode(void);
    std::string Get_FragmentShaderSourceCode(void);
    bool Compile(const std::string& _vsSourceCode, const std::string& _fsSourceCode, const SHADER_COMPILE_MESSAGE_HANDLER& _handler);

};

#endif
