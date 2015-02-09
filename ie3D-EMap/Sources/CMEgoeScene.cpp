//
//  CMEgoeScene.cpp
//  ie3D-EMap
//
//  Created by sergey.sergeev on 2/6/15.
//
//

#include "CMEgoeScene.h"
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
#include "CLandscape.h"

CMEgoeScene::CMEgoeScene(IGameTransition* root) :
IScene(root)
{
    
}

CMEgoeScene::~CMEgoeScene(void)
{
    
}

void CMEgoeScene::load(void)
{
    assert(m_root != nullptr);
    m_camera = m_root->createCamera(45.0, 0.01, 1024.0,
                                    glm::ivec4(0, 0,
                                               m_root->getScreenWidth(),
                                               m_root->getScreenHeight()));
    
    m_camera->Set_Position(glm::vec3(0.0f, 0.0f, 0.0f));
    m_camera->Set_LookAt(glm::vec3(2.0f, 0.0f, 2.0f));
    m_camera->Set_Distance(8.0f);
    m_camera->Set_Height(4.0f);
    m_root->setCamera(m_camera);
    
    m_globalLightSource = m_root->createGlobalLightSource(45.0, 0.01, 1024.0);
    m_root->setGlobalLightSource(m_globalLightSource);
    
    m_root->addCollisionHandler(shared_from_this());
    
    m_model = m_root->createModel("gameobject.model.xml");
    m_root->addModel(m_model);
    m_model->setPosition(glm::vec3(2.0f, 0.0f, 2.0f));
    
    m_landscape = m_root->createLandscape("gameobject.landscape.goe.xml");
    m_root->setLandscape(m_landscape);

    std::shared_ptr<COcean> ocean = m_root->createOcean("gameobject.ocean.xml");
    m_root->setOcean(ocean);
    
    m_skybox = m_root->createSkybox("gameobject.skybox.xml");
    m_root->setSkybox(m_skybox);
    
    m_globalLightSource->setAngle(3.0);
    m_globalLightSource->setDistanceToSun(512.0);
    m_globalLightSource->setDistanceToLookAt(8.0);
    m_globalLightSource->setRotationCenter(glm::vec3(2.0f, 0.0f, 2.0f));
    m_globalLightSource->setLookAt(glm::vec3(2.0f, 0.0f, 2.0f));
}

void CMEgoeScene::update(f32)
{
    static f32 angle = 0.0;
    angle += 3.0;
    m_model->setRotation(glm::vec3(0.0, angle, 0.0));
}

void CMEgoeScene::onGestureRecognizerPressed(const glm::ivec2&, E_INPUT_BUTTON)
{
    
}

void CMEgoeScene::onGestureRecognizerMoved(const glm::ivec2&)
{
    
}

void CMEgoeScene::onGestureRecognizerDragged(const glm::ivec2&, E_INPUT_BUTTON)
{
    
}

void CMEgoeScene::onGestureRecognizerReleased(const glm::ivec2&, E_INPUT_BUTTON)
{
    
}

void CMEgoeScene::onGestureRecognizerWheelScroll(E_SCROLL_WHEEL_DIRECTION)
{
    
}

void CMEgoeScene::onKeyUp(i32)
{
    
}

void CMEgoeScene::onKeyDown(i32)
{
    
}
