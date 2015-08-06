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
#include "HDeclaration.h"
#include "CCollisionMgr.h"
#include "IInputContext.h"

class IScene : public std::enable_shared_from_this<IScene>,
public ITouchCollider,
public IGestureRecognizerHandler
{
private:
    
protected:
    
    IGameTransition* m_root;
    CSharedCamera m_camera;
    
    std::vector<ISharedGameObject> m_colliders;
    
    ISharedUICommands m_uiToSceneCommands;
    ISharedUICommands m_sceneToUICommands;
    
    virtual std::vector<ISharedGameObject> colliders(void);
    virtual void onCollision(const glm::vec3& position, ISharedGameObjectRef gameObject, E_INPUT_BUTTON inputButton);
    
public:
    
    IScene(IGameTransition* root);
    virtual ~IScene(void);
    
    virtual void load(void) = 0;
    virtual void update(f32 deltatime) = 0;
    
    void setSceneToUICommands(ISharedUICommandsRef commands);
    ISharedUICommands getUIToSceneCommands(void) const;
};

#endif 
