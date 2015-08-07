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
#include "CGlobalLightSource.h"
#include "CModel.h"
#include "COcean.h"
#include "CLandscape.h"
#include "CSkyBox.h"
#include "CParticleEmitter.h"
#include "CAtmosphericScattering.h"
#include "CCamera.h"
#include "CMapDragController.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CEComplexModel.h"
#include "CDEUIToSceneCommands.h"
#include "CGameObjectNavigator.h"
#include "ICharacterController.h"

#define key_z 122
#define key_x 120

CDemoGameScene::CDemoGameScene(IGameTransition* root) :
IScene(root),
m_uiToSceneCommands(std::make_shared<CDEUIToSceneCommands>()),
m_sceneToUICommands(nullptr),
m_characterControllerMoveState(E_CHARACTER_CONTROLLER_MOVE_STATE_NONE),
m_characterControllerSteerState(E_CHARACTER_CONTROLLER_STEER_STATE_NONE)
{
    m_uiToSceneCommands->connectSetCharacterMoveStateCommand(std::bind(&CDemoGameScene::onCharacterMoveStateChanged, this, std::placeholders::_1));
    m_uiToSceneCommands->connectSetCharacterSteerStateCommand(std::bind(&CDemoGameScene::onCharacterSteerStateChanged, this, std::placeholders::_1));
    
    m_uiToSceneCommands->connectOnKeyDownCommand(std::bind(&CDemoGameScene::onKeyDown, this, std::placeholders::_1));
    m_uiToSceneCommands->connectOnKeyUpCommand(std::bind(&CDemoGameScene::onKeyUp, this, std::placeholders::_1));
}

CDemoGameScene::~CDemoGameScene(void)
{
    
}

void CDemoGameScene::load(void)
{
    assert(m_root != nullptr);
    m_camera = m_root->createCamera(45.0, 0.1, 1024.0,
                                    glm::ivec4(0, 0,
                                               m_root->getScreenWidth(),
                                               m_root->getScreenHeight()));
    
    m_camera->setPosition(glm::vec3(0.0f));
    m_camera->setDistanceToLookAt(glm::vec3(32.0f));
    
    m_root->setCamera(m_camera);
    
    m_globalLightSource = m_root->createGlobalLightSource(45.0 , 0.1, 1024.0);
    m_root->setGlobalLightSource(m_globalLightSource);
    
    std::shared_ptr<COcean> ocean = m_root->createOcean("gameobject.ocean.xml");
    m_root->setOcean(ocean);
    ocean->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    
    m_skybox = m_root->createSkybox("gameobject.skybox.xml");
    m_root->setSkybox(m_skybox);
    
    //std::shared_ptr<CParticleEmitter> particleEmitter = m_root->createParticleEmitter("gameobject.particle.emitter.xml");
    //particleEmitter->setPosition(glm::vec3(12.0f, 2.0f, 12.0f));
    //m_root->addParticleEmitter(particleEmitter);
    
    m_landscape = m_root->createLandscape("gameobject.landscape.xml");
    m_root->setLandscape(m_landscape);
    
    m_models["model_01"] = m_root->createModel("gameobject.human_02.xml");
    m_root->addModel(m_models["model_01"]);
    m_models["model_01"]->setScale(glm::vec3(2.0));
    
    m_models["model_02"] = m_root->createModel("gameobject.orc_01.xml");
    m_root->addModel(m_models["model_02"]);
    m_models["model_02"]->setPosition(glm::vec3(6.0,
                                                m_landscape->getHeight(glm::vec3(6.0, 0.0, 36.0)),
                                                36.0));
    m_models["model_02"]->setScale(glm::vec3(2.0));
    
    m_models["model_03"] = m_root->createModel("gameobject.orc_02.xml");
    m_root->addModel(m_models["model_03"]);
    m_models["model_03"]->setPosition(glm::vec3(12.0,
                                                m_landscape->getHeight(glm::vec3(12.0, 0.0, 36.0)),
                                                36.0));
    m_models["model_03"]->setScale(glm::vec3(2.0));
    
    m_models["model_04"] = m_root->createModel("gameobject.human_01.xml");
    m_root->addModel(m_models["model_04"]);
    m_models["model_04"]->setPosition(glm::vec3(8.0,
                                                m_landscape->getHeight(glm::vec3(8.0, 0.0, 42.0)),
                                                42.0));
    m_models["model_04"]->setScale(glm::vec3(2.0));
    m_models["model_04"]->setRotation(glm::vec3(0.0, 180.0, 0.0));
    
    m_characterControllers["model_01"] = std::make_shared<ICharacterController>(m_models["model_01"],
                                                                                m_landscape,
                                                                                m_camera);
    
    m_navigators["model_01"] = std::make_shared<CGameObjectNavigator>(20.0f,
                                                                      10.0f,
                                                                      0.0,
                                                                      2.5,
                                                                      m_landscape,
                                                                      glm::vec3(512.0, 0.0, 512.0),
                                                                      glm::vec3(0.0, 0.0, 0.0));
    m_navigators["model_01"]->addNavigatorHandler(m_characterControllers["model_01"]);
    
    m_navigators["model_01"]->setPosition(glm::vec3(2.0, 0.0, 2.0));
    m_navigators["model_01"]->setRotation(glm::vec3(0.0, 0.0, 0.0));
    
    m_characterControllers["model_02"] = std::make_shared<ICharacterController>(m_models["model_02"],
                                                                                m_landscape);
    
    m_navigators["model_02"] = std::make_shared<CGameObjectNavigator>(20.0f,
                                                                      10.0f,
                                                                      0.0,
                                                                      2.5,
                                                                      m_landscape,
                                                                      glm::vec3(512.0, 0.0, 512.0),
                                                                      glm::vec3(0.0, 0.0, 0.0));
    m_navigators["model_02"]->addNavigatorHandler(m_characterControllers["model_02"]);
    
    m_navigators["model_02"]->setPosition(glm::vec3(6.0, 0.0, 36.0));
    m_navigators["model_02"]->setRotation(glm::vec3(0.0, 0.0, 0.0));
    
    m_globalLightSource->setAngle(3.0);
    m_globalLightSource->setDistanceToSun(512.0);
    m_globalLightSource->setDistanceToLookAt(64.0);
    m_globalLightSource->setRotationCenter(glm::vec3(256.0, 0.0, 256.0));
    m_globalLightSource->setLookAt(m_models["model_01"]->getPosition());
    
    m_root->setBox2dScene(glm::vec2(0.0f), glm::vec2(512.0f));
    
    m_models["model_01"]->addResourceLoadingCommand([this](ISharedResourceRef resource) {
        if(resource->getResourceClass() == E_RESOURCE_CLASS_MESH)
        {
            m_root->addBox2dCollider(std::static_pointer_cast<IBox2dCollider>(m_navigators["model_01"]), false);
        }
    });
    
    m_models["model_02"]->addResourceLoadingCommand([this](ISharedResourceRef resource) {
        if(resource->getResourceClass() == E_RESOURCE_CLASS_MESH)
        {
            m_root->addBox2dCollider(std::static_pointer_cast<IBox2dCollider>(m_navigators["model_02"]), false);
        }
    });
}

