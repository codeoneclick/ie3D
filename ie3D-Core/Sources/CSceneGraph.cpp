//
//  CSceneGraph.cpp
//  indi2dEngine
//
//  Created by Sergey Sergeev on 5/14/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CSceneGraph.h"
#include "CSceneUpdateMgr.h"
#include "CCollisionMgr.h"
#include "CCamera.h"
#include "CLightSource.h"
#include "IGameObject.h"
#include "CModel.h"
#include "COcean.h"
#include "CSkyBox.h"
#include "CAtmosphericScattering.h"
#include "CLandscape.h"
#include "CParticleEmitter.h"
#include "IInputContext.h"
#include "CRenderPipeline.h"

CSceneGraph::CSceneGraph(CSharedRenderPipelineRef renderPipeline,
                         CSharedSceneUpdateMgrRef sceneUpdateMgr,
                         CSharedCollisionMgr collisionMgr,
                         ISharedInputContext inputContext) :
IGraph(renderPipeline, sceneUpdateMgr),
m_camera(nullptr),
m_ocean(nullptr),
m_landscape(nullptr),
m_skyBox(nullptr),
m_atmosphericScattering(nullptr),
m_collisionMgr(collisionMgr),
m_inputContext(inputContext)
{
    m_lightSources.at(0) = nullptr;
    m_lightSources.at(1) = nullptr;
    m_lightSources.at(2) = nullptr;
    m_lightSources.at(3) = nullptr;
}

CSceneGraph::~CSceneGraph(void)
{
    m_gameObjectsContainer.clear();
}

void CSceneGraph::setCamera(CSharedCameraRef camera)
{
    assert(m_sceneUpdateMgr != nullptr);
    if(m_camera != nullptr)
    {
        m_sceneUpdateMgr->UnregisterSceneUpdateHandler(m_camera);
    }
    
    m_camera = camera;
    assert(m_sceneUpdateMgr != nullptr);
    m_sceneUpdateMgr->RegisterSceneUpdateHandler(m_camera);
    
    for(const auto& iterator : m_gameObjectsContainer)
    {
        iterator->setCamera(m_camera);
    }
    
    assert(m_collisionMgr != nullptr);
    m_collisionMgr->setCamera(m_camera);
}

void CSceneGraph::setLightSource(CSharedLightSourceRef lightSource, E_LIGHT_SOURCE index)
{
    assert(m_sceneUpdateMgr != nullptr);
    if(index < E_LIGHT_SOURCE_MAX)
    {
        if(m_lightSources.at(index) != nullptr)
        {
            m_sceneUpdateMgr->UnregisterSceneUpdateHandler(m_lightSources.at(index));
        }
        
        m_lightSources.at(index) = lightSource;
        m_sceneUpdateMgr->RegisterSceneUpdateHandler(m_lightSources.at(index));
        
        for(const auto& iterator : m_gameObjectsContainer)
        {
            iterator->setLightSource(lightSource, index);
        }
    }
}

void CSceneGraph::addGameObject(ISharedGameObjectRef gameObject)
{
    assert(m_sceneUpdateMgr != nullptr);
    assert(m_renderPipeline != nullptr);
    
    if(m_camera != nullptr)
    {
        gameObject->setCamera(m_camera);
    }
    
    for(ui32 i = 0; i < m_lightSources.size(); ++i)
    {
        if(m_lightSources.at(i) != nullptr)
        {
            gameObject->setLightSource(m_lightSources.at(i), static_cast<E_LIGHT_SOURCE>(i));
        }
    }
    
    gameObject->setSceneUpdateMgr(m_sceneUpdateMgr);
    gameObject->setRenderTechniqueImporter(m_renderPipeline);
    gameObject->setRenderTechniqueAccessor(m_renderPipeline);
    gameObject->enableRender(true);
    gameObject->enableUpdate(true);
    m_gameObjectsContainer.insert(gameObject);
}

