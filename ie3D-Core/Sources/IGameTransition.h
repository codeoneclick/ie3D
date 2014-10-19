//
//  IGameTransition.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/6/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IGameTransition_h
#define IGameTransition_h

#include "HCommon.h"
#include "IConfiguration.h"
#include "HDeclaration.h"
#include "IGameLoopHandler.h"

class IGameTransition :
public IConfigurationLoadingHandler,
public IGameLoopHandler
{
private:
    
protected:
    
    friend class IGameController;
    
    ISharedGraphicsContext m_graphicsContext;
    ISharedInputContext m_inputContext;
    CSharedResourceAccessor m_resourceAccessor;
    CSharedConfigurationAccessor m_configurationAccessor;
    
    CSharedSceneGraph m_sceneGraph;
    CSharedSceneFabricator m_sceneFabricator;
    ISharedScene m_scene;
    
    CSharedRenderPipeline m_renderPipeline;
    CSharedSceneUpdateMgr m_sceneUpdateMgr;
    CSharedCollisionMgr m_collisionMgr;
    
    std::string m_guid;
    
    bool m_isLoaded;
    
    virtual void _OnRegistered(void);
    virtual void _OnUnregistered(void);
    
    virtual void _OnActivate(void);
    virtual void _OnDeactivate(void);
    
    virtual void onConfigurationLoaded(ISharedConfigurationRef configuration, bool success);
    virtual void _OnLoaded(void);
    
    virtual void _OnGameLoopUpdate(f32 _deltatime);
    
public:
    
    IGameTransition(const std::string& filename,
                    ISharedGraphicsContextRef graphicsContext,
                    ISharedInputContextRef inputContext,
                    CSharedResourceAccessorRef resourceAccessor,
                    CSharedConfigurationAccessorRef configurationAccessor);
    
    virtual ~IGameTransition(void);
    
    virtual void initScene(void);

    std::string getGuid(void) const;
    
    ui32 getScreenWidth(void) const;
    ui32 getScreenHeight(void) const;
    
    ISharedRenderTechniqueImporter getRenderTechniqueImporter(void) const;
    ISharedRenderTechniqueAccessor getRenderTechniqueAccessor(void) const;
    
    void setCamera(CSharedCameraRef camera);
    
    void setLightSource(CSharedLightSourceRef lightSource, E_LIGHT_SOURCE index);
    
    const CSharedResourceAccessor getResourceAccessor(void) const;
    const CSharedConfigurationAccessor getConfigurationAccessor(void) const;
    
    void addModel(CSharedModelRef model);
    void removeModel(CSharedModelRef model);
    
    void setOcean(CSharedOceanRef ocean);
    void removeOcean(CSharedOceanRef ocean);
    
    void setSkyBox(CSharedSkyBoxRef skybox);
    void removeSkyBox(CSharedSkyBoxRef skybox);
    
    void setAtmosphericScattering(CSharedAtmosphericScatteringRef atmosphericScattering);
    void removeAtmosphericScattering(CSharedAtmosphericScatteringRef atmosphericScattering);
    
    void setLandscape(CSharedLandscapeRef landscape);
    void removeLandscape(CSharedLandscapeRef landscape);
    
    void addParticleEmitter(CSharedParticleEmitterRef particleEmitter);
    void removeParticleEmitter(CSharedParticleEmitterRef particleEmitter);
    
    void addCustomGameObject(ISharedGameObjectRef gameObject);
    void removeCustomGameObject(ISharedGameObjectRef gameObject);
    
    void addGestureRecognizerHandler(ISharedGestureRecognizerHandlerRef handler);
    void removeGestureRecognizerHandler(ISharedGestureRecognizerHandlerRef handler);
    
    void addCollisionHandler(ISharedCollisionHandlerRef handler);
    void removeCollisionHandler(ISharedCollisionHandlerRef handler);
    
    CSharedCamera createCamera(f32 fov, f32 near, f32 far,const glm::ivec4& viewport);
    void deleteCamera(CSharedCameraRef camera);
    
    CSharedLightSource createLightSource(f32 fov, f32 near, f32 far, f32 aspect);
    void deleteLightSource(CSharedLightSourceRef lightSource);
    
    CSharedModel createModel(const std::string& filename);
    CSharedOcean createOcean(const std::string& filename);
    CSharedSkyBox createSkyBox(const std::string& filename);
    CSharedAtmosphericScattering createAtmosphericScattering(const std::string& filename);
    CSharedLandscape createLandscape(const std::string& filename);
    CSharedParticleEmitter createParticleEmitter(const std::string& filename);
    
    void deleteGameObject(ISharedGameObjectRef gameObject);
    
    ui32 getFrameNumTriangles(void);
    ui32 getSceneNumTriangles(void);
};

#endif 
