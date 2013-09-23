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
#include "ICollisionHandler.h"

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
    std::vector<std::shared_ptr<IGameObject> > m_colliders;
    
    std::vector<std::shared_ptr<IGameObject> > _OnGetColliders(void);
    virtual void _OnCollision(const glm::vec3& _position, std::shared_ptr<IGameObject> _collider) = 0;
    
public:
    
    IScene(IGameTransition* _root);
    virtual ~IScene(void);
    
    virtual void Load(void) = 0;
    virtual void OnUpdate(f32 _deltatime) = 0;
};

#endif 
