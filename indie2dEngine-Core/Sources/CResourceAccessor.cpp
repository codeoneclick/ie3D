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

CSharedTexture CResourceAccessor::getTexture(const std::string &filename)
{
    assert(m_resourceLoader != nullptr);
    CSharedTexture texture = m_resourceLoader->startTextureLoadingOperation(filename);
    assert(texture != nullptr);
    return texture;
}

CSharedShader CResourceAccessor::getShader(const std::string &vsFilename, const std::string &fsFilename)
{
    assert(m_resourceLoader != nullptr);
    CSharedShader shader = m_resourceLoader->startShaderLoadingOperation(vsFilename, fsFilename);
    assert(shader != nullptr);
    return shader;
}

CSharedMesh CResourceAccessor::getMesh(const std::string& meshFilename,
                                       const std::string& skeletonFilename,
                                       const std::string& sequenceFilename)
{
    assert(m_resourceLoader != nullptr);
    CSharedMesh mesh = m_resourceLoader->startMeshLoadingOperation(meshFilename,
                                                                   skeletonFilename,
                                                                   sequenceFilename);
    assert(mesh != nullptr);
    return mesh;
}