//
//  CMEgoeTransition.cpp
//  ie3D-EMap
//
//  Created by sergey.sergeev on 2/6/15.
//
//

#include "CMEgoeTransition.h"
#include "CMEgoeScene.h"
#include "CMESceneFabricator.h"
#include "CSceneGraph.h"
#include "CRenderPipeline.h"

CMEgoeTransition::CMEgoeTransition(const std::string& filename, bool isOffscreen) :
IEGameTransition(filename, isOffscreen)
{
    
}

CMEgoeTransition::~CMEgoeTransition(void)
{
    
}

void CMEgoeTransition::initScene(void)
{
    assert(m_graphicsContext != nullptr);
    assert(m_inputContext != nullptr);
    assert(m_sceneUpdateMgr != nullptr);
    assert(m_collisionMgr != nullptr);
    assert(m_renderPipeline != nullptr);
    
    m_sceneGraph = std::make_shared<CSceneGraph>(m_renderPipeline, m_sceneUpdateMgr,
                                                 m_collisionMgr, m_inputContext);
    
    m_sceneFabricator = std::make_shared<CMESceneFabricator>(m_configurationAccessor,
                                                             m_resourceAccessor,
                                                             m_renderPipeline);
}

void CMEgoeTransition::_OnLoaded(void)
{
    m_scene = std::make_shared<CMEgoeScene>(this);
    IGameTransition::_OnLoaded();
    m_scene->load();
    m_isLoaded = true;
}

void CMEgoeTransition::_OnGameLoopUpdate(f32 deltatime)
{
    if(m_isLoaded && m_scene != nullptr)
    {
        m_scene->update(deltatime);
    }
}

CMESharedSceneStage CMEgoeTransition::createSceneStage(const std::string& filename)
{
    assert(m_sceneFabricator != nullptr);
    std::shared_ptr<CMESceneFabricator> sceneFabricator = std::static_pointer_cast<CMESceneFabricator>(m_sceneFabricator);
    return sceneFabricator->createSceneStage(filename);
}
