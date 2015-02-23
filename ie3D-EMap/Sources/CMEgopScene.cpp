//
//  CMEgopScene.cpp
//  ie3D-EMap
//
//  Created by sergey.sergeev on 11/3/14.
//
//

#include "CMEgopScene.h"
#include "IGameTransition.h"
#include "IGameObject.h"
#include "CCamera.h"
#include "CGlobalLightSource.h"
#include "CCollisionMgr.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CModel.h"
#include "CResourceAccessor.h"
#include "CTexture.h"
#include "ICommand.h"
#include "IUICommands.h"
#include "HUICommands.h"

CMEgopScene::CMEgopScene(IGameTransition* root) :
IScene(root),
m_model(nullptr)
{
    ISharedCommand command = std::make_shared<CCommand<UICommandGOPSetGameObjectConfigurationName::COMMAND>>(std::bind(&CMEgopScene::setModelConfigurationFilenameCommand,
                                                                                                            this,
                                                                                                            std::placeholders::_1));
    m_uiToSceneCommands->addCommand(UICommandGOPSetGameObjectConfigurationName::GUID, command);
}

CMEgopScene::~CMEgopScene(void)
{
    
}

void CMEgopScene::load(void)
{
    assert(m_root != nullptr);
    m_camera = m_root->createCamera(45.0, 0.01, 1024.0,
                                    glm::ivec4(0, 0,
                                               m_root->getScreenWidth(),
                                               m_root->getScreenHeight()));
    
    m_camera->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    m_camera->setLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
    m_camera->setDistanceToLookAt(glm::vec3(8.0, 4.0, 8.0));
    m_root->setCamera(m_camera);
    
    m_globalLightSource = m_root->createGlobalLightSource(45.0, 0.01, 1024.0);
    m_root->setGlobalLightSource(m_globalLightSource);
    
    m_root->addCollisionHandler(shared_from_this());
    
    m_globalLightSource->setAngle(3.0);
    m_globalLightSource->setDistanceToSun(512.0);
    m_globalLightSource->setDistanceToLookAt(32.0);
    m_globalLightSource->setRotationCenter(glm::vec3(256.0, 0.0, 256.0));
    m_globalLightSource->setLookAt(glm::vec3(256.0, 0.0, 256.0));
}

void CMEgopScene::update(f32)
{
    if(m_model)
    {
        static f32 angle = 0.0;
        angle += 3.0;
        m_model->setRotation(glm::vec3(0.0, angle, 0.0));
    }
}

void CMEgopScene::onGestureRecognizerPressed(const glm::ivec2&, E_INPUT_BUTTON)
{
    
}

void CMEgopScene::onGestureRecognizerMoved(const glm::ivec2&)
{

}

void CMEgopScene::onGestureRecognizerDragged(const glm::ivec2&, E_INPUT_BUTTON)
{

}

void CMEgopScene::onGestureRecognizerReleased(const glm::ivec2&, E_INPUT_BUTTON)
{
    
}

void CMEgopScene::onGestureRecognizerWheelScroll(E_SCROLL_WHEEL_DIRECTION)
{

}

void CMEgopScene::onKeyUp(i32)
{
    
}

void CMEgopScene::onKeyDown(i32)
{
    
}

void CMEgopScene::setModelConfigurationFilenameCommand(const std::string& filename)
{
    if(m_model)
    {
        m_root->removeModel(m_model);
        m_root->deleteGameObject(m_model);
        m_model = nullptr;
    }
    m_model = m_root->createModel(filename);
    m_root->addModel(m_model);
}
