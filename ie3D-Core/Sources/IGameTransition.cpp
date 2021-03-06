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
#include "CSceneUpdateMgr.h"
#include "CCollisionMgr.h"
#include "CCommonOS.h"
#include "IGraphicsContext.h"
#include "IInputContext.h"
#include "CResourceLoader.h"
#include "CSceneGraph.h"
#include "CSceneFabricator.h"
#include "CRenderPipeline.h"
#include "CRenderTechniqueMain.h"
#include "CRenderTechniqueWorldSpace.h"
#include "CRenderTechniqueScreenSpace.h"
#include "IScene.h"

IGameTransition::IGameTransition(const std::string& filename, bool isOffscreen) :
m_guid(filename),
m_scene(nullptr),
m_isLoaded(false),
m_graphicsContext(nullptr),
m_inputContext(nullptr),
m_resourceAccessor(nullptr),
m_configurationAccessor(nullptr),
m_isOffscreen(isOffscreen),
m_sceneToUICommands(nullptr)
{

}

IGameTransition::~IGameTransition(void)
{
    
}

void IGameTransition::setupOnce(ISharedGraphicsContextRef graphicsContext,
                                ISharedInputContextRef inputContext,
                                CSharedResourceAccessorRef resourceAccessor,
                                CSharedConfigurationAccessorRef configurationAccessor)
{
    assert(m_graphicsContext == nullptr);
    assert(m_inputContext == nullptr);
    assert(m_sceneUpdateMgr == nullptr);
    assert(m_collisionMgr == nullptr);
    assert(m_renderPipeline == nullptr);
    
    m_graphicsContext = graphicsContext;
    m_inputContext = inputContext;
    m_resourceAccessor = resourceAccessor;
    m_configurationAccessor = configurationAccessor;
    m_renderPipeline = std::make_shared<CRenderPipeline>(m_graphicsContext, m_isOffscreen);
    //CSharedBatchingMgr batchingMgr = std::make_shared<CBatchingMgr>(m_renderPipeline);
    //m_renderPipeline->setBatchingMgr(batchingMgr);
    m_sceneUpdateMgr = std::make_shared<CSceneUpdateMgr>();
    
    m_collisionMgr = std::make_shared<CCollisionMgr>();
    m_inputContext->addGestureRecognizerHandler(std::static_pointer_cast<IGestureRecognizerHandler>(m_collisionMgr));
    
    assert(m_graphicsContext != nullptr);
    assert(m_inputContext != nullptr);
    assert(m_sceneUpdateMgr != nullptr);
    assert(m_collisionMgr != nullptr);
    assert(m_renderPipeline != nullptr);
}

void IGameTransition::initScene(void)
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

void IGameTransition::setPaused(bool value)
{
    assert(m_renderPipeline != nullptr);
    assert(m_sceneUpdateMgr != nullptr);
    m_renderPipeline->setPaused(value);
    m_sceneUpdateMgr->setPaused(value);
}

void IGameTransition::_OnRegistered(void)
{
    
}

void IGameTransition::_OnUnregistered(void)
{
    
}

void IGameTransition::_OnActivate(void)
{
    ConnectToGameLoop(m_renderPipeline);
    ConnectToGameLoop(m_sceneUpdateMgr);
    ConnectToGameLoop(m_collisionMgr);
    ConnectToGameLoop(m_resourceAccessor->getResourceLoader());
}

void IGameTransition::_OnDeactivate(void)
{
    DisconnectFromGameLoop(m_renderPipeline);
    DisconnectFromGameLoop(m_sceneUpdateMgr);
    DisconnectFromGameLoop(m_collisionMgr);
    DisconnectFromGameLoop(m_resourceAccessor->getResourceLoader());
}

void IGameTransition::_OnLoaded(void)
{
    if(m_sceneToUICommands)
    {
        assert(m_scene);
        m_scene->setSceneToUICommands(m_sceneToUICommands);
    }
}

void IGameTransition::_OnGameLoopUpdate(f32 deltatime)
{
    assert(false);
}

