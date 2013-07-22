//
//  CMainMenuScene.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/22/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CMainMenuScene_h
#define CMainMenuScene_h

#include "HCommon.h"
#include "ICollisionHandler.h"

class IGameTransition;
class IGameObject;
class CModel;

class CMainMenuScene final : public std::enable_shared_from_this<CMainMenuScene>, public ICollisionHandler 
{
private:
    
    std::shared_ptr<CModel> m_model_01;
    std::shared_ptr<CModel> m_model_02;
    std::shared_ptr<CModel> m_model_03;
    
    std::vector<std::shared_ptr<IGameObject> > m_collisionTargets;
    
    IGameTransition* m_root;
    
protected:
    
    std::vector<std::shared_ptr<IGameObject> > _OnGetTargets(void);
    void _OnCollision(const glm::vec3& _position, std::shared_ptr<IGameObject> _target);
    
public:
    
    CMainMenuScene(IGameTransition* _root);
    ~CMainMenuScene(void);
    
    void Load(void);
    void Update(f32 _deltatime);
};

#endif 
