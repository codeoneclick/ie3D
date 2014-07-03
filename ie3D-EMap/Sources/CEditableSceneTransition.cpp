//
//  CEditableSceneTransition.cpp
//  indieEngine-EMap
//
//  Created by Sergey Sergeev on 4/20/14.
//
//

#include "CEditableSceneTransition.h"
#include "CEditableScene.h"
#include "CMESceneFabricator.h"
#include "CSceneGraph.h"
#include "CRenderPipeline.h"

CEditableSceneTransition::CEditableSceneTransition(const std::string& filename,
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

CEditableSceneTransition::~CEditableSceneTransition(void)
{
    
}

void CEditableSceneTransition::initScene(void)
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

void CEditableSceneTransition::_OnLoaded(void)
{
    m_scene = std::make_shared<CEditableScene>(this);
    m_scene->load();
    m_isLoaded = true;
}

void CEditableSceneTransition::_OnGameLoopUpdate(f32 deltatime)
{
    if(m_isLoaded && m_scene != nullptr)
    {
        m_scene->update(deltatime);
    }
}

CSharedSelectionArea CEditableSceneTransition::createSelectionArea(const std::string& filename)
{
    assert(m_sceneFabricator != nullptr);
    std::shared_ptr<CMESceneFabricator> sceneFabricator = std::static_pointer_cast<CMESceneFabricator>(m_sceneFabricator);
    return sceneFabricator->createSelectionArea(filename);
}

void CEditableSceneTransition::deleteSelectionArea(CSharedSelectionAreaRef selectionArea)
{
    assert(m_sceneFabricator != nullptr);
    std::shared_ptr<CMESceneFabricator> sceneFabricator = std::static_pointer_cast<CMESceneFabricator>(m_sceneFabricator);
    sceneFabricator->deleteSelectionArea(selectionArea);
}

