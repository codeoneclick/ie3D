//
//  CResourceAccessor.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/23/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CResourceAccessor.h"
#include "CResourceLoader.h"

#if defined(__NDK__)

#endif

CResourceAccessor::CResourceAccessor(void)
{
    m_resourceLoader = std::make_shared<CResourceLoader>();
}

CResourceAccessor::~CResourceAccessor(void)
{
    
}

CSharedResourceLoader CResourceAccessor::getResourceLoader(void) const
{
    assert(m_resourceLoader != nullptr);
    return m_resourceLoader;
}

void CResourceAccessor::addCustomTexture(const std::string& textureName, CSharedTextureRef texture)
{
    const auto& iterator = m_customTextures.find(textureName);
    if(iterator != m_customTextures.end())
    {
        m_customTextures[textureName] = texture;
    }
    else
    {
        m_customTextures.insert(std::make_pair(textureName, texture));
    }
}

CSharedTexture CResourceAccessor::getCustomTexture(const std::string& guid, bool sync) const
{
    const auto& iterator = m_customTextures.find(guid);
    if(iterator != m_customTextures.end())
    {
        return iterator->second;
    }
    return nullptr;
}

CSharedTexture CResourceAccessor::getTexture(const std::string &filename, bool sync) const
{
    const auto& iterator = m_customTextures.find(filename);
    if(iterator != m_customTextures.end())
    {
        return iterator->second;
    }
    assert(m_resourceLoader != nullptr);
    CSharedTexture texture = m_resourceLoader->startTextureLoadingOperation(filename, sync);
    assert(texture != nullptr);
    return texture;
}

CSharedShader CResourceAccessor::getShader(const std::string &vsFilename, const std::string &fsFilename) const
{
    assert(m_resourceLoader != nullptr);
    CSharedShader shader = m_resourceLoader->startShaderLoadingOperation(vsFilename, fsFilename);
    assert(shader != nullptr);
    return shader;
}

CSharedMesh CResourceAccessor::getMesh(const std::string& filename) const
{
    assert(m_resourceLoader != nullptr);
    CSharedMesh mesh = m_resourceLoader->startMeshLoadingOperation(filename);
    assert(mesh != nullptr);
    return mesh;
}

CSharedAnimationSequence CResourceAccessor::getAnimationSequence(const std::string& filename) const
{
    assert(m_resourceLoader != nullptr);
    CSharedAnimationSequence animationSequence = m_resourceLoader->startAnimationSequenceLoadingOperation(filename);
    assert(animationSequence != nullptr);
    return animationSequence;
}

void CResourceAccessor::removeLoadingDependecies(ISharedResourceLoadingHandler handler)
{
    assert(m_resourceLoader);
    m_resourceLoader->removeLoadingDependecies(handler);
}
