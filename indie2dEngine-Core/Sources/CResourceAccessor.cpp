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
#include "CSkeletonLoader.h"
#include "CSequenceLoader.h"

#if defined(__NDK__)

#endif

CResourceAccessor::CResourceAccessor(void)
{
    m_resourceLoaders[E_RESOURCE_LOADER_TEXTURE] = std::make_shared<CTextureLoader>();
    m_resourceLoaders[E_RESOURCE_LOADER_SHADER] = std::make_shared<CShaderLoader>();
    m_resourceLoaders[E_RESOURCE_LOADER_MESH] = std::make_shared<CMeshLoader>();
}

CResourceAccessor::~CResourceAccessor(void)
{
    
}

CSharedTexture CResourceAccessor::getTexture(const std::string &filename)
{
    assert(m_resourceLoaders[E_RESOURCE_LOADER_TEXTURE] != nullptr);
    std::shared_ptr<CTexture> texture = std::static_pointer_cast<CTextureLoader>(m_resourceLoaders[E_RESOURCE_LOADER_TEXTURE])->StartLoadOperation(filename);
    assert(texture != nullptr);
    return texture;
}

CSharedShader CResourceAccessor::getShader(const std::string &vsFilename, const std::string &fsFilename)
{
    assert(m_resourceLoaders[E_RESOURCE_LOADER_SHADER] != nullptr);
    std::shared_ptr<CShader> shader = std::static_pointer_cast<CShaderLoader>(m_resourceLoaders[E_RESOURCE_LOADER_SHADER])->StartLoadOperation(vsFilename, fsFilename);
    assert(shader != nullptr);
    return shader;
}

CSharedMesh CResourceAccessor::getMesh(const std::string &filename)
{
    assert(m_resourceLoaders[E_RESOURCE_LOADER_MESH] != nullptr);
    std::shared_ptr<CMesh> mesh = std::static_pointer_cast<CMeshLoader>(m_resourceLoaders[E_RESOURCE_LOADER_MESH])->StartLoadOperation(filename);
    assert(mesh != nullptr);
    return mesh;
}