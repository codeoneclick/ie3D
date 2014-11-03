//
//  CMEModelsSceneTransition.cpp
//  ie3D-EMap
//
//  Created by sergey.sergeev on 11/3/14.
//
//

#include "CMEModelsSceneTransition.h"
#include "CMEModelsScene.h"
#include "CMESceneFabricator.h"
#include "CSceneGraph.h"
#include "CRenderPipeline.h"

CMEModelsSceneTransition::CMEModelsSceneTransition(const std::string& filename) :
IEGameTransition(filename)
{
    
}

CMEModelsSceneTransition::~CMEModelsSceneTransition(void)
{
    
}

void CMEModelsSceneTransition::initScene(void)
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

void CMEModelsSceneTransition::_OnLoaded(void)
{
    m_scene = std::make_shared<CMEModelsScene>(this);
    m_scene->load();
    m_isLoaded = true;
}

void CMEModelsSceneTransition::_OnGameLoopUpdate(f32 deltatime)
{
    if(m_isLoaded && m_scene != nullptr)
    {
        m_scene->update(deltatime);
    }
}

CMESharedLandscapeBrush CMEModelsSceneTransition::createLandscapeBrush(const std::string& filename)
{
    assert(m_sceneFabricator != nullptr);
    std::shared_ptr<CMESceneFabricator> sceneFabricator = std::static_pointer_cast<CMESceneFabricator>(m_sceneFabricator);
    return sceneFabricator->createLandscapeBrush(filename);
}

CMESharedModelBrush CMEModelsSceneTransition::createModelBrush(const std::string& filename)
{
    assert(m_sceneFabricator != nullptr);
    std::shared_ptr<CMESceneFabricator> sceneFabricator = std::static_pointer_cast<CMESceneFabricator>(m_sceneFabricator);
    return sceneFabricator->createModelBrush(filename);
}
