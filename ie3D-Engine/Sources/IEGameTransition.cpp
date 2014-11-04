//
//  CEGameTransition.cpp
//  ie3D-Engine
//
//  Created by sergey.sergeev on 8/6/14.
//  Copyright (c) 2014 sergey.sergeev. All rights reserved.
//

#include "IEGameTransition.h"
#include "CESceneFabricator.h"
#include "CSceneGraph.h"
#include "CRenderPipeline.h"

IEGameTransition::IEGameTransition(const std::string& filename, bool isOffscreen) :
IGameTransition(filename, isOffscreen)
{
    
}

IEGameTransition::~IEGameTransition(void)
{
    
}

void IEGameTransition::initScene(void)
{
    assert(m_graphicsContext != nullptr);
    assert(m_inputContext != nullptr);
    assert(m_sceneUpdateMgr != nullptr);
    assert(m_collisionMgr != nullptr);
    assert(m_renderPipeline != nullptr);
    
    m_sceneGraph = std::make_shared<CSceneGraph>(m_renderPipeline, m_sceneUpdateMgr,
                                                 m_collisionMgr, m_inputContext);
    
    m_sceneFabricator = std::make_shared<CESceneFabricator>(m_configurationAccessor,
                                                            m_resourceAccessor,
                                                            m_renderPipeline);
}

void IEGameTransition::_OnLoaded(void)
{
    assert(false);
}

void IEGameTransition::_OnGameLoopUpdate(f32 deltatime)
{
    assert(false);
}

CESharedComplexModel IEGameTransition::createComplexModel(const std::string& filename)
{
    assert(m_sceneFabricator != nullptr);
    std::shared_ptr<CESceneFabricator> sceneFabricator = std::static_pointer_cast<CESceneFabricator>(m_sceneFabricator);
    return sceneFabricator->createComplexModel(filename);
}

CESharedCustomModel IEGameTransition::createCustomModel(const std::string& filename)
{
    assert(m_sceneFabricator != nullptr);
    std::shared_ptr<CESceneFabricator> sceneFabricator = std::static_pointer_cast<CESceneFabricator>(m_sceneFabricator);
    return sceneFabricator->createCustomModel(filename);
}
