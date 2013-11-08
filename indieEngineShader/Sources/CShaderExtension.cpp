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

std::string CShaderExtension::Get_VertexShaderSourceCode(void)
{
    assert(m_shader != nullptr);
    return m_shader->_Get_VertexShaderSourceCode();
}

std::string CShaderExtension::Get_FragmentShaderSourceCode(void)
{
    assert(m_shader != nullptr);
    return m_shader->_Get_FragmentShaderSourceCode();
}

bool CShaderExtension::Compile(const std::string& _vsSourceCode, const std::string& _fsSourceCode)
{
    return false;
}