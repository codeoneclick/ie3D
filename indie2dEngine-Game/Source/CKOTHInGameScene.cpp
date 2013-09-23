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
                                    256.0f,
                                    glm::ivec4(0, 0, static_cast<i32>(Get_ScreenWidth()), static_cast<i32>(Get_ScreenHeight())));
    m_camera->Set_Position(glm::vec3(0.0f, 0.0f, 0.0f));
    m_camera->Set_LookAt(glm::vec3(12.0f, 4.0f, 12.0f));
    m_camera->Set_Distance(32.0f);
    m_camera->Set_Height(32.0f);
    
    std::shared_ptr<CLight> light_01 = m_root->CreateLight();
    m_lights.push_back(light_01);
    
    m_root->Set_Camera(m_camera);
    m_root->Set_Light(light_01, E_LIGHT_01);
    
    std::shared_ptr<CLight> light_02 = m_root->CreateLight();
    m_lights.push_back(light_02);
    
    m_root->Set_Camera(m_camera);
    m_root->Set_Light(light_02, E_LIGHT_02);
    
    std::shared_ptr<CModel> model = m_root->CreateModel("model.Footman.xml");
    model->Set_Position(glm::vec3(0.0f, 0.0f, 0.0f));
    model->Set_Scale(glm::vec3(10.0f, 10.0f, 10.0f));
    m_models.push_back(model);
    m_colliders.push_back(model);
    m_root->InsertModel(model);
    
    for(i32 i = 0; i < 14; ++i)
    {
        for(i32 j = 0; j < 14; ++j)
        {
            if(i == 0 || i == 13 || j == 0 || j == 13)
            {
                std::shared_ptr<CModel> model = m_root->CreateModel("model.Building.xml");
                model->Set_Position(glm::vec3(i * 10, 0.0f, j * 10));
                model->Set_Scale(glm::vec3(10.0f, 10.0f, 10.0f));
                m_models.push_back(model);
                m_colliders.push_back(model);
                m_root->InsertModel(model);
            }
            else if((i > 3 && i < 11) && (j > 3 && j < 11) && (i%2 == 0 || j%2 == 0))
            {
                std::shared_ptr<CModel> model = m_root->CreateModel("model.Building.xml");
                model->Set_Position(glm::vec3(i * 10, -7.5f, j * 10));
                model->Set_Scale(glm::vec3(10.0f, 10.0f, 10.0f));
                m_models.push_back(model);
                m_colliders.push_back(model);
                m_root->InsertModel(model);
            }
        }
    }
    
    std::shared_ptr<COcean> ocean = m_root->CreateOcean("ocean.xml");
    m_root->InsertOcean(ocean);
    
    std::shared_ptr<CParticleEmitter> particleEmitter = m_root->CreateParticleEmitter("particle.emitter.01.xml");
    particleEmitter->Set_Position(glm::vec3(12.0f, 2.0f, 12.0f));
    
    m_particles.push_back(particleEmitter);
    m_root->InsertParticleEmitter(particleEmitter);
    
    m_root->RegisterCollisionHandler(shared_from_this());
    
    m_navigator = std::make_shared<CNavigator>(0.75f, 0.5f, 0.75f, 0.025f);
    m_characterController = std::make_shared<CCharacterController>();
    m_characterController->Set_Camera(m_camera);
    m_characterController->Set_Character(m_models[0]);
    m_characterController->Set_Navigator(m_navigator);
    RegisterMoveControllerHandler(m_characterController);
}

void CKOTHInGameScene::OnUpdate(f32 _deltatime)
{
    static float angle = 0.0f;
    m_models[0]->Set_Animation("model_02.MDL_anim");
    
    for(ui32 i = 1; i < m_models.size(); ++i)
    {
        if(i % 2 == 0)
        {
            m_models[i]->Set_Animation("model_03.MDL_anim");
        }
    }
    angle += 0.033f;
    
    static glm::vec3 lightPosition_01 = glm::vec3(0.0f);
    lightPosition_01.x = 0.0f + cosf(-angle) * -16.0f;
    lightPosition_01.y = 16.0f;
    lightPosition_01.z = 0.0f + sinf(-angle) * -16.0f;
    m_lights[0]->Set_Position(lightPosition_01);

    static glm::vec3 lightPosition_02 = glm::vec3(0.0f);
    lightPosition_02.x = 32.0f + cosf(-angle) * -16.0f;
    lightPosition_02.y = 16.0f;
    lightPosition_02.z = 32.0f + sinf(-angle) * -16.0f;
    m_lights[1]->Set_Position(lightPosition_02);
    
    m_characterController->OnUpdate(_deltatime);
}

void CKOTHInGameScene::_OnCollision(const glm::vec3& _position, std::shared_ptr<IGameObject> _collider)
{
    std::cout<<_collider<<std::endl;
}

