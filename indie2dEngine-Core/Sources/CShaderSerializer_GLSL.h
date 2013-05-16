//
//  CShaderSerializer_GLSL.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/16/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CShaderSerializer_GLSL_h
#define CShaderSerializer_GLSL_h

#include "IResourceSerializer.h"

class CShaderSerializer_GLSL final : public IResourceSerializer
{
private:
    
protected:
    
    std::string m_vsFilename;
    std::string m_fsFilename;
    
    std::string m_vsSourceCode;
    std::string m_fsSourceCode;
    
public:
    
    CShaderSerializer_GLSL(const std::string& _vsFilename, const std::string& _fsFilename);
    ~CShaderSerializer_GLSL(void);

    void Serialize(void);

    inline std::string Get_VertexShaderSourceCode(void)
    {
        assert(m_status == E_SERIALIZER_STATUS_SUCCESS);
        return m_vsSourceCode;
    };
    
    inline std::string Get_FragmentShaderSourceCode(void)
    {
        assert(m_status == E_SERIALIZER_STATUS_SUCCESS);
        return m_fsSourceCode;
    };
};

#endif 
