//
//  CMainMenuScene.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/22/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CMainMenuScene.h"
#include "IGameTransition.h"
#include "IGameObject.h"
#include "CCommonOS.h"
#include "CLight.h"
#include "CModel.h"
#include "CParticleEmitter.h"
#include "CCamera.h"

CMainMenuScene::CMainMenuScene(IGameTransition* _root) :
m_root(_root)
{

}

CMainMenuScene::~CMainMenuScene(void)
{
    
}

void CMainMenuScene::Load(void)
{
    assert(m_root != nullptr);
    std::shared_ptr<CCamera> camera = m_root->CreateCamera(60.0f,
                                                          0.1f,
                                                          128.0f,
                                                          glm::ivec4(0, 0, static_cast<i32>(Get_ScreenWidth()), static_cast<i32>(Get_ScreenHeight())));
    camera->Set_Position(glm::vec3(0.0f, 0.0f, 0.0f));
    camera->Set_LookAt(glm::vec3(0.0f, 4.0f, 0.0f));
    camera->Set_Distance(16.0f);
    camera->Set_Height(16.0f);
    std::shared_ptr<CLight> light = m_root->CreateLight();
    light->Set_Position(glm::vec3(32.0f, 32.0f, 32.0f));
    m_root->Set_Camera(camera);
    m_root->Set_Light(light);
    
    m_model_01 = m_root->CreateModel("model.xml");
    m_model_01->Set_Position(glm::vec3(12.0f, 0.0f, 12.0f));
    m_collisionTargets.push_back(m_model_01);
    m_root->InsertModel(m_model_01);
    
    m_model_02 = m_root->CreateModel("model.xml");
    m_model_02->Set_Position(glm::vec3(12.0f, 0.0f, 30.0f));
    m_collisionTargets.push_back(m_model_02);
    m_root->InsertModel(m_model_02);
    
    
    m_model_03= m_root->CreateModel("model.xml");
    m_model_03->Set_Position(glm::vec3(30.0f, 0.0f, 30.0f));
    m_collisionTargets.push_back(m_model_03);
    m_root->InsertModel(m_model_03);
    
    std::shared_ptr<CParticleEmitter> particleEmitter = m_root->CreateParticleEmitter("particle.emitter.01.xml");
    particleEmitter->Set_Position(glm::vec3(16.0f, 2.0f, 16.0f));
    m_root->InsertParticleEmitter(particleEmitter);
    
    m_root->RegisterCollisionHandler(shared_from_this());
}

void CMainMenuScene::Update(f32 _deltatime)
{
    static float angle = 0.0f;
    //m_model_01->Set_Rotation(glm::vec3(0.0f, angle, 0.0f));
    angle += 1.0f;
}

std::vector<std::shared_ptr<IGameObject> > CMainMenuScene::_OnGetTargets(void)
{
    return m_collisionTargets;
}

void CMainMenuScene::_OnCollision(const glm::vec3& _position, std::shared_ptr<IGameObject> _target)
{
    std::cout<<_target<<std::endl;
}

