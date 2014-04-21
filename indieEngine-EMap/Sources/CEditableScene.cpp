//
//  CEditableScene.cpp
//  indieEngine-EMap
//
//  Created by Sergey Sergeev on 4/20/14.
//
//

#include "CEditableScene.h"
#include "IGameTransition.h"
#include "IGameObject.h"
#include "CCamera.h"
#include "CLandscape.h"
#include "COcean.h"
#include "CParticleEmitter.h"
#include "CNavigator.h"
#include "CCharacterController.h"
#include "CMoveControllerRecognizer.h"

CEditableScene::CEditableScene(IGameTransition* root) :
IScene(root)
{
    
}

CEditableScene::~CEditableScene(void)
{
    
}

void CEditableScene::load(void)
{
    assert(m_root != nullptr);
    m_camera = m_root->CreateCamera(60.0f,
                                    0.1f,
                                    256.0f,
                                    glm::ivec4(0, 0,
                                               m_root->getWindowWidth(),
                                               m_root->getWindowHeight()));
    
    m_camera->Set_Position(glm::vec3(0.0f, 0.0f, 0.0f));
    m_camera->Set_LookAt(glm::vec3(12.0f, 4.0f, 12.0f));
    m_camera->Set_Distance(32.0f);
    m_camera->Set_Height(32.0f);
    
    m_root->Set_Camera(m_camera);
    
    std::shared_ptr<COcean> ocean = m_root->CreateOcean("ocean.xml");
    m_root->InsertOcean(ocean);
    ocean->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    
    std::shared_ptr<CParticleEmitter> particleEmitter = m_root->CreateParticleEmitter("particle.emitter.01.xml");
    particleEmitter->setPosition(glm::vec3(12.0f, 2.0f, 12.0f));
    
    m_particles.push_back(particleEmitter);
    m_root->InsertParticleEmitter(particleEmitter);
    
    std::shared_ptr<CLandscape> landscape = m_root->CreateLandscape("landscape.xml");
    m_root->InsertLandscape(landscape);
    
    m_navigator = std::make_shared<CNavigator>(0.75f, 0.5f, 0.75f, 0.025f);
    m_characterController = std::make_shared<CCharacterController>();
    m_characterController->Set_Camera(m_camera);
    m_characterController->Set_Character(particleEmitter);
    m_characterController->Set_Navigator(m_navigator);
	m_characterController->Set_Position(glm::vec3(24.0f, 0.0f, 24.0f));
    
    m_moveControllerRecognizer = std::make_shared<CMoveControllerRecognizer>();
    m_root->addGestureRecognizerHandler(m_moveControllerRecognizer);
    m_moveControllerRecognizer->RegisterMoveControllerHandler(m_characterController);
}

void CEditableScene::update(f32 deltatime)
{
    m_characterController->OnUpdate(deltatime);
}

void CEditableScene::_OnCollision(const glm::vec3& position,
                                  std::shared_ptr<IGameObject> collider)
{
    
}

