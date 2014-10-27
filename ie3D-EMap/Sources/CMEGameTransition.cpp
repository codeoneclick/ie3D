//
//  CMEGameTransition.cpp
//  indieEngine-EMap
//
//  Created by Sergey Sergeev on 4/20/14.
//
//

#include "CMEGameTransition.h"
#include "CMEScene.h"
#include "CMESceneFabricator.h"
#include "CSceneGraph.h"
#include "CRenderPipeline.h"

CMEGameTransition::CMEGameTransition(const std::string& filename,
                                     std::shared_ptr<IGraphicsContext> graphicsContext,
                                     std::shared_ptr<IInputContext> gestureRecognizerContext,
                                     std::shared_ptr<CResourceAccessor> resourceAccessor,
                                     std::shared_ptr<CConfigurationAccessor> configurationAccessor) :
IEGameTransition(filename,
                 graphicsContext,
                 gestureRecognizerContext,
                 resourceAccessor,
                 configurationAccessor)
{
    
}

CMEGameTransition::~CMEGameTransition(void)
{
    
}

void CMEGameTransition::initScene(void)
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

void CMEGameTransition::_OnLoaded(void)
{
    m_scene = std::make_shared<CMEScene>(this);
    m_scene->load();
    m_isLoaded = true;
}

void CMEGameTransition::_OnGameLoopUpdate(f32 deltatime)
{
    if(m_isLoaded && m_scene != nullptr)
    {
        m_scene->update(deltatime);
    }
}

CSharedMEUIToSceneCommands CMEGameTransition::getUIToSceneCommands(void) const
{
    return m_isLoaded && m_scene != nullptr ? std::static_pointer_cast<CMEScene>(m_scene)->getUIToSceneCommands() : nullptr;
}

void CMEGameTransition::setSceneToUICommands(CSharedMESceneToUICommandsRef commands)
{
    assert(m_isLoaded && m_scene != nullptr);
    std::static_pointer_cast<CMEScene>(m_scene)->setSceneToUICommands(commands);
}

CMESharedLandscapeBrush CMEGameTransition::createLandscapeBrush(const std::string& filename)
{
    assert(m_sceneFabricator != nullptr);
    std::shared_ptr<CMESceneFabricator> sceneFabricator = std::static_pointer_cast<CMESceneFabricator>(m_sceneFabricator);
    return sceneFabricator->createLandscapeBrush(filename);
}

CMESharedPlacementModel CMEGameTransition::createPlacementModel(const std::string& filename)
{
    assert(m_sceneFabricator != nullptr);
    std::shared_ptr<CMESceneFabricator> sceneFabricator = std::static_pointer_cast<CMESceneFabricator>(m_sceneFabricator);
    return sceneFabricator->createPlacementModel(filename);
}
