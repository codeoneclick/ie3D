//
//  CMEgopTransition.cpp
//  ie3D-EMap
//
//  Created by sergey.sergeev on 11/3/14.
//
//

#include "CMEgopTransition.h"
#include "CMEgopScene.h"
#include "CMESceneFabricator.h"
#include "CSceneGraph.h"
#include "CRenderPipeline.h"

CMEgopTransition::CMEgopTransition(const std::string& filename, bool isOffscreen) :
IEGameTransition(filename, isOffscreen)
{
    
}

CMEgopTransition::~CMEgopTransition(void)
{
    
}

void CMEgopTransition::initScene(void)
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

void CMEgopTransition::_OnLoaded(void)
{
    m_scene = std::make_shared<CMEgopScene>(this);
    m_scene->load();
    m_isLoaded = true;
}

void CMEgopTransition::_OnGameLoopUpdate(f32 deltatime)
{
    if(m_isLoaded && m_scene != nullptr)
    {
        m_scene->update(deltatime);
    }
}