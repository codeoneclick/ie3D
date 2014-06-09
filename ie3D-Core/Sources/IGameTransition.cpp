//
//  IGameTransition.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/6/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IGameTransition.h"
#include "CResourceAccessor.h"
#include "CConfigurationAccessor.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CTexture.h"
#include "IGameObject.h"
#include "CGameLoopExecutor.h"
#include "CRenderMgr.h"
#include "CBatchingMgr.h"
#include "CSceneUpdateMgr.h"
#include "CCollisionMgr.h"
#include "CRenderOperationWorldSpace.h"
#include "CRenderOperationScreenSpace.h"
#include "CCommonOS.h"
#include "CConfigurationGameObjects.h"
#include "IGraphicsContext.h"
#include "IInputContext.h"
#include "CResourceLoader.h"
#include "CSceneGraph.h"
#include "CSceneFabricator.h"

IGameTransition::IGameTransition(const std::string& filename,
                                 ISharedGraphicsContextRef graphicsContext,
                                 ISharedInputContextRef inputContext,
                                 CSharedResourceAccessorRef resourceAccessor,
                                 CSharedConfigurationAccessorRef configurationAccessor) :
m_guid(filename),
m_scene(nullptr),
m_isLoaded(false),
m_graphicsContext(graphicsContext),
m_inputContext(inputContext),
m_resourceAccessor(resourceAccessor),
m_configurationAccessor(configurationAccessor)
{
    assert(m_graphicsContext != nullptr);
    assert(m_inputContext != nullptr);
    
    m_renderMgr = std::make_shared<CRenderMgr>(m_graphicsContext);
    m_screenSpaceTextureAccessor = m_renderMgr;
    m_sceneUpdateMgr = std::make_shared<CSceneUpdateMgr>();
    
    std::shared_ptr<CBatchingMgr> batchingMgr = std::make_shared<CBatchingMgr>(m_renderMgr);
    m_renderMgr->Set_BatchingMgr(batchingMgr);
    
    m_collisionMgr = std::make_shared<CCollisionMgr>();
    m_inputContext->addGestureRecognizerHandler(std::static_pointer_cast<IGestureRecognizerHandler>(m_collisionMgr));
}

IGameTransition::~IGameTransition(void)
{
    
}

void IGameTransition::initScene(void)
{
    assert(m_graphicsContext != nullptr);
    assert(m_inputContext != nullptr);
    assert(m_sceneUpdateMgr != nullptr);
    assert(m_collisionMgr != nullptr);
    assert(m_screenSpaceTextureAccessor != nullptr);
    
    m_sceneGraph = std::make_shared<CSceneGraph>(m_renderMgr, m_sceneUpdateMgr,
                                                 m_collisionMgr, m_inputContext);
    
    m_sceneFabricator = std::make_shared<CSceneFabricator>(m_configurationAccessor,
                                                           m_resourceAccessor,
                                                           m_screenSpaceTextureAccessor);
}

void IGameTransition::_OnRegistered(void)
{
    
}

void IGameTransition::_OnUnregistered(void)
{
    
}

void IGameTransition::_OnActivate(void)
{
    ConnectToGameLoop(m_renderMgr);
    ConnectToGameLoop(m_sceneUpdateMgr);
    ConnectToGameLoop(m_resourceAccessor->getResourceLoader());
}

void IGameTransition::_OnDeactivate(void)
{
    DisconnectFromGameLoop(m_renderMgr);
    DisconnectFromGameLoop(m_sceneUpdateMgr);
    DisconnectFromGameLoop(m_resourceAccessor->getResourceLoader());
}

void IGameTransition::_OnLoaded(void)
{
    assert(false);
}

void IGameTransition::_OnGameLoopUpdate(f32 _deltatime)
{
    assert(false);
}

