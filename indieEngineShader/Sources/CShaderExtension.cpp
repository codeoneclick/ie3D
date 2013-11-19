//
//  CShaderExtension.cpp
//  indieEngineShader
//
//  Created by Sergey Sergeev on 11/6/13.
//
//

#include "CShaderExtension.h"
#include "CShaderCommiter_GLSL.h"

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
    assert(m_shader != nullptr);
    std::shared_ptr<CShaderCommiter_GLSL> commiter = std::make_shared<CShaderCommiter_GLSL>(m_shader->Get_Guid(), _vsSourceCode, _fsSourceCode, m_shader);
    commiter->Commit();
    return true;
}