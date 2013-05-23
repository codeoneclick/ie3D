//
//  CResourceFabricator.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/23/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CResourceFabricator.h"
#include "CTextureLoader.h"
#include "CShaderLoader.h"

CResourceFabricator::CResourceFabricator(void) :
m_textureLoader(std::make_shared<CTextureLoader>()),
m_shaderLoader(std::make_shared<CShaderLoader>())
{
    
}

CResourceFabricator::~CResourceFabricator(void)
{
    
}

std::shared_ptr<CTexture> CResourceFabricator::CreateTexture(const std::string &_filename)
{
    assert(m_textureLoader != nullptr);
    std::shared_ptr<CTexture> texture = m_textureLoader->StartLoadOperation(_filename);
    assert(texture != nullptr);
    return texture;
}

std::shared_ptr<CShader> CResourceFabricator::CreateShader(const std::string &_vsFilename, const std::string &_fsFilename)
{
    assert(m_shaderLoader);
    std::shared_ptr<CShader> shader = m_shaderLoader->StartLoadOperation(_vsFilename, _fsFilename);
    assert(shader != nullptr);
    return shader;
}