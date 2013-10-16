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
#include "CAnimationMixer.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CRenderMgr.h"

const ui32 CBatchingMgr::k_MAX_BATCHES_PER_MESH_TYPE = 8;

CBatchingMgr::CBatchingMgr(const std::shared_ptr<CRenderMgr>& _renderMgr) :
m_renderMgr(_renderMgr)
{
    
}

CBatchingMgr::~CBatchingMgr(void)
{
    m_batches.clear();
}

void CBatchingMgr::Lock(const std::string& _mode)
{
    for(const auto& iterator : m_batches)
    {
        if(iterator.second->Get_Mode() == _mode)
        {
            iterator.second->Lock();
        }
    }
}

void CBatchingMgr::Unlock(const std::string& _mode)
{
    for(const auto& iterator : m_batches)
    {
        if(iterator.second->Get_Mode() == _mode)
        {
            iterator.second->Unlock();
        }
    }
}

void CBatchingMgr::Batch(const std::string& _mode, ui32 _renderQueuePosition, const std::tuple<std::shared_ptr<CMesh>, std::shared_ptr<CAnimationMixer>>& _mesh, std::shared_ptr<CMaterial> _material, const std::function<void(std::shared_ptr<CMaterial>)>& _bind, const glm::mat4x4& _matrix)
{
    assert(_material != nullptr);
    assert(_material->Get_Shader() != nullptr);
    assert(std::get<1>(_mesh)->Get_TransformationSize() <= CBatch::k_MAX_NUM_TRANSFORMATION);
    assert(std::get<0>(_mesh)->Get_NumVertexes() <= CBatch::k_MAX_NUM_VERTICES);
    assert(std::get<0>(_mesh)->Get_NumIndexes() <= CBatch::k_MAX_NUM_INDICES);
    
    for(ui32 i = 0; i < k_MAX_BATCHES_PER_MESH_TYPE; ++i)
    {
        std::string guid = _material->Get_Guid() + ".batch_" + std::to_string(i);
        auto iterator = m_batches.find(guid);
        
        if(iterator == m_batches.end())
        {
            m_batches.insert(std::make_pair(guid, std::make_shared<CBatch>(_mode, _renderQueuePosition, _material, _bind)));
            iterator = m_batches.find(guid);
            m_renderMgr->RegisterWorldSpaceRenderHandler(_mode, iterator->second);
            iterator->second->Batch(_mesh, _matrix);
            break;
        }
        else if((iterator->second->Get_NumBatchedTransformations() + std::get<1>(_mesh)->Get_TransformationSize()) > CBatch::k_MAX_NUM_TRANSFORMATION ||
                (iterator->second->Get_NumBatchedVertices() + std::get<0>(_mesh)->Get_NumVertexes()) > CBatch::k_MAX_NUM_VERTICES ||
                (iterator->second->Get_NumBatchedIndices() + std::get<0>(_mesh)->Get_NumIndexes()) > CBatch::k_MAX_NUM_INDICES)
        {
            continue;
        }
        else
        {
            iterator->second->Batch(_mesh, _matrix);
            break;
        }
        assert(false);
    }
}