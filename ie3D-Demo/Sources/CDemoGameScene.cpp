//
//  CDemoGameScene.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/22/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CDemoGameScene.h"
#include "IGameTransition.h"
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
    m_camera = m_root->CreateCamera(45.0,
                                    0.1,
                                    1024.0,
                                    glm::ivec4(0, 0,
                                               m_root->getWindowWidth(),
                                               m_root->getWindowHeight()));
    
    m_camera->Set_Position(glm::vec3(0.0f, 0.0f, 0.0f));
    m_camera->Set_LookAt(glm::vec3(12.0f, 4.0f, 12.0f));
    m_camera->Set_Distance(32.0f);
    m_camera->Set_Height(32.0f);
    
    m_root->Set_Camera(m_camera);
    
    std::shared_ptr<COcean> ocean = m_root->CreateOcean("gameobject.ocean.xml");
    m_root->InsertOcean(ocean);
    ocean->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    
    m_skyBox = m_root->createSkyBox("gameobject.skybox.xml");
    m_root->InsertSkyBox(m_skyBox);
    
    std::shared_ptr<CParticleEmitter> particleEmitter = m_root->CreateParticleEmitter("gameobject.particle.emitter.xml");
    particleEmitter->setPosition(glm::vec3(12.0f, 2.0f, 12.0f));
    
    m_particles.push_back(particleEmitter);
    m_root->InsertParticleEmitter(particleEmitter);
    
    std::shared_ptr<CLandscape> landscape = m_root->CreateLandscape("gameobject.landscape.xml");
    m_root->InsertLandscape(landscape);
    
    /*for(ui32 i = 0; i < landscape->getChunks().size(); ++i)
    {
        m_colliders.push_back(landscape->getChunks().at(i));
    }*/
    
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
}

void CDemoGameScene::onCollision(const glm::vec3& position, ISharedGameObjectRef gameObject)
{
    
}

