//
//  CSceneGraph.h
//  indi2dEngine
//
//  Created by Sergey Sergeev on 5/14/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CSceneGraph_h
#define CSceneGraph_h

#include "HCommon.h"
#include "HEnums.h"
#include "IGraph.h"
#include "HDeclaration.h"

class CSceneGraph : public IGraph
{
private:
    
    CSharedCamera m_camera;
    CSharedFrustum m_cameraFrustum;
    CSharedGlobalLightSource m_globalLightSource;
    
    std::set<ISharedGameObject> m_gameObjectsContainer;
    CSharedOcean m_ocean;
    CSharedLandscape m_landscape;
    CSharedSkyBox m_skyBox;
    CSharedAtmosphericScattering m_atmosphericScattering;
    
protected:
    
    CSharedCollisionMgr m_collisionMgr;
    ISharedInputContext m_inputContext;
    
    void addGameObject(ISharedGameObjectRef gameObject);
    void removeGameObject(ISharedGameObjectRef gameObject);
    
public:
    
    CSceneGraph(CSharedRenderPipelineRef renderPipeline,
                CSharedSceneUpdateMgrRef sceneUpdateMgr,
                CSharedCollisionMgr collisionMgr,
                ISharedInputContext inputContext);
    virtual ~CSceneGraph(void);
    
    void setCamera(CSharedCameraRef camera);
    void setGlobalLightSource(CSharedGlobalLightSourceRef lightSource);
    
    void addModel(CSharedModelRef model);
    void removeModel(CSharedModelRef model);
    
    void setOcean(CSharedOceanRef ocean);
    void removeOcean(CSharedOceanRef ocean);
    
    void setSkybox(CSharedSkyBoxRef skybox);
    void removeSkybox(CSharedSkyBoxRef skybox);
    
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
    
    void addTouchCollider(ISharedTouchColliderRef collider);
    void removeTouchCollider(ISharedTouchColliderRef collider);
    
    void setBox2dScene(const glm::vec2 &minBound, const glm::vec2 &maxBound);
    void addBox2dCollider(ISharedBox2dColliderRef collider, bool isStatic);
    void removeBox2dCollider(ISharedBox2dColliderRef collider);
};

#endif 
