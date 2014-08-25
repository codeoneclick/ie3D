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
#include "CDEUIToSceneCommands.h"
#include "CGameObjectNavigator.h"
#include "ICharacterController.h"

CDemoGameScene::CDemoGameScene(IGameTransition* root) :
IScene(root),
m_uiToSceneCommands(std::make_shared<CDEUIToSceneCommands>()),
m_sceneToUICommands(nullptr),
m_gameObjectNavigator(nullptr),
m_characterController(nullptr),
m_characterControllerMoveState(E_CHARACTER_CONTROLLER_MOVE_STATE_NONE),
m_characterControllerSteerState(E_CHARACTER_CONTROLLER_STEER_STATE_NONE)
{
    m_uiToSceneCommands->connectSetCharacterMoveStateCommand(std::bind(&CDemoGameScene::onCharacterMoveStateChanged, this, std::placeholders::_1));
    m_uiToSceneCommands->connectSetCharacterSteerStateCommand(std::bind(&CDemoGameScene::onCharacterSteerStateChanged, this, std::placeholders::_1));
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
    m_camera->Set_Distance(12.0f);
    m_camera->Set_Height(12.0f);
    
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
    m_lightTank->setScale(glm::vec3(2.0, 2.0, 2.0));
    
    m_mediumTank = transition->createComplexModel("gameobject.tank.medium.xml");
    m_root->addCustomGameObject(m_mediumTank);
    m_mediumTank->setScale(glm::vec3(6.0, 6.0, 6.0));
    m_mediumTank->setPosition(glm::vec3(0.0, 0.0, 32.0));
    
    m_heavyTank = transition->createComplexModel("gameobject.tank.heavy.xml");
    m_root->addCustomGameObject(m_heavyTank);
    m_heavyTank->setScale(glm::vec3(7.0, 7.0, 7.0));
    m_heavyTank->setPosition(glm::vec3(0.0, 0.0, 64.0));
    
    m_root->addCollisionHandler(shared_from_this());
    
    m_characterController = std::make_shared<ICharacterController>(m_lightTank,
                                                                   m_camera);
    
    m_gameObjectNavigator = std::make_shared<CGameObjectNavigator>(2.5,
                                                                   2.5,
                                                                   0.0,
                                                                   5.0,
                                                                   m_landscape,
                                                                   glm::vec3(512.0, 0.0, 512.0),
                                                                   glm::vec3(0.0, 0.0, 0.0));
    m_gameObjectNavigator->addNavigatorHandler(m_characterController);
    
    m_gameObjectNavigator->setPosition(glm::vec3(128.0, 0.0, 64.0));
    m_gameObjectNavigator->setRotation(glm::vec3(0.0, 90.0, 0.0));
}

void CDemoGameScene::update(f32 deltatime)
{
    assert(m_characterController != nullptr);
    
    switch (m_characterControllerMoveState)
    {
        case E_CHARACTER_CONTROLLER_MOVE_STATE_NONE:
        {
            m_characterController->decreaseSpeed();
        }
            break;
        case E_CHARACTER_CONTROLLER_MOVE_STATE_FORWARD:
        {
            m_gameObjectNavigator->moveForward();
            m_characterController->increaseSpeed();
        }
            break;
        case E_CHARACTER_CONTROLLER_MOVE_STATE_BACKWARD:
        {
            m_gameObjectNavigator->moveBackward();
            m_characterController->decreaseSpeed();
        }
            break;
        default:
        {
            assert(false);
        }
            break;
    }
    
    switch (m_characterControllerSteerState)
    {
        case E_CHARACTER_CONTROLLER_STEER_STATE_NONE:
            
            break;
        case E_CHARACTER_CONTROLLER_STEER_STATE_LEFT:
        {
            m_gameObjectNavigator->steerLeft();
        }
            break;
        case E_CHARACTER_CONTROLLER_STEER_STATE_RIGHT:
        {
            m_gameObjectNavigator->steerRight();
        }
            break;
        default:
        {
            assert(false);
        }
            break;
    }
    m_characterController->update(deltatime);
}

void CDemoGameScene::onCollision(const glm::vec3& position, ISharedGameObjectRef gameObject)
{
    
}

CDESharedUIToSceneCommands CDemoGameScene::getUIToSceneCommands(void) const
{
    assert(m_uiToSceneCommands != nullptr);
    return m_uiToSceneCommands;
}

void CDemoGameScene::setSceneToUICommands(CDESharedSceneToUICommandsRef commands)
{
    m_sceneToUICommands = commands;
}

void CDemoGameScene::onCharacterMoveStateChanged(E_CHARACTER_CONTROLLER_MOVE_STATE state)
{
    m_characterControllerMoveState = state;
}

void CDemoGameScene::onCharacterSteerStateChanged(E_CHARACTER_CONTROLLER_STEER_STATE state)
{
    m_characterControllerSteerState = state;
}
