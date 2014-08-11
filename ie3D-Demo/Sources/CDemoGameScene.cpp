//
//  CDemoGameScene.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/22/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CDemoGameScene.h"
#include "IEGameTransition.h"
#include "IGameObject.h"
#include "CCommonOS.h"
#include "CLight.h"
#include "CModel.h"
#include "COcean.h"
#include "CLandscape.h"
#include "CSkyBox.h"
#include "CParticleEmitter.h"
#include "CCamera.h"
#include "CMapDragController.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CEComplexModel.h"

CDemoGameScene::CDemoGameScene(IGameTransition* root) :
IScene(root)
{

}

CDemoGameScene::~CDemoGameScene(void)
{
    
}

void CDemoGameScene::load(void)
{
    assert(m_root != nullptr);
    m_camera = m_root->createCamera(60.0, 0.1, 1024.0,
                                    glm::ivec4(0, 0,
                                               m_root->getScreenWidth(),
                                               m_root->getScreenHeight()));
    
    m_camera->Set_Position(glm::vec3(0.0f, 0.0f, 0.0f));
    m_camera->Set_LookAt(glm::vec3(12.0f, 4.0f, 12.0f));
    m_camera->Set_Distance(32.0f);
    m_camera->Set_Height(32.0f);
    
    m_root->setCamera(m_camera);
    
    std::shared_ptr<COcean> ocean = m_root->createOcean("gameobject.ocean.xml");
    m_root->setOcean(ocean);
    ocean->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    
    m_skyBox = m_root->createSkyBox("gameobject.skybox.xml");
    m_root->setSkyBox(m_skyBox);
    
    std::shared_ptr<CParticleEmitter> particleEmitter = m_root->createParticleEmitter("gameobject.particle.emitter.xml");
    particleEmitter->setPosition(glm::vec3(12.0f, 2.0f, 12.0f));
    
    m_particles.push_back(particleEmitter);
    m_root->addParticleEmitter(particleEmitter);
    
    m_landscape = m_root->createLandscape("gameobject.landscape.xml");
    m_root->setLandscape(m_landscape);
    
    m_model = m_root->createModel("gameobject.model.xml");
    m_root->addModel(m_model);
    m_model->setScale(glm::vec3(10.0, 10.0, 10.0));
    
    IEGameTransition* transition = static_cast<IEGameTransition*>(m_root);
    
    m_lightTank = transition->createComplexModel("gameobject.tank.light.xml");
    m_root->addCustomGameObject(m_lightTank);
    m_lightTank->setScale(glm::vec3(5.0, 5.0, 5.0));
    
    m_mediumTank = transition->createComplexModel("gameobject.tank.medium.xml");
    m_root->addCustomGameObject(m_mediumTank);
    m_mediumTank->setScale(glm::vec3(6.0, 6.0, 6.0));
    m_mediumTank->setPosition(glm::vec3(0.0, 0.0, 32.0));
    
    m_heavyTank = transition->createComplexModel("gameobject.tank.heavy.xml");
    m_root->addCustomGameObject(m_heavyTank);
    m_heavyTank->setScale(glm::vec3(7.0, 7.0, 7.0));
    m_heavyTank->setPosition(glm::vec3(0.0, 0.0, 64.0));
    
    m_root->addCollisionHandler(shared_from_this());
    
    m_mapDragController = std::make_shared<CMapDragController>(m_camera, 0.1,
                                                               glm::vec3(0.0, 0.0, 0.0),
                                                               glm::vec3(512.0, 0.0, 512.0));
    m_root->addGestureRecognizerHandler(m_mapDragController);
}

void CDemoGameScene::update(f32 deltatime)
{
    m_mapDragController->update(deltatime);
    static f32 angle = 0.0;
    angle += 0.1;
    m_skyBox->setRotation(glm::vec3(0.0, angle, 0.0));
    
    m_lightTank->setRotation(glm::vec3(0.0, angle * 10.0, 0.0));
    m_mediumTank->setRotation(glm::vec3(0.0, angle * 10.0, 0.0));
    m_heavyTank->setRotation(glm::vec3(0.0, angle * 10.0, 0.0));
    
    glm::vec3 position = m_camera->Get_LookAt();
    position.y = m_landscape->getHeight(position);
    m_model->setPosition(position);
}

void CDemoGameScene::onCollision(const glm::vec3& position, ISharedGameObjectRef gameObject)
{
    
}

