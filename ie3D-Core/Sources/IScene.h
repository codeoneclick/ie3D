//
//  IScene.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/23/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IScene_h
#define IScene_h

#include "HCommon.h"
#include "CCollisionMgr.h"

class CCamera;
class CLight;
class IGameObject;
class CModel;
class CParticleEmitter;
class IGameTransition;

class IScene : public std::enable_shared_from_this<IScene>, public ICollisionHandler
{
private:
    
protected:
    
    IGameTransition* m_root;
    std::shared_ptr<CCamera> m_camera;
    std::vector<std::shared_ptr<CLight>> m_lights;
    std::vector<std::shared_ptr<CModel> > m_models;
    std::vector<std::shared_ptr<CParticleEmitter> > m_particles;
    std::vector<ISharedGameObject> m_colliders;
    
    virtual std::vector<ISharedGameObject> colliders(void);
    virtual void onCollision(const glm::vec3& position, ISharedGameObjectRef gameObject, E_INPUT_BUTTON inputButton);
    
public:
    
    IScene(IGameTransition* root);
    virtual ~IScene(void);
    
    virtual void load(void) = 0;
    virtual void update(f32 deltatime) = 0;
};

#endif 
