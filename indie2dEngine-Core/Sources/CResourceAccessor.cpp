//
//  CResourceAccessor.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/23/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CResourceAccessor.h"
#include "CTextureLoader.h"
#include "CShaderLoader.h"
#include "CMeshLoader.h"

CResourceAccessor::CResourceAccessor(void) :
m_textureLoader(std::make_shared<CTextureLoader>()),
m_shaderLoader(std::make_shared<CShaderLoader>()),
m_meshLoader(std::make_shared<CMeshLoader>())
{
    
}

CResourceAccessor::~CResourceAccessor(void)
{
    
}

std::shared_ptr<CTexture> CResourceAccessor::CreateTexture(const std::string &_filename)
{
    assert(m_textureLoader != nullptr);
    std::shared_ptr<CTexture> texture = m_textureLoader->StartLoadOperation(_filename);
    assert(texture != nullptr);
    return texture;
}

std::shared_ptr<CShader> CResourceAccessor::CreateShader(const std::string &_vsFilename, const std::string &_fsFilename)
{
    assert(m_shaderLoader != nullptr);
    std::shared_ptr<CShader> shader = m_shaderLoader->StartLoadOperation(_vsFilename, _fsFilename);
    assert(shader != nullptr);
    return shader;
}

std::shared_ptr<CMesh> CResourceAccessor::CreateMesh(const std::string &_filename)
{
    assert(m_meshLoader != nullptr);
    std::shared_ptr<CMesh> mesh = m_meshLoader->StartLoadOperation(_filename);
    assert(mesh != nullptr);
    return mesh;
}