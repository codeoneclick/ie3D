//
//  CMEMainSceneTransition.cpp
//  indieEngine-EMap
//
//  Created by Sergey Sergeev on 4/20/14.
//
//

#include "CMEMainSceneTransition.h"
#include "CMEMainScene.h"
#include "CMESceneFabricator.h"
#include "CSceneGraph.h"
#include "CRenderPipeline.h"

CMEMainSceneTransition::CMEMainSceneTransition(const std::string& filename) :
IEGameTransition(filename)
{
    
}

CMEMainSceneTransition::~CMEMainSceneTransition(void)
{
    
}

void CMEMainSceneTransition::initScene(void)
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

void CMEMainSceneTransition::_OnLoaded(void)
{
    m_scene = std::make_shared<CMEMainScene>(this);
    m_scene->load();
    m_isLoaded = true;
}

void CMEMainSceneTransition::_OnGameLoopUpdate(f32 deltatime)
{
    if(m_isLoaded && m_scene != nullptr)
    {
        m_scene->update(deltatime);
    }
}

CSharedMEUIToSceneCommands CMEMainSceneTransition::getUIToSceneCommands(void) const
{
    return m_isLoaded && m_scene != nullptr ? std::static_pointer_cast<CMEMainScene>(m_scene)->getUIToSceneCommands() : nullptr;
}

void CMEMainSceneTransition::setSceneToUICommands(CSharedMESceneToUICommandsRef commands)
{
    assert(m_isLoaded && m_scene != nullptr);
    std::static_pointer_cast<CMEMainScene>(m_scene)->setSceneToUICommands(commands);
}

CMESharedLandscapeBrush CMEMainSceneTransition::createLandscapeBrush(const std::string& filename)
{
    assert(m_sceneFabricator != nullptr);
    std::shared_ptr<CMESceneFabricator> sceneFabricator = std::static_pointer_cast<CMESceneFabricator>(m_sceneFabricator);
    return sceneFabricator->createLandscapeBrush(filename);
}

CMESharedModelBrush CMEMainSceneTransition::createModelBrush(const std::string& filename)
{
    assert(m_sceneFabricator != nullptr);
    std::shared_ptr<CMESceneFabricator> sceneFabricator = std::static_pointer_cast<CMESceneFabricator>(m_sceneFabricator);
    return sceneFabricator->createModelBrush(filename);
}