void CDemoGameScene::update(f32 deltatime)
{
    assert(m_characterControllers["model_01"]);
    switch (m_characterControllerMoveState)
    {
        case E_CHARACTER_CONTROLLER_MOVE_STATE_NONE:
        {
            m_characterControllers["model_01"]->decreaseSpeed();
            m_models["model_01"]->setAnimation("IDLE");
        }
            break;
        case E_CHARACTER_CONTROLLER_MOVE_STATE_FORWARD:
        {
            m_navigators["model_01"]->moveForward();
            m_characterControllers["model_01"]->increaseSpeed();
            m_models["model_01"]->setAnimation("RUN");
        }
            break;
        case E_CHARACTER_CONTROLLER_MOVE_STATE_BACKWARD:
        {
            m_navigators["model_01"]->moveBackward();
            m_characterControllers["model_01"]->decreaseSpeed();
            m_models["model_01"]->setAnimation("RUN");
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
            m_navigators["model_01"]->steerLeft();
        }
            break;
        case E_CHARACTER_CONTROLLER_STEER_STATE_RIGHT:
        {
            m_navigators["model_01"]->steerRight();
        }
            break;
        default:
        {
            assert(false);
        }
            break;
    }
    
    m_navigators["model_01"]->update(deltatime);
    m_characterControllers["model_01"]->update(deltatime);
    
    m_navigators["model_02"]->update(deltatime);
    m_characterControllers["model_02"]->update(deltatime);
    
    static f32 angle = 0.0;
    angle += 0.033;
    m_skybox->setRotation(glm::vec3(0.0, angle, 0.0));
    m_globalLightSource->setLookAt(m_models["model_01"]->getPosition());
    
    m_models["model_02"]->setAnimation("IDLE");
    m_models["model_03"]->setAnimation("IDLE");
    m_models["model_04"]->setAnimation("IDLE");
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

void CDemoGameScene::onKeyDown(i32 key)
{
    static f32 angle = 3.0;
    if(key == key_z)
    {
        angle += 0.05;
    }
    else if(key == key_x)
    {
        angle -= 0.05;
    }
    m_globalLightSource->setAngle(angle);
}

void CDemoGameScene::onKeyUp(i32 key)
{
    
}
