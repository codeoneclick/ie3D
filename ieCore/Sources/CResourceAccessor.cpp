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

AAssetManager* CResourceAccessor::m_assetManager = nullptr;

#endif

CResourceAccessor::CResourceAccessor(void) :
m_textureLoader(std::make_shared<CTextureLoader>()),
m_shaderLoader(std::make_shared<CShaderLoader>()),
m_meshLoader(std::make_shared<CMeshLoader>()),
m_skeletonLoader(std::make_shared<CSkeletonLoader>()),
m_sequenceLoader(std::make_shared<CSequenceLoader>())
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

std::shared_ptr<CSkeleton> CResourceAccessor::CreateSkeleton(const std::string &_filename)
{
    assert(m_skeletonLoader != nullptr);
    std::shared_ptr<CSkeleton> skeleton = m_skeletonLoader->StartLoadOperation(_filename);
    assert(skeleton != nullptr);
    return skeleton;
}

std::shared_ptr<CSequence> CResourceAccessor::CreateSequence(const std::string &_filename)
{
    assert(m_sequenceLoader != nullptr);
    std::shared_ptr<CSequence> sequence = m_sequenceLoader->StartLoadOperation(_filename);
    assert(sequence != nullptr);
    return sequence;
}
