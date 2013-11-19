//
//  CMaterialExtension.h
//  indieEngineShader
//
//  Created by Sergey Sergeev on 11/19/13.
//
//

#ifndef CMaterialExtension_h
#define CMaterialExtension_h

#include "CMAterial.h"

class CShaderExtension;

class CMaterialExtension : public CMaterial
{
private:
    
protected:
    
    std::shared_ptr<CMaterial> m_material;
    std::shared_ptr<CShaderExtension> m_shaderExtension;
    
public:
    
    CMaterialExtension(const std::shared_ptr<CMaterial>& _material);
    ~CMaterialExtension(void);
    
    inline std::shared_ptr<CShaderExtension> Get_ShaderExtension(void)
    {
        return m_shaderExtension;
    };
};

#endif