void IGameTransition::onConfigurationLoaded(ISharedConfigurationRef configuration, bool success)
{
    assert(m_renderMgr != nullptr);
    assert(m_resourceAccessor != nullptr);
    
    std::shared_ptr<CConfigurationGameTransition> gameTransitionConfiguration = std::static_pointer_cast<CConfigurationGameTransition>(configuration);
    assert(gameTransitionConfiguration != nullptr);
    
    for(const auto& iterator : gameTransitionConfiguration->getWSRenderOperationsConfigurations())
    {
        std::shared_ptr<CConfigurationWSRenderOperation> worldSpaceRenderOperationConfiguration = std::static_pointer_cast<CConfigurationWSRenderOperation>(iterator);
        std::shared_ptr<CRenderOperationWorldSpace> worldSpaceRenderOperation =
        std::make_shared<CRenderOperationWorldSpace>(worldSpaceRenderOperationConfiguration->getScreenWidth(),
                                                     worldSpaceRenderOperationConfiguration->getScreenHeight(),
                                                     worldSpaceRenderOperationConfiguration->getClearColor(),
                                                     worldSpaceRenderOperationConfiguration->getGuid(),
                                                     worldSpaceRenderOperationConfiguration->getIndex());
        m_renderMgr->RegisterWorldSpaceRenderOperation(worldSpaceRenderOperationConfiguration->getGuid(), worldSpaceRenderOperation);
    }
    
    for(const auto& iterator : gameTransitionConfiguration->getSSRenderOperationsConfigurations())
    {
        std::shared_ptr<CConfigurationSSRenderOperation> screenSpaceRenderOperationConfiguration = std::static_pointer_cast<CConfigurationSSRenderOperation>(iterator);
        std::shared_ptr<CConfigurationMaterial> screenSpaceRenderOperationMaterialConfiguration = std::static_pointer_cast<CConfigurationMaterial>(screenSpaceRenderOperationConfiguration->getMaterialConfiguration());
        assert(screenSpaceRenderOperationMaterialConfiguration != nullptr);
        
        std::shared_ptr<CMaterial> screenSpaceRenderOperationMaterial = std::make_shared<CMaterial>();
        
        assert(screenSpaceRenderOperationMaterialConfiguration != nullptr);
        assert(m_screenSpaceTextureAccessor != nullptr);
        assert(m_resourceAccessor != nullptr);
        
        CMaterial::setupMaterial(screenSpaceRenderOperationMaterial,
                                 screenSpaceRenderOperationMaterialConfiguration,
                                 m_resourceAccessor,
                                 m_screenSpaceTextureAccessor);
        
        std::shared_ptr<CRenderOperationScreenSpace> screenSpaceRenderOperation =
        std::make_shared<CRenderOperationScreenSpace>(screenSpaceRenderOperationConfiguration->getScreenWidth(),
                                                      screenSpaceRenderOperationConfiguration->getScreenHeight(),
                                                      screenSpaceRenderOperationConfiguration->getGuid(),
                                                      screenSpaceRenderOperationMaterial);
        m_renderMgr->RegisterScreenSpaceRenderOperation( screenSpaceRenderOperationConfiguration->getGuid(), screenSpaceRenderOperation);
    }
    
    std::shared_ptr<CConfigurationORenderOperation> outputRenderOperationConfiguration = std::static_pointer_cast<CConfigurationORenderOperation>(gameTransitionConfiguration->getORenderOperationConfiguration());
    std::shared_ptr<CConfigurationMaterial> outputRenderOperationMaterialConfiguration = std::static_pointer_cast<CConfigurationMaterial>(outputRenderOperationConfiguration->getMaterialConfiguration());
    assert(outputRenderOperationMaterialConfiguration != nullptr);
    
    std::shared_ptr<CMaterial> outputRenderOperationMaterial = std::make_shared<CMaterial>();
    
    assert(outputRenderOperationMaterialConfiguration != nullptr);
    assert(m_screenSpaceTextureAccessor != nullptr);
	assert(m_resourceAccessor != nullptr);
    
    CMaterial::setupMaterial(outputRenderOperationMaterial,
                             outputRenderOperationMaterialConfiguration,
                             m_resourceAccessor,
                             m_screenSpaceTextureAccessor);
    m_renderMgr->RegisterOutputRenderOperation(outputRenderOperationMaterial);
    
    _OnLoaded();
}

std::string IGameTransition::getGuid(void) const
{
    return m_guid;
}

void IGameTransition::setCamera(CSharedCameraRef camera)
{
    assert(m_sceneGraph != nullptr);
    m_sceneGraph->setCamera(camera);
}

void IGameTransition::addModel(CSharedModelRef model)
{
    assert(m_sceneGraph != nullptr);
    m_sceneGraph->addModel(model);
}

void IGameTransition::removeModel(CSharedModelRef model)
{
    assert(m_sceneGraph != nullptr);
    m_sceneGraph->removeModel(model);
}

void IGameTransition::setOcean(CSharedOceanRef ocean)
{
    assert(m_sceneGraph != nullptr);
    m_sceneGraph->setOcean(ocean);
}

void IGameTransition::removeOcean(CSharedOceanRef ocean)
{
    assert(m_sceneGraph != nullptr);
    m_sceneGraph->removeOcean(ocean);
}

void IGameTransition::setSkyBox(CSharedSkyBoxRef skybox)
{
    assert(m_sceneGraph != nullptr);
    m_sceneGraph->setSkyBox(skybox);
}

void IGameTransition::removeSkyBox(CSharedSkyBoxRef skybox)
{
    assert(m_sceneGraph != nullptr);
    m_sceneGraph->removeSkyBox(skybox);
}

void IGameTransition::setLandscape(CSharedLandscapeRef landscape)
{
    assert(m_sceneGraph != nullptr);
    m_sceneGraph->setLandscape(landscape);
}

