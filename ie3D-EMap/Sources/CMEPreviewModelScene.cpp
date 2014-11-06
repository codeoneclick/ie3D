//
//  CMEPreviewModelScene.cpp
//  ie3D-EMap
//
//  Created by sergey.sergeev on 11/3/14.
//
//

#include "CMEPreviewModelScene.h"
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

CMEPreviewModelScene::CMEPreviewModelScene(IGameTransition* root) :
IScene(root)
{

}

CMEPreviewModelScene::~CMEPreviewModelScene(void)
{
    
}

void CMEPreviewModelScene::load(void)
{
    assert(m_root != nullptr);
    m_camera = m_root->createCamera(45.0, 0.01, 1024.0,
                                    glm::ivec4(0, 0,
                                               m_root->getScreenWidth(),
                                               m_root->getScreenHeight()));
    
    m_camera->Set_Position(glm::vec3(0.0f, 0.0f, 0.0f));
    m_camera->Set_LookAt(glm::vec3(0.0f, 0.0f, 0.0f));
    m_camera->Set_Distance(8.0f);
    m_camera->Set_Height(4.0f);
    m_root->setCamera(m_camera);
    
    m_globalLightSource = m_root->createGlobalLightSource(45.0, 0.01, 1024.0);
    m_root->setGlobalLightSource(m_globalLightSource);
    
    m_root->addCollisionHandler(shared_from_this());
    
    m_model = m_root->createModel("gameobject.model.xml");
    m_root->addModel(m_model);
    
    m_globalLightSource->setAngle(3.0);
    m_globalLightSource->setDistanceToSun(512.0);
    m_globalLightSource->setDistanceToLookAt(32.0);
    m_globalLightSource->setRotationCenter(glm::vec3(256.0, 0.0, 256.0));
    m_globalLightSource->setLookAt(glm::vec3(256.0, 0.0, 256.0));
}

void CMEPreviewModelScene::update(f32)
{
    static f32 angle = 0.0;
    angle += 3.0;
    m_model->setRotation(glm::vec3(0.0, angle, 0.0));
}

std::vector<ISharedGameObject> CMEPreviewModelScene::colliders(void)
{
    std::vector<ISharedGameObject> colliders;
    return colliders;
}

void CMEPreviewModelScene::onCollision(const glm::vec3&, ISharedGameObjectRef, E_INPUT_BUTTON)
{
    
}

void CMEPreviewModelScene::onGestureRecognizerPressed(const glm::ivec2&, E_INPUT_BUTTON)
{
    
}

void CMEPreviewModelScene::onGestureRecognizerMoved(const glm::ivec2&)
{

}

void CMEPreviewModelScene::onGestureRecognizerDragged(const glm::ivec2&, E_INPUT_BUTTON)
{

}

void CMEPreviewModelScene::onGestureRecognizerReleased(const glm::ivec2&, E_INPUT_BUTTON)
{
    
}

void CMEPreviewModelScene::onGestureRecognizerWheelScroll(E_SCROLL_WHEEL_DIRECTION)
{

}

void CMEPreviewModelScene::onKeyUp(i32)
{
    
}

void CMEPreviewModelScene::onKeyDown(i32)
{
    
}

void CMEPreviewModelScene::onConfigurationLoaded(ISharedConfigurationRef)
{

}

void CMEPreviewModelScene::onResourceLoaded(ISharedResourceRef resource)
{

}
