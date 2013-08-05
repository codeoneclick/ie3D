//
//  CKOTHInGameScene.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/22/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CKOTHInGameScene.h"
#include "IGameTransition.h"
#include "IGameObject.h"
#include "CCommonOS.h"
#include "CLight.h"
#include "CModel.h"
#include "COcean.h"
#include "CParticleEmitter.h"
#include "CCamera.h"
#include "CNavigator.h"
#include "CCharacterController.h"

extern void RegisterMoveControllerHandler(std::shared_ptr<IMoveControllerHandler> _handler);
extern void UnregisterMoveControllerHandler(std::shared_ptr<IMoveControllerHandler> _handler);

CKOTHInGameScene::CKOTHInGameScene(IGameTransition* _root) :
IScene(_root),
m_navigator(nullptr),
m_characterController(nullptr)
{
    std::cout<<_root<<std::endl;
}

CKOTHInGameScene::~CKOTHInGameScene(void)
{
    
}

void CKOTHInGameScene::Load(void)
{
    assert(m_root != nullptr);
    m_camera = m_root->CreateCamera(60.0f,
                                    0.1f,
                                    128.0f,
                                    glm::ivec4(0, 0, static_cast<i32>(Get_ScreenWidth()), static_cast<i32>(Get_ScreenHeight())));
    m_camera->Set_Position(glm::vec3(0.0f, 0.0f, 0.0f));
    m_camera->Set_LookAt(glm::vec3(12.0f, 4.0f, 12.0f));
    m_camera->Set_Distance(24.0f);
    m_camera->Set_Height(24.0f);
    
    m_light = m_root->CreateLight();
    m_light->Set_Position(glm::vec3(32.0f, 32.0f, 32.0f));
    
    m_root->Set_Camera(m_camera);
    m_root->Set_Light(m_light);
    
    std::shared_ptr<CModel> model = m_root->CreateModel("model.Footman.xml");
    model->Set_Position(glm::vec3(0.0f, 0.0f, 0.0f));
    //model->Set_Scale(glm::vec3(0.25f, 0.25f, 0.25f));
    m_models.push_back(model);
    m_colliders.push_back(model);
    m_root->InsertModel(model);
    
    for(i32 i = 0; i < 2; ++i)
    {
        for(i32 j = 0; j < 2; ++j)
        {
            std::shared_ptr<CModel> model = m_root->CreateModel("model.Building.xml");
            model->Set_Position(glm::vec3(i * 10, 0.0f, j * 10));
            m_models.push_back(model);
            m_colliders.push_back(model);
            m_root->InsertModel(model);
        }
    }
    
    std::shared_ptr<COcean> ocean = m_root->CreateOcean("ocean.xml");
    m_root->InsertOcean(ocean);
    
    std::shared_ptr<CParticleEmitter> particleEmitter = m_root->CreateParticleEmitter("particle.emitter.01.xml");
    particleEmitter->Set_Position(glm::vec3(12.0f, 2.0f, 12.0f));
    
    m_particles.push_back(particleEmitter);
    m_root->InsertParticleEmitter(particleEmitter);
    
    m_root->RegisterCollisionHandler(shared_from_this());
    
    m_navigator = std::make_shared<CNavigator>(0.3f, 0.15f, 0.3f, 0.025f);
    m_characterController = std::make_shared<CCharacterController>();
    m_characterController->Set_Camera(m_camera);
    m_characterController->Set_Character(m_models[0]);
    m_characterController->Set_Navigator(m_navigator);
    RegisterMoveControllerHandler(m_characterController);
}

void CKOTHInGameScene::OnUpdate(f32 _deltatime)
{
    static float angle = 0.0f;
    //m_models[0]->Set_Rotation(glm::vec3(0.0f, angle, 0.0f));
    angle += 0.01f;
    
    static glm::vec3 lightPosition = glm::vec3(0.0f);
    lightPosition.x = 12.0f + cosf(-angle / 50.0f ) * -8.0f;
    lightPosition.y = 8.0f;
    lightPosition.z = 12.0f + sinf(-angle / 50.0f) * -8.0f;
    
    m_light->Set_Position(lightPosition);
    
    m_characterController->OnUpdate(_deltatime);
    m_camera->Set_Rotation(angle);
}

void CKOTHInGameScene::_OnCollision(const glm::vec3& _position, std::shared_ptr<IGameObject> _collider)
{
    std::cout<<_collider<<std::endl;
}

