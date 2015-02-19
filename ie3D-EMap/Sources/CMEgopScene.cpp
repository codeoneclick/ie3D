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

CMEgopScene::CMEgopScene(IGameTransition* root) :
IScene(root)
{

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
    
    m_model = m_root->createModel("gameobject.human_01.xml");
    m_root->addModel(m_model);
    
    m_globalLightSource->setAngle(3.0);
    m_globalLightSource->setDistanceToSun(512.0);
    m_globalLightSource->setDistanceToLookAt(32.0);
    m_globalLightSource->setRotationCenter(glm::vec3(256.0, 0.0, 256.0));
    m_globalLightSource->setLookAt(glm::vec3(256.0, 0.0, 256.0));
}

void CMEgopScene::update(f32)
{
    static f32 angle = 0.0;
    angle += 3.0;
    m_model->setRotation(glm::vec3(0.0, angle, 0.0));
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
