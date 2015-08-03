//
//  CMEpoeTransition.cpp
//  ie3D-EMap-OSX
//
//  Created by sergey.sergeev on 7/28/15.
//
//

#include "CMEpoeTransition.h"
#include "CMEpoeScene.h"
#include "CMESceneFabricator.h"
#include "CSceneGraph.h"
#include "CRenderPipeline.h"

CMEpoeTransition::CMEpoeTransition(const std::string& filename, bool isOffscreen) :
IEGameTransition(filename, isOffscreen)
{
    
}

CMEpoeTransition::~CMEpoeTransition(void)
{
    
}

void CMEpoeTransition::initScene(void)
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

void CMEpoeTransition::_OnLoaded(void)
{
    m_scene = std::make_shared<CMEpoeScene>(this);
    IGameTransition::_OnLoaded();
    m_scene->load();
    m_isLoaded = true;
}

void CMEpoeTransition::_OnGameLoopUpdate(f32 deltatime)
{
    if(m_isLoaded && m_scene != nullptr)
    {
        m_scene->update(deltatime);
    }
}

CMESharedSceneStage CMEpoeTransition::createSceneStage(const std::string& filename)
{
    assert(m_sceneFabricator != nullptr);
    std::shared_ptr<CMESceneFabricator> sceneFabricator = std::static_pointer_cast<CMESceneFabricator>(m_sceneFabricator);
    return sceneFabricator->createSceneStage(filename);
}