void IGameTransition::onConfigurationLoaded(ISharedConfigurationRef configuration, bool success)
{
    assert(m_renderPipeline != nullptr);
    assert(m_resourceAccessor != nullptr);
    
    std::shared_ptr<CConfigurationTransition> configurationTransition = std::static_pointer_cast<CConfigurationTransition>(configuration);
    assert(configurationTransition != nullptr);
    
    for(const auto& iterator : configurationTransition->getConfigurationWSTechnique())
    {
        std::shared_ptr<CConfigurationWSTechnique> configurationWSTechnique = std::static_pointer_cast<CConfigurationWSTechnique>(iterator);
        
        ui32 screenWidth = MIN_VALUE(configurationWSTechnique->getScreenWidth(), m_graphicsContext->getWidth());
        ui32 screenHeight = MIN_VALUE(configurationWSTechnique->getScreenHeight(), m_graphicsContext->getHeight());
        
        CSharedRenderTechniqueWorldSpace renderWSTechnique =
        std::make_shared<CRenderTechniqueWorldSpace>(screenWidth,
                                                     screenHeight,
                                                     configurationWSTechnique->getGUID(),
                                                     configurationWSTechnique->getIndex());
        glm::vec4 color = glm::vec4(configurationWSTechnique->getClearColorR(),
                                    configurationWSTechnique->getClearColorG(),
                                    configurationWSTechnique->getClearColorB(),
                                    configurationWSTechnique->getClearColorA());
        renderWSTechnique->setClearColor(color);
        renderWSTechnique->setAreDrawBoundingBoxes(configurationWSTechnique->getDrawBoundingBoxes());
        m_renderPipeline->addWorldSpaceRenderTechnique(configurationWSTechnique->getGUID(), renderWSTechnique);
    }
    
    for(const auto& iterator : configurationTransition->getConfigurationSSTechnique())
    {
        std::shared_ptr<CConfigurationSSTechnique> configurationSSTechnique = std::static_pointer_cast<CConfigurationSSTechnique>(iterator);
        assert(configurationSSTechnique != nullptr);
        std::shared_ptr<CConfigurationMaterial> configurationMaterial = configurationSSTechnique->getConfigurationMaterial();
        assert(configurationMaterial != nullptr);
        
        assert(m_resourceAccessor != nullptr);
        CSharedMaterial material =  CMaterial::constructCustomMaterial(configurationMaterial,
                                                                       m_resourceAccessor,
                                                                       m_renderPipeline);
        
        ui32 screenWidth = MIN_VALUE(configurationSSTechnique->getScreenWidth(), m_graphicsContext->getWidth());
        ui32 screenHeight = MIN_VALUE(configurationSSTechnique->getScreenHeight(), m_graphicsContext->getHeight());
        
        CSharedRenderTechniqueScreenSpace renderSSTechnique =
        std::make_shared<CRenderTechniqueScreenSpace>(screenWidth,
                                                      screenHeight,
                                                      configurationSSTechnique->getGUID(),
                                                      material);
        m_renderPipeline->addScreenSpaceRenderTechnique(configurationSSTechnique->getGUID(), renderSSTechnique);
    }
    
    if(!m_isOffscreen)
    {
        std::shared_ptr<CConfigurationOutputTechnique> configurationOutputTechnique = configurationTransition->getConfigurationOutputTechnique();
        assert(configurationOutputTechnique != nullptr);
        std::shared_ptr<CConfigurationMaterial> configurationMaterial = configurationOutputTechnique->getConfigurationMaterial();
        assert(configurationMaterial != nullptr);
        
        assert(m_resourceAccessor != nullptr);
        CSharedMaterial material =  CMaterial::constructCustomMaterial(configurationMaterial,
                                                                       m_resourceAccessor,
                                                                       m_renderPipeline);
        m_renderPipeline->setMainRenderTechnique(material);
    }
    
    _OnLoaded();
}

std::string IGameTransition::getGuid(void) const
{
    return m_guid;
}

ISharedRenderTechniqueImporter IGameTransition::getRenderTechniqueImporter(void) const
{
    assert(m_renderPipeline != nullptr);
    return m_renderPipeline;
}

ISharedRenderTechniqueAccessor IGameTransition::getRenderTechniqueAccessor(void) const
{
    assert(m_renderPipeline != nullptr);
    return m_renderPipeline;
}

void IGameTransition::setCamera(CSharedCameraRef camera)
{
    assert(m_sceneGraph != nullptr);
    m_sceneGraph->setCamera(camera);
}

void IGameTransition::setGlobalLightSource(CSharedGlobalLightSourceRef lightSource)
{
    assert(m_sceneGraph != nullptr);
    m_sceneGraph->setGlobalLightSource(lightSource);
}

const CSharedResourceAccessor IGameTransition::getResourceAccessor(void) const
{
    assert(m_resourceAccessor != nullptr);
    return m_resourceAccessor;
}

