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

class CCamera;
class CLight;
class IGameObject;
class CSprite;
class CModel;
class COcean;
class CLandscape;
class CBillboard;
class CParticleEmitter;
class CRenderMgr;
class CSceneUpdateMgr;
class CCollisionMgr;
class ICollisionHandler;

class CSceneGraph : public virtual IGraph
{
private:
    
    std::shared_ptr<CCamera> m_camera;
    //std::array<std::shared_ptr<CLight>, E_LIGHT_MAX> m_lights;
    
    std::set<std::shared_ptr<IGameObject> > m_gameObjectsContainer;
    
    std::set<std::shared_ptr<CSprite> > m_spritesContainer;
    std::set<std::shared_ptr<CBillboard> > m_billboardsContainer;
    std::set<std::shared_ptr<CParticleEmitter> > m_particlesContainer;
    std::set<std::shared_ptr<CModel> > m_modelsContainer;
    std::shared_ptr<COcean> m_oceansContainer;
    std::shared_ptr<CLandscape> m_landscapeContainer;
    
protected:
    
    std::shared_ptr<CCollisionMgr> m_collisionMgr;
    std::shared_ptr<IInputContext> m_inputContext;
    
    void _InsertGameObject(const std::shared_ptr<IGameObject>& _gameObject);
    void _RemoveGameObject(const std::shared_ptr<IGameObject>& _gameObject);
    
public:
    
    CSceneGraph(void);
    virtual ~CSceneGraph(void);
    
    void Set_Camera(const std::shared_ptr<CCamera>& _camera);
    //void Set_Light(const std::shared_ptr<CLight>& _light, E_LIGHTS _id);
    
    void InsertSprite(const std::shared_ptr<CSprite>& _sprite);
    void RemoveSprite(const std::shared_ptr<CSprite>& _sprite);
    
    void InsertModel(const std::shared_ptr<CModel>& _model);
    void RemoveModel(const std::shared_ptr<CModel>& _model);
    
    void InsertOcean(const std::shared_ptr<COcean>& _ocean);
    void RemoveOcean(const std::shared_ptr<COcean>& _ocean);
    
    void InsertSkyBox(CSharedSkyBoxRef skybox);
    void RemoveSkyBox(CSharedSkyBoxRef skybox);
    
    void InsertLandscape(const std::shared_ptr<CLandscape>& _landscape);
    void RemoveLandscape(const std::shared_ptr<CLandscape>& _landscape);
    
    void InsertBillboard(const std::shared_ptr<CBillboard>& _billboard);
    void RemoveBillboard(const std::shared_ptr<CBillboard>& _billboard);
    
    void InsertParticleEmitter(const std::shared_ptr<CParticleEmitter>& _particleEmitter);
    void RemoveParticleEmitter(const std::shared_ptr<CParticleEmitter>& _particleEmitter);
    
    void addGestureRecognizerHandler(ISharedGestureRecognizerHandlerRef handler);
    void removeGestureRecognizerHandler(ISharedGestureRecognizerHandlerRef handler);
    
    void addCollisionHandler(ISharedCollisionHandlerRef handler);
    void removeCollisionHandler(ISharedCollisionHandlerRef handler);
};

#endif 
