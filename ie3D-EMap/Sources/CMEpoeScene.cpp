//
//  CMEpoeScene.cpp
//  ie3D-EMap-OSX
//
//  Created by sergey.sergeev on 7/28/15.
//
//

#include "CMEpoeScene.h"
#include "IGameTransition.h"
#include "IGameObject.h"
#include "CCamera.h"
#include "CGlobalLightSource.h"
#include "CCollisionMgr.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CParticleEmitter.h"
#include "CResourceAccessor.h"
#include "CTexture.h"
#include "CLandscape.h"
#include "CConfigurationAccessor.h"
#include "CCommonOS.h"
#include "ICommand.h"
#include "IUICommands.h"
#include "HUICommands.h"
#include "CMEpoeTransition.h"
#include "CMESceneStage.h"

CMEpoeScene::CMEpoeScene(IGameTransition* root) :
IScene(root),
m_particle(nullptr),
m_previousDraggedPoint(0.0f)
{
    ISharedCommand command = std::make_shared<CCommand<UICommandPOEUpdateConfigurationParticleEmitter::COMMAND>>(std::bind(&CMEpoeScene::onConfigurationUpdated,
                                                                                                            this,
                                                                                                            std::placeholders::_1));
    m_uiToSceneCommands->addCommand(UICommandPOEUpdateConfigurationParticleEmitter::GUID, command);
}

CMEpoeScene::~CMEpoeScene(void)
{
    
}

void CMEpoeScene::load(void)
{
    assert(m_root != nullptr);
    m_camera = m_root->createCamera(45.0, 0.01, 1024.0,
                                    glm::ivec4(0, 0,
                                               m_root->getScreenWidth(),
                                               m_root->getScreenHeight()));
    
    m_camera->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    m_camera->setLookAt(glm::vec3(2.5f, 0.0f, 2.5f));
    m_camera->setDistanceToLookAt(glm::vec3(8.0, 4.0, 8.0));
    m_root->setCamera(m_camera);
    
    m_globalLightSource = m_root->createGlobalLightSource(45.0, 0.01, 1024.0);
    m_root->setGlobalLightSource(m_globalLightSource);
    
    m_root->addCollisionHandler(shared_from_this());
    
    std::shared_ptr<COcean> ocean = m_root->createOcean("gameobject.ocean.xml");
    m_root->setOcean(ocean);
    
    m_skybox = m_root->createSkybox("gameobject.skybox.xml");
    m_root->setSkybox(m_skybox);
    
    m_particle = m_root->createParticleEmitter("gameobject.particle.emitter.xml");
    m_particle->setPosition(glm::vec3(2.5f, 0.0f, 2.5f));
    m_root->addParticleEmitter(m_particle);
    m_particle->addConfigurationLoadedCallback(std::bind(&CMEpoeScene::onConfigurationLoaded, this, std::placeholders::_1));
    
    m_globalLightSource->setAngle(3.0);
    m_globalLightSource->setDistanceToSun(512.0);
    m_globalLightSource->setDistanceToLookAt(8.0);
    m_globalLightSource->setRotationCenter(glm::vec3(2.5f, 0.0f, 2.5f));
    m_globalLightSource->setLookAt(glm::vec3(2.5f, 0.0f, 2.5f));
    
    CMEpoeTransition* root = static_cast<CMEpoeTransition *>(m_root);
    m_stage = root->createSceneStage("gameobject.scene.stage.xml");
    root->addCustomGameObject(m_stage);
    m_stage->setPosition(glm::vec3(2.5f, -0.5f, 2.5f));
    m_stage->setScale(glm::vec3(5.0, 1.0, 5.0));
    
    m_root->addGestureRecognizerHandler(std::static_pointer_cast<IGestureRecognizerHandler>(shared_from_this()));
}

void CMEpoeScene::update(f32)
{
    
}

void CMEpoeScene::onGestureRecognizerPressed(const glm::ivec2& point, E_INPUT_BUTTON)
{
    m_previousDraggedPoint = point;
}

void CMEpoeScene::onGestureRecognizerMoved(const glm::ivec2&)
{
    
}

void CMEpoeScene::onGestureRecognizerDragged(const glm::ivec2& point, E_INPUT_BUTTON inputButton)
{
    glm::vec2 draggingDelta = glm::vec2(std::abs(m_previousDraggedPoint.x - point.x),
                                        std::abs(m_previousDraggedPoint.y - point.y));
    
    if(inputButton == E_INPUT_BUTTON_MOUSE_LEFT && m_particle)
    {
        glm::vec3 position = m_particle->getPosition();
        
        if(m_previousDraggedPoint.x > point.x)
        {
            position.z -= 0.01f * draggingDelta.x;
        }
        if(m_previousDraggedPoint.x < point.x)
        {
            position.z += 0.01f * draggingDelta.x;
        }
        if(m_previousDraggedPoint.y > point.y)
        {
            position.x += 0.01f * draggingDelta.y;
        }
        if(m_previousDraggedPoint.y < point.y)
        {
            position.x -= 0.01f * draggingDelta.y;
        }
        position.z = glm::clamp(position.z, 0.0f, 5.0f);
        position.x = glm::clamp(position.x, 0.0f, 5.0f);
        m_particle->setPosition(position);
        
    }
    else if(inputButton == E_INPUT_BUTTON_MOUSE_RIGHT && m_particle)
    {
        glm::vec3 rotation = m_particle->getRotation();
        if(m_previousDraggedPoint.x > point.x)
        {
            rotation.y -= 0.25f * draggingDelta.x;
        }
        if(m_previousDraggedPoint.x < point.x)
        {
            rotation.y += 0.25f * draggingDelta.x;
        }
        m_particle->setRotation(rotation);
    }
    m_previousDraggedPoint = point;
}

void CMEpoeScene::onGestureRecognizerReleased(const glm::ivec2&, E_INPUT_BUTTON)
{
    
}

void CMEpoeScene::onGestureRecognizerWheelScroll(E_SCROLL_WHEEL_DIRECTION)
{
    
}

void CMEpoeScene::onKeyUp(i32)
{
    
}

void CMEpoeScene::onKeyDown(i32)
{
    
}

void CMEpoeScene::onConfigurationLoaded(ISharedConfigurationRef configuration)
{
    CSharedConfigurationParticleEmitter configurationParticleEmitter = std::static_pointer_cast<CConfigurationParticleEmitter>(configuration);
    assert(m_sceneToUICommands != nullptr);
    if (m_sceneToUICommands != nullptr)
    {
        m_sceneToUICommands->execute<UICommandPOEUpdateConfigurationParticleEmitter::COMMAND>(UICommandPOEUpdateConfigurationParticleEmitter::GUID,
                                                                                              configurationParticleEmitter);
    }
}

void CMEpoeScene::onConfigurationUpdated(CSharedConfigurationParticleEmitterRef configuration)
{
    if(m_particle)
    {
        m_particle->onConfigurationUpdated(configuration);
    }
}