const CSharedConfigurationAccessor IGameTransition::getConfigurationAccessor(void) const
{
    assert(m_configurationAccessor != nullptr);
    return m_configurationAccessor;
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

void IGameTransition::setSkybox(CSharedSkyBoxRef skybox)
{
    assert(m_sceneGraph != nullptr);
    m_sceneGraph->setSkybox(skybox);
}

void IGameTransition::removeSkybox(CSharedSkyBoxRef skybox)
{
    assert(m_sceneGraph != nullptr);
    m_sceneGraph->removeSkybox(skybox);
}

void IGameTransition::setAtmosphericScattering(CSharedAtmosphericScatteringRef atmosphericScattering)
{
    assert(m_sceneGraph != nullptr);
    m_sceneGraph->setAtmosphericScattering(atmosphericScattering);
}

void IGameTransition::removeAtmosphericScattering(CSharedAtmosphericScatteringRef atmosphericScattering)
{
    assert(m_sceneGraph != nullptr);
    m_sceneGraph->removeAtmosphericScattering(atmosphericScattering);
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

void IGameTransition::addTouchCollider(ISharedTouchColliderRef collider)
{
    assert(m_sceneGraph != nullptr);
    m_sceneGraph->addTouchCollider(collider);
}

void IGameTransition::removeTouchCollider(ISharedTouchColliderRef collider)
{
    assert(m_sceneGraph != nullptr);
    m_sceneGraph->removeTouchCollider(collider);
}

void IGameTransition::setBox2dScene(const glm::vec2 &minBound, const glm::vec2 &maxBound)
{
    assert(m_sceneGraph != nullptr);
    m_sceneGraph->setBox2dScene(minBound, maxBound);
}

void IGameTransition::addBox2dCollider(ISharedBox2dColliderRef collider, bool isStatic)
{
    assert(m_sceneGraph != nullptr);
    m_sceneGraph->addBox2dCollider(collider, isStatic);
}

void IGameTransition::removeBox2dCollider(ISharedBox2dColliderRef collider)
{
    assert(m_sceneGraph != nullptr);
    m_sceneGraph->removeBox2dCollider(collider);
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

CSharedGlobalLightSource IGameTransition::createGlobalLightSource(f32 fov, f32 near, f32 far)
{
    assert(m_sceneFabricator != nullptr);
    return m_sceneFabricator->createGlobalLightSource(fov, near, far);
}

void IGameTransition::deleteGlobalLightSource(CSharedGlobalLightSourceRef lightSource)
{
    assert(m_sceneFabricator != nullptr);
    m_sceneFabricator->deleteGlobalLightSource(lightSource);
}

CSharedModel IGameTransition::createModel(const std::string& filename)
{
    assert(m_sceneFabricator != nullptr);
    return m_sceneFabricator->createModel(filename);
}

CSharedOcean IGameTransition::createOcean(const std::string& filename)
{
    assert(m_sceneFabricator != nullptr);
    return m_sceneFabricator->createOcean(filename);
}

CSharedSkyBox IGameTransition::createSkybox(const std::string& filename)
{
    assert(m_sceneFabricator != nullptr);
    return m_sceneFabricator->createSkybox(filename);
}

CSharedAtmosphericScattering IGameTransition::createAtmosphericScattering(const std::string& filename)
{
    assert(m_sceneFabricator != nullptr);
    return m_sceneFabricator->createAtmosphericScattering(filename);
}

CSharedLandscape IGameTransition::createLandscape(const std::string& filename)
{
    assert(m_sceneFabricator != nullptr);
    return m_sceneFabricator->createLandscape(filename);
}

CSharedParticleEmitter IGameTransition::createParticleEmitter(const std::string& filename)
{
    assert(m_sceneFabricator != nullptr);
    return m_sceneFabricator->createParticleEmitter(filename);
}

void IGameTransition::deleteGameObject(ISharedGameObjectRef gameObject)
{
    assert(m_sceneFabricator != nullptr);
    m_sceneFabricator->deleteGameObject(gameObject);
}

ui32 IGameTransition::getFrameNumTriangles(void)
{
    assert(m_renderPipeline != nullptr);
    return m_renderPipeline->getFrameNumTriagles();
}

ui32 IGameTransition::getSceneNumTriangles(void)
{
    return 0;
}

ui32 IGameTransition::getScreenWidth(void) const
{
    return m_renderPipeline->getScreenWidth();
}

ui32 IGameTransition::getScreenHeight(void) const
{
    return m_renderPipeline->getScreenHeight();
}

void IGameTransition::setSceneToUICommands(ISharedUICommandsRef commands)
{
    if(m_scene)
    {
        m_scene->setSceneToUICommands(commands);
    }
    m_sceneToUICommands = commands;
}

ISharedUICommands IGameTransition::getUIToSceneCommands(void) const
{
    return m_isLoaded && m_scene != nullptr ? m_scene->getUIToSceneCommands() : nullptr;
}

