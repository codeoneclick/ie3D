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
#include "IRenderTechniqueImporter.h"

const ui32 CBatchingMgr::k_MAX_BATCHES_PER_MODELTYPE = 8;

CBatchingMgr::CBatchingMgr(ISharedRenderTechniqueImporterRef renderTechniqueImporter) :
m_renderTechniqueImporter(renderTechniqueImporter)
{
    
}

CBatchingMgr::~CBatchingMgr(void)
{
    m_batches.clear();
}

void CBatchingMgr::lock(const std::string& techniqueName)
{
    for(const auto& iterator : m_batches)
    {
        if(iterator.second->getTechniqueName() == techniqueName)
        {
            iterator.second->lock();
        }
    }
}

void CBatchingMgr::unlock(const std::string& techniqueName)
{
    for(const auto& iterator : m_batches)
    {
        if(iterator.second->getTechniqueName() == techniqueName)
        {
            iterator.second->unlock();
        }
    }
}

void CBatchingMgr::batch(const std::string& techniqueName,
                         ui32 renderQueuePosition,
                         CSharedMeshRef mesh,
                         CSharedMaterialRef material,
                         const std::function<void(CSharedMaterialRef)>& materialBindImposer,
                         const glm::mat4x4& matrix)
{
    assert(material != nullptr);
    assert(material->getShader() != nullptr);
    assert(mesh->getNumVertices() <= CBatch::k_MAX_NUM_VERTICES);
    assert(mesh->getNumIndices() <= CBatch::k_MAX_NUM_INDICES);
    
    for(ui32 i = 0; i < k_MAX_BATCHES_PER_MODELTYPE; ++i)
    {
        std::ostringstream stream;
        stream<<i;
        std::string guid = material->getShader()->getGuid() + ".batch_" + stream.str();
        auto iterator = m_batches.find(guid);
        
        if(iterator == m_batches.end())
        {
            m_batches.insert(std::make_pair(guid, std::make_shared<CBatch>(techniqueName,
                                                                           renderQueuePosition,
                                                                           material,
                                                                           materialBindImposer)));
            iterator = m_batches.find(guid);
            m_renderTechniqueImporter->addRenderTechniqueHandler(techniqueName, iterator->second);
            iterator->second->batch(mesh, matrix);
            break;
        }
        else if((iterator->second->getNumUnlockedVertices() + mesh->getNumVertices() > CBatch::k_MAX_NUM_VERTICES ||
                (iterator->second->getNumUnlockedIndices() + mesh->getNumIndices()) > CBatch::k_MAX_NUM_INDICES))
        {
            continue;
        }
        else
        {
            iterator->second->batch(mesh, matrix);
            break;
        }
        assert(false);
    }
}

void CBatchingMgr::batch(const std::string& techniqueName,
                         ui32 renderQueuePosition,
                         CSharedMeshRef mesh,
                         CSharedAnimationMixerRef animationMixer,
                         CSharedMaterialRef material,
                         const std::function<void(CSharedMaterialRef)>& materialBindImposer,
                         const glm::mat4x4& matrix)
{
    assert(material != nullptr);
    assert(material->getShader() != nullptr);
    assert(mesh->getNumVertices() <= CBatch::k_MAX_NUM_VERTICES);
    assert(mesh->getNumIndices() <= CBatch::k_MAX_NUM_INDICES);
    
    for(ui32 i = 0; i < k_MAX_BATCHES_PER_MODELTYPE; ++i)
    {
        std::ostringstream stream;
        stream<<i;
        std::string guid = material->getShader()->getGuid() + ".batch_" + stream.str();
        auto iterator = m_batches.find(guid);
        
        if(iterator == m_batches.end())
        {
            m_batches.insert(std::make_pair(guid, std::make_shared<CBatch>(techniqueName,
                                                                           renderQueuePosition,
                                                                           material,
                                                                           materialBindImposer)));
            iterator = m_batches.find(guid);
            m_renderTechniqueImporter->addRenderTechniqueHandler(techniqueName, iterator->second);
            iterator->second->batch(mesh, animationMixer, matrix);
            break;
        }
        else if((iterator->second->getNumUnlockedTransformations() + animationMixer->getTransformationSize() > CBatch::k_MAX_NUM_TRANSFORMATION) ||
                (iterator->second->getNumUnlockedVertices() + mesh->getNumVertices() > CBatch::k_MAX_NUM_VERTICES ||
                (iterator->second->getNumUnlockedIndices() + mesh->getNumIndices()) > CBatch::k_MAX_NUM_INDICES))
        {
            continue;
        }
        else
        {
            iterator->second->batch(mesh, animationMixer, matrix);
            break;
        }
        assert(false);
    }
}