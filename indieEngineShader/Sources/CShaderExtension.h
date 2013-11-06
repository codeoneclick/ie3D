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
private:
    
protected:
    
    std::shared_ptr<CShader> m_shader;
    
public:
    
    CShaderExtension(const std::shared_ptr<CShader>& _shader);
    ~CShaderExtension(void);
};

#endif