void CSceneGraph::removeGameObject(ISharedGameObjectRef gameObject)
{
    assert(m_sceneUpdateMgr != nullptr);
    assert(m_renderPipeline != nullptr);
    
    gameObject->setSceneUpdateMgr(nullptr);
    gameObject->setRenderTechniqueImporter(nullptr);
    gameObject->setRenderTechniqueAccessor(nullptr);
    gameObject->enableRender(false);
    gameObject->enableUpdate(false);
    m_gameObjectsContainer.erase(gameObject);
}

void CSceneGraph::addModel(CSharedModelRef model)
{
    CSceneGraph::addGameObject(model);
}

void CSceneGraph::removeModel(CSharedModelRef model)
{
    CSceneGraph::removeGameObject(model);
}

void CSceneGraph::setOcean(CSharedOceanRef ocean)
{
    if(m_ocean != nullptr)
    {
        CSceneGraph::removeGameObject(m_ocean);
    }
    CSceneGraph::addGameObject(ocean);
    m_ocean = ocean;
}

void CSceneGraph::removeOcean(CSharedOceanRef ocean)
{
    CSceneGraph::removeGameObject(ocean);
    m_ocean = nullptr;
}

void CSceneGraph::setSkyBox(CSharedSkyBoxRef skyBox)
{
    if(m_skyBox != nullptr)
    {
        CSceneGraph::removeGameObject(m_skyBox);
    }
    CSceneGraph::addGameObject(skyBox);
    m_skyBox = skyBox;
}

void CSceneGraph::removeSkyBox(CSharedSkyBoxRef skyBox)
{
    CSceneGraph::removeGameObject(skyBox);
    m_skyBox = nullptr;
}

void CSceneGraph::setAtmosphericScattering(CSharedAtmosphericScatteringRef atmosphericScattering)
{
    if(m_atmosphericScattering != nullptr)
    {
        CSceneGraph::removeGameObject(m_atmosphericScattering);
    }
    CSceneGraph::addGameObject(atmosphericScattering);
    m_atmosphericScattering = atmosphericScattering;
}

void CSceneGraph::removeAtmosphericScattering(CSharedAtmosphericScatteringRef atmosphericScattering)
{
    CSceneGraph::removeGameObject(atmosphericScattering);
    m_atmosphericScattering = nullptr;
}

void CSceneGraph::setLandscape(CSharedLandscapeRef landscape)
{
    if(m_landscape != nullptr)
    {
        CSceneGraph::removeGameObject(m_landscape);
    }
    CSceneGraph::addGameObject(landscape);
    m_landscape = landscape;
}

void CSceneGraph::removeLandscape(CSharedLandscapeRef landscape)
{
    CSceneGraph::removeGameObject(landscape);
    m_landscape = nullptr;
}

void CSceneGraph::addParticleEmitter(CSharedParticleEmitterRef particleEmitter)
{
    CSceneGraph::addGameObject(particleEmitter);
}

void CSceneGraph::removeParticleEmitter(CSharedParticleEmitterRef particleEmitter)
{
    CSceneGraph::removeGameObject(particleEmitter);
}

void CSceneGraph::addCustomGameObject(ISharedGameObjectRef gameObject)
{
    CSceneGraph::addGameObject(gameObject);
}

void CSceneGraph::removeCustomGameObject(ISharedGameObjectRef gameObject)
{
    CSceneGraph::removeGameObject(gameObject);
}

void CSceneGraph::addCollisionHandler(ISharedCollisionHandlerRef handler)
{
    assert(m_collisionMgr != nullptr);
    m_collisionMgr->addCollisionHandler(handler);
}

void CSceneGraph::removeCollisionHandler(ISharedCollisionHandlerRef handler)
{
    assert(m_collisionMgr != nullptr);
    m_collisionMgr->removeCollisionHandler(handler);
}

void CSceneGraph::addGestureRecognizerHandler(ISharedGestureRecognizerHandlerRef handler)
{
    assert(m_inputContext != nullptr);
    m_inputContext->addGestureRecognizerHandler(handler);
}

void CSceneGraph::removeGestureRecognizerHandler(ISharedGestureRecognizerHandlerRef handler)
{
    assert(m_inputContext != nullptr);
    m_inputContext->removeGestureRecognizerHandler(handler);
}

