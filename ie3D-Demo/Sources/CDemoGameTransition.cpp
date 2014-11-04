//
//  CDemoGameTransition.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/12/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CDemoGameTransition.h"
#include "CDemoGameScene.h"
#include "CSceneGraph.h"
#include "CSceneFabricator.h"
#include "CRenderPipeline.h"

CDemoGameTransition::CDemoGameTransition(const std::string& filename) :
IEGameTransition(filename)
{
    
}

CDemoGameTransition::~CDemoGameTransition(void)
{
    
}

void CDemoGameTransition::initScene(void)
{
    assert(m_graphicsContext != nullptr);
    assert(m_inputContext != nullptr);
    assert(m_sceneUpdateMgr != nullptr);
    assert(m_collisionMgr != nullptr);
    assert(m_renderPipeline != nullptr);
    
    m_sceneGraph = std::make_shared<CSceneGraph>(m_renderPipeline, m_sceneUpdateMgr,
                                                 m_collisionMgr, m_inputContext);
    
    m_sceneFabricator = std::make_shared<CSceneFabricator>(m_configurationAccessor,
                                                           m_resourceAccessor,
                                                           m_renderPipeline);
}

void CDemoGameTransition::_OnLoaded(void)
{																			
    m_scene = std::make_shared<CDemoGameScene>(this);
    m_scene->load();
    m_isLoaded = true;
}

void CDemoGameTransition::_OnGameLoopUpdate(f32 deltatime)
{
    if(m_isLoaded && m_scene != nullptr)
    {
        m_scene->update(deltatime);
    }
}

CDESharedUIToSceneCommands CDemoGameTransition::getUIToSceneCommands(void) const
{
    return m_isLoaded && m_scene != nullptr ? std::static_pointer_cast<CDemoGameScene>(m_scene)->getUIToSceneCommands() : nullptr;
}

void CDemoGameTransition::setSceneToUICommands(CDESharedSceneToUICommandsRef commands)
{
    assert(m_isLoaded && m_scene != nullptr);
    std::static_pointer_cast<CDemoGameScene>(m_scene)->setSceneToUICommands(commands);
}