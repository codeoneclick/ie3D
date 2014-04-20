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
#include "HDeclaration.h"

class IGameTransition;
class IGameObject;
class CModel;
class CLight;
class CCamera;
class CNavigator;
class CCharacterController;
class CMoveControllerRecognizer;

class CKOTHInGameScene final : public IScene
{
private:
    
protected:
    
    std::shared_ptr<CMoveControllerRecognizer> m_moveControllerRecognizer;
    std::shared_ptr<CCharacterController> m_characterController;
    std::shared_ptr<CNavigator> m_navigator;
    
    void _OnCollision(const glm::vec3& _position, std::shared_ptr<IGameObject> _collider);
    
public:
    
    CKOTHInGameScene(IGameTransition* _root);
    ~CKOTHInGameScene(void);
    
    void Load(void);
    void OnUpdate(f32 _deltatime);
};

#endif 
