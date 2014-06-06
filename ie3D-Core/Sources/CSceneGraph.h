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
    
    std::set<ISharedGameObject> m_gameObjectsContainer;
    CSharedOcean m_ocean;
    CSharedLandscape m_landscape;
    CSharedSkyBox m_skyBox;
    
protected:
    
    CSharedCollisionMgr m_collisionMgr;
    ISharedInputContext m_inputContext;
    
    void addGameObject(ISharedGameObjectRef gameObject);
    void removeGameObject(ISharedGameObjectRef gameObject);
    
public:
    
    CSceneGraph(CSharedRenderMgrRef renderMgr,
                CSharedSceneUpdateMgrRef sceneUpdateMgr,
                CSharedCollisionMgr collisionMgr,
                ISharedInputContext inputContext);
    virtual ~CSceneGraph(void);
    
    void setCamera(CSharedCameraRef camera);

    void addModel(CSharedModelRef model);
    void removeModel(CSharedModelRef model);
    
    void setOcean(CSharedOceanRef ocean);
    void removeOcean(CSharedOceanRef ocean);
    
    void setSkyBox(CSharedSkyBoxRef skybox);
    void removeSkyBox(CSharedSkyBoxRef skybox);
    
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
};

#endif 
