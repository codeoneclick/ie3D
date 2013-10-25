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

class CCamera;
class CLight;
class IGameObject;
class CSprite;
class CModel;
class COcean;
class CBillboard;
class CParticleEmitter;
class CRenderMgr;
class CSceneUpdateMgr;
class CCollisionMgr;
class ICollisionHandler;
class IInputContext;

class CSceneGraph : public virtual IGraph
{
private:
    
    std::shared_ptr<CCamera> m_camera;
    std::shared_ptr<CLight> m_lights[E_LIGHT_MAX];
    
    std::set<std::shared_ptr<IGameObject> > m_gameObjectsContainer;
    
    std::set<std::shared_ptr<CSprite> > m_spritesContainer;
    std::set<std::shared_ptr<CBillboard> > m_billboardsContainer;
    std::set<std::shared_ptr<CParticleEmitter> > m_particlesContainer;
    std::set<std::shared_ptr<CModel> > m_modelsContainer;
    std::set<std::shared_ptr<COcean> > m_oceansContainer;
    
protected:
    
    std::shared_ptr<CCollisionMgr> m_collisionMgr;
    std::shared_ptr<IInputContext> m_inputContext;
    
    void _InsertGameObject(std::shared_ptr<IGameObject> _gameObject);
    void _RemoveGameObject(std::shared_ptr<IGameObject> _gameObject);
    
public:
    
    CSceneGraph(void);
    virtual ~CSceneGraph(void);
    
    void Set_Camera(std::shared_ptr<CCamera> _camera);
    void Set_Light(std::shared_ptr<CLight> _light, E_LIGHTS _id);
    
    void InsertSprite(std::shared_ptr<CSprite> _sprite);
    void RemoveSprite(std::shared_ptr<CSprite> _sprite);
    
    void InsertModel(std::shared_ptr<CModel> _model);
    void RemoveModel(std::shared_ptr<CModel> _model);
    
    void InsertOcean(std::shared_ptr<COcean> _ocean);
    void RemoveOcean(std::shared_ptr<COcean> _ocean);
    
    void InsertBillboard(std::shared_ptr<CBillboard> _billboard);
    void RemoveBillboard(std::shared_ptr<CBillboard> _billboard);
    
    void InsertParticleEmitter(std::shared_ptr<CParticleEmitter> _particleEmitter);
    void RemoveParticleEmitter(std::shared_ptr<CParticleEmitter> _particleEmitter);
    
    void RegisterCollisionHandler(std::shared_ptr<ICollisionHandler> _handler);
    void UnregisterCollisionHandler(std::shared_ptr<ICollisionHandler> _handler);
};

#endif 
