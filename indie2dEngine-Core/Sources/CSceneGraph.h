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

class CCamera;
class CLight;
class CSprite;
class CBillboard;
class CParticleEmitter;
class CRenderMgr;
class CSceneUpdateMgr;
class CCollisionMgr;

class CSceneGraph 
{
private:
    
    std::shared_ptr<CCamera> m_camera;
    std::shared_ptr<CLight> m_light;
    
    std::set<std::shared_ptr<CSprite> > m_spritesContainer;
    std::set<std::shared_ptr<CBillboard> > m_billboardsContainer;
    std::set<std::shared_ptr<CParticleEmitter> > m_particlesContainer;
    
protected:
    
    std::shared_ptr<CRenderMgr> m_renderMgr;
    std::shared_ptr<CSceneUpdateMgr> m_sceneUpdateMgr;
    std::shared_ptr<CCollisionMgr> m_collisionMgr;
    
    virtual void _Set_RenderMgr(std::shared_ptr<CRenderMgr> _renderMgr) = 0;
    virtual void _Set_SceneUpdateMgr(std::shared_ptr<CSceneUpdateMgr> _sceneUpdateMgr) = 0;
    virtual void _Set_CollisionMgr( std::shared_ptr<CCollisionMgr> _collisionMgr) = 0;
    
public:
    
    CSceneGraph(void);
    virtual ~CSceneGraph(void);
    
    void Set_Camera(std::shared_ptr<CCamera> _camera);
    void Set_Light(std::shared_ptr<CLight> _light);
    
    void InsertSprite(std::shared_ptr<CSprite> _sprite);
    void RemoveSprite(std::shared_ptr<CSprite> _sprite);
    
    void InsertBillboard(std::shared_ptr<CBillboard> _billboard);
    void RemoveBillboard(std::shared_ptr<CBillboard> _billboard);
    
    void InsertParticleEmitter(std::shared_ptr<CParticleEmitter> _particleEmitter);
    void RemoveParticleEmitter(std::shared_ptr<CParticleEmitter> _particleEmitter);
};

#endif 
