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
#include "CParticleEmitter.h"
#include "CCamera.h"

CKOTHInGameScene::CKOTHInGameScene(IGameTransition* _root) :
IScene(_root)
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
    m_camera->Set_Distance(16.0f);
    m_camera->Set_Height(16.0f);
    
    m_light = m_root->CreateLight();
    m_light->Set_Position(glm::vec3(32.0f, 32.0f, 32.0f));
    
    m_root->Set_Camera(m_camera);
    m_root->Set_Light(m_light);
    
    std::shared_ptr<CModel> model_01 = m_root->CreateModel("model.xml");
    model_01->Set_Position(glm::vec3(12.0f, 0.0f, 12.0f));
    
    m_models.push_back(model_01);
    m_colliders.push_back(model_01);
    m_root->InsertModel(model_01);
    
    std::shared_ptr<CModel> model_02 = m_root->CreateModel("model.xml");
    model_02->Set_Position(glm::vec3(12.0f, 0.0f, 30.0f));
    
    m_models.push_back(model_02);
    m_colliders.push_back(model_02);
    m_root->InsertModel(model_02);
    
    std::shared_ptr<CModel> model_03= m_root->CreateModel("model.xml");
    model_03->Set_Position(glm::vec3(30.0f, 0.0f, 30.0f));
    
    m_models.push_back(model_03);
    m_colliders.push_back(model_03);
    m_root->InsertModel(model_03);
    
    std::shared_ptr<CParticleEmitter> particleEmitter = m_root->CreateParticleEmitter("particle.emitter.01.xml");
    particleEmitter->Set_Position(glm::vec3(12.0f, 2.0f, 12.0f));
    
    m_particles.push_back(particleEmitter);
    m_root->InsertParticleEmitter(particleEmitter);
    
    m_root->RegisterCollisionHandler(shared_from_this());
}

void CKOTHInGameScene::Update(f32 _deltatime)
{
    static float angle = 0.0f;
    m_models[0]->Set_Rotation(glm::vec3(0.0f, angle, 0.0f));
    angle += 1.0f;
    
    static glm::vec3 lightPosition = glm::vec3(0.0f);
    lightPosition.x = 12.0f + cosf(-angle / 50.0f ) * -8.0f;
    lightPosition.y = 8.0f;
    lightPosition.z = 12.0f + sinf(-angle / 50.0f) * -8.0f;
    
    m_light->Set_Position(lightPosition);
}

void CKOTHInGameScene::_OnCollision(const glm::vec3& _position, std::shared_ptr<IGameObject> _target)
{
    std::cout<<_target<<std::endl;
}

