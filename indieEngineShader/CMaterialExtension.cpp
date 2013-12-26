//
//  CMaterialExtension.cpp
//  indieEngineShader
//
//  Created by Sergey Sergeev on 11/19/13.
//
//

#include "CMaterialExtension.h"
#include "CShaderExtension.h"

CMaterialExtension::CMaterialExtension(const std::shared_ptr<CMaterial>& _material) :
CMaterial(_material->Get_Guid()),
m_material(_material)
{
    m_shaderExtension = std::make_shared<CShaderExtension>(_material->Get_Shader());
}

CMaterialExtension::~CMaterialExtension(void)
{
    
}