//
//  CBatchingMgr.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/22/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CBatchingMgr.h"
#include "CBatch.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CShader.h"

CBatchingMgr::CBatchingMgr(void)
{
    
}

CBatchingMgr::~CBatchingMgr(void)
{
    m_batches.clear();
}

void CBatchingMgr::Lock(void)
{
    for(const auto& iterator : m_batches)
    {
        iterator.second->Lock();
    }
}

void CBatchingMgr::Unlock(void)
{
    for(const auto& iterator : m_batches)
    {
        iterator.second->Unlock();
    }
}

void CBatchingMgr::Batch(const std::tuple<std::shared_ptr<CMesh>, std::shared_ptr<CAnimationMixer>>& _mesh, std::shared_ptr<CMaterial> _material, const glm::mat4x4& _matrix)
{
    assert(_material != nullptr);
    assert(_material->Get_Shader() != nullptr);
    std::string guid = _material->Get_Shader()->Get_Guid() + ".batch";
    auto iterator = m_batches.find(guid);
    if(iterator == m_batches.end())
    {
        m_batches.insert(std::make_pair(guid, std::make_shared<CBatch>(_material)));
        iterator = m_batches.find(guid);
    }
    iterator->second->Batch(_mesh, _matrix);
}

void CBatchingMgr::Draw(void)
{
    for(const auto& iterator : m_batches)
    {
        iterator.second->Draw();
    }
}
