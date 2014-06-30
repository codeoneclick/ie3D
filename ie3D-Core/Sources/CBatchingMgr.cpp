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
#include "CQuad.h"
#include "CAnimationMixer.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CRenderMgr.h"

const ui32 CBatchingMgr::k_MAX_BATCHES_PER_MODELTYPE = 8;

CBatchingMgr::CBatchingMgr(CSharedRenderMgrRef renderMgr) :
m_renderMgr(renderMgr)
{
    
}

CBatchingMgr::~CBatchingMgr(void)
{
    m_batches.clear();
}

void CBatchingMgr::lock(const std::string& mode)
{
    for(const auto& iterator : m_batches)
    {
        if(iterator.second->getMode() == mode)
        {
            iterator.second->lock();
        }
    }
}

void CBatchingMgr::unlock(const std::string& mode)
{
    for(const auto& iterator : m_batches)
    {
        if(iterator.second->getMode() == mode)
        {
            iterator.second->unlock();
        }
    }
}

void CBatchingMgr::batch(const std::string& mode,
                         ui32 renderQueuePosition,
                         const std::tuple<CSharedMesh, CSharedAnimationMixer>& model,
                         CSharedMaterialRef material,
                         const std::function<void(CSharedMaterialRef)>& materialBindImposer,
                         const glm::mat4x4& matrix)
{
    assert(material != nullptr);
    assert(material->getShader() != nullptr);
	//assert(std::get<1>(model)->Get_TransformationSize() <= CBatch::k_MAX_NUM_TRANSFORMATION);
    assert(std::get<0>(model)->getNumVertices() <= CBatch::k_MAX_NUM_VERTICES);
    assert(std::get<0>(model)->getNumIndices() <= CBatch::k_MAX_NUM_INDICES);
    
    for(ui32 i = 0; i < k_MAX_BATCHES_PER_MODELTYPE; ++i)
    {
    	std::ostringstream stream;
    	stream<<i;
    	std::string guid = material->getShader()->getGuid() + ".batch_" + stream.str();
        auto iterator = m_batches.find(guid);
        
        if(iterator == m_batches.end())
        {
			m_batches.insert(std::make_pair(guid, std::make_shared<CBatch>(mode,
                                                                           renderQueuePosition,
                                                                           material,
                                                                           materialBindImposer)));
            iterator = m_batches.find(guid);
            m_renderMgr->RegisterWorldSpaceRenderHandler(mode, iterator->second);
			iterator->second->batch(model, matrix);
            break;
        }
        /*else if((iterator->second->getNumUnlockedNumTransformations() + std::get<1>(model)->Get_TransformationSize()) > CBatch::k_MAX_NUM_TRANSFORMATION ||
                (iterator->second->getNumUnlockedNumVertices() + std::get<0>(model)->getNumVertices()) > CBatch::k_MAX_NUM_VERTICES ||
                (iterator->second->getNumUnlockedNumIndices() + std::get<0>(model)->getNumIndices()) > CBatch::k_MAX_NUM_INDICES)*/
        //{
        //    continue;
        //}
        else
        {
			iterator->second->batch(model, matrix);
            break;
        }
        assert(false);
    }
}
