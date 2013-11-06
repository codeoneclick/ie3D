//
//  CShaderExtension.cpp
//  indieEngineShader
//
//  Created by Sergey Sergeev on 11/6/13.
//
//

#include "CShaderExtension.h"

CShaderExtension::CShaderExtension(const std::shared_ptr<CShader>& _shader) :
CShader("shader.extension"),
m_shader(_shader)
{
    
}

CShaderExtension::~CShaderExtension(void)
{
    
}