//
//  CKOTHInGameScene.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/22/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CKOTHInGameScene_h
#define CKOTHInGameScene_h

#include "HCommon.h"
#include "IScene.h"


class IGameTransition;
class IGameObject;
class CModel;
class CLight;
class CCamera;

class CKOTHInGameScene final : public IScene
{
private:
    
protected:
    
    void _OnCollision(const glm::vec3& _position, std::shared_ptr<IGameObject> _collider);
    
public:
    
    CKOTHInGameScene(IGameTransition* _root);
    ~CKOTHInGameScene(void);
    
    void Load(void);
    void Update(f32 _deltatime);
};

#endif 
