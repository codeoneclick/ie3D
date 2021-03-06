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
    
public:
    
    CShaderSerializer_GLSL(const std::string& vsFilename,
                           const std::string& fsFilename,
                           ISharedResourceRef resource);
    ~CShaderSerializer_GLSL(void);

    void serialize(void);
};

#endif 
