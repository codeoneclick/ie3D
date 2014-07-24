//
//  CMESceneTransition.cpp
//  indieEngine-EMap
//
//  Created by Sergey Sergeev on 4/20/14.
//
//

#include "CMESceneTransition.h"
#include "CMEScene.h"
#include "CMESceneFabricator.h"
#include "CSceneGraph.h"
#include "CRenderPipeline.h"

CMESceneTransition::CMESceneTransition(const std::string& filename,
                                                   std::shared_ptr<IGraphicsContext> graphicsContext,
                                                   std::shared_ptr<IInputContext> gestureRecognizerContext,
                                                   std::shared_ptr<CResourceAccessor> resourceAccessor,
                                                   std::shared_ptr<CConfigurationAccessor> configurationAccessor) :
IGameTransition(filename,
                graphicsContext,
                gestureRecognizerContext,
                resourceAccessor,
                configurationAccessor)
{
    
}

CMESceneTransition::~CMESceneTransition(void)
{
    
}

void CMESceneTransition::initScene(void)
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

void CMESceneTransition::_OnLoaded(void)
{
    m_scene = std::make_shared<CMEScene>(this);
    m_scene->load();
    m_isLoaded = true;
}

void CMESceneTransition::_OnGameLoopUpdate(f32 deltatime)
{
    if(m_isLoaded && m_scene != nullptr)
    {
        m_scene->update(deltatime);
    }
}

CSharedMEUIToSceneCommands CMESceneTransition::getUIToSceneCommands(void) const
{
    return m_isLoaded && m_scene != nullptr ? std::static_pointer_cast<CMEScene>(m_scene)->getUIToSceneCommands() : nullptr;
}

void CMESceneTransition::setSceneToUICommands(CSharedMESceneToUICommandsRef commands)
{
    assert(m_isLoaded && m_scene != nullptr);
    std::static_pointer_cast<CMEScene>(m_scene)->setSceneToUICommands(commands);
}

CSharedMELandscapeBrush CMESceneTransition::createLandscapeBrush(const std::string& filename)
{
    assert(m_sceneFabricator != nullptr);
    std::shared_ptr<CMESceneFabricator> sceneFabricator = std::static_pointer_cast<CMESceneFabricator>(m_sceneFabricator);
    return sceneFabricator->createLandscapeBrush(filename);
}

void CMESceneTransition::deleteLandscapeBrush(CSharedMELandscapeBrushRef gameObject)
{
    assert(m_sceneFabricator != nullptr);
    std::shared_ptr<CMESceneFabricator> sceneFabricator = std::static_pointer_cast<CMESceneFabricator>(m_sceneFabricator);
    sceneFabricator->deleteLandscapeBrush(gameObject);
}

CSharedMETankComplex CMESceneTransition::createTankComplex(const std::string& filename)
{
    assert(m_sceneFabricator != nullptr);
    std::shared_ptr<CMESceneFabricator> sceneFabricator = std::static_pointer_cast<CMESceneFabricator>(m_sceneFabricator);
    return sceneFabricator->createTankComplex(filename);
}

void CMESceneTransition::deleteTankComplex(CSharedMETankComplexRef gameObject)
{
    assert(m_sceneFabricator != nullptr);
    std::shared_ptr<CMESceneFabricator> sceneFabricator = std::static_pointer_cast<CMESceneFabricator>(m_sceneFabricator);
    sceneFabricator->deleteTankComplex(gameObject);
}