void IGameTransition::removeLandscape(CSharedLandscapeRef landscape)
{
    assert(m_sceneGraph != nullptr);
    m_sceneGraph->removeLandscape(landscape);
}

void IGameTransition::addParticleEmitter(CSharedParticleEmitterRef particleEmitter)
{
    assert(m_sceneGraph != nullptr);
    m_sceneGraph->addParticleEmitter(particleEmitter);
}

void IGameTransition::removeParticleEmitter(CSharedParticleEmitterRef particleEmitter)
{
    assert(m_sceneGraph != nullptr);
    m_sceneGraph->removeParticleEmitter(particleEmitter);
}

void IGameTransition::addCustomGameObject(ISharedGameObjectRef gameObject)
{
    assert(m_sceneGraph != nullptr);
    m_sceneGraph->addCustomGameObject(gameObject);
}

void IGameTransition::removeCustomGameObject(ISharedGameObjectRef gameObject)
{
    assert(m_sceneGraph != nullptr);
    m_sceneGraph->removeCustomGameObject(gameObject);
}

void IGameTransition::addGestureRecognizerHandler(ISharedGestureRecognizerHandlerRef handler)
{
    assert(m_sceneGraph != nullptr);
    m_sceneGraph->addGestureRecognizerHandler(handler);
}

void IGameTransition::removeGestureRecognizerHandler(ISharedGestureRecognizerHandlerRef handler)
{
    assert(m_sceneGraph != nullptr);
    m_sceneGraph->removeGestureRecognizerHandler(handler);
}

void IGameTransition::addCollisionHandler(ISharedCollisionHandlerRef handler)
{
    assert(m_sceneGraph != nullptr);
    m_sceneGraph->addCollisionHandler(handler);
}

void IGameTransition::removeCollisionHandler(ISharedCollisionHandlerRef handler)
{
    assert(m_sceneGraph != nullptr);
    m_sceneGraph->removeCollisionHandler(handler);
}

CSharedCamera IGameTransition::createCamera(f32 fov, f32 near, f32 far,const glm::ivec4& viewport)
{
    assert(m_sceneFabricator != nullptr);
    return m_sceneFabricator->createCamera(fov, near, far, viewport);
}

void IGameTransition::deleteCamera(CSharedCameraRef camera)
{
    assert(m_sceneFabricator != nullptr);
    m_sceneFabricator->deleteCamera(camera);
}

CSharedModel IGameTransition::createModel(const std::string& filename)
{
    assert(m_sceneFabricator != nullptr);
    return m_sceneFabricator->createModel(filename);
}

void IGameTransition::deleteModel(CSharedModelRef model)
{
    assert(m_sceneFabricator != nullptr);
    m_sceneFabricator->deleteModel(model);
}

CSharedOcean IGameTransition::createOcean(const std::string& filename)
{
    assert(m_sceneFabricator != nullptr);
    return m_sceneFabricator->createOcean(filename);
}

void IGameTransition::deleteOcean(CSharedOceanRef ocean)
{
    assert(m_sceneFabricator != nullptr);
    m_sceneFabricator->deleteOcean(ocean);
}

CSharedSkyBox IGameTransition::createSkyBox(const std::string& filename)
{
    assert(m_sceneFabricator != nullptr);
    return m_sceneFabricator->createSkyBox(filename);
}

void IGameTransition::deleteSkyBox(CSharedSkyBoxRef skyBox)
{
    assert(m_sceneFabricator != nullptr);
    m_sceneFabricator->deleteSkyBox(skyBox);
}

CSharedLandscape IGameTransition::createLandscape(const std::string& filename)
{
    assert(m_sceneFabricator != nullptr);
    return m_sceneFabricator->createLandscape(filename);
}

void IGameTransition::deleteLandscape(CSharedLandscapeRef landscape)
{
    assert(m_sceneFabricator != nullptr);
    m_sceneFabricator->deleteLandscape(landscape);
}

CSharedParticleEmitter IGameTransition::createParticleEmitter(const std::string& filename)
{
    assert(m_sceneFabricator != nullptr);
    return m_sceneFabricator->createParticleEmitter(filename);
}

void IGameTransition::deleteParticleEmitter(CSharedParticleEmitterRef particleEmitter)
{
    assert(m_sceneFabricator != nullptr);
    m_sceneFabricator->deleteParticleEmitter(particleEmitter);
}

ui32 IGameTransition::Get_CurrentNumTriangles(void)
{
    assert(m_renderMgr != nullptr);
    return m_renderMgr->Get_NumTriangles();
}


ui32 IGameTransition::Get_TotalNumTriangles(void)
{
    return 0;
}

ui32 IGameTransition::getWindowWidth(void) const
{
    return m_renderMgr->getWidth();
}

ui32 IGameTransition::getWindowHeight(void) const
{
    return m_renderMgr->getHeight();
}

