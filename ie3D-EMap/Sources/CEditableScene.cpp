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
#include "CSkyBox.h"
#include "CParticleEmitter.h"
#include "CMapDragController.h"

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
    m_camera = m_root->CreateCamera(60.0,
                                    0.1,
                                    2048.0,
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
    
    m_landscape = m_root->CreateLandscape("gameobject.landscape.xml");
    m_root->InsertLandscape(m_landscape);
    
    m_root->addCollisionHandler(shared_from_this());
    
    m_mapDragController = std::make_shared<CMapDragController>(m_camera, 0.1,
                                                               glm::vec3(0.0, 0.0, 0.0),
                                                               glm::vec3(512.0, 0.0, 512.0));
    m_root->addGestureRecognizerHandler(m_mapDragController);
    m_root->addGestureRecognizerHandler(std::dynamic_pointer_cast<IGestureRecognizerHandler>(shared_from_this()));
}

void CEditableScene::update(f32 deltatime)
{
    m_mapDragController->update(deltatime);
    static f32 angle = 0.0;
    angle += 0.1;
    m_skyBox->setRotation(glm::vec3(0.0, angle, 0.0));
}

std::vector<ISharedGameObject> CEditableScene::colliders(void)
{
    std::vector<ISharedGameObject> colliders;
    for(ui32 i = 0; i < m_landscape->getChunks().size(); ++i)
    {
        if(m_landscape->getChunks().at(i) != nullptr)
        {
            colliders.push_back(m_landscape->getChunks().at(i));
        }
    }
    return colliders;
}

void CEditableScene::onCollision(const glm::vec3& position, ISharedGameObjectRef, E_INPUT_BUTTON inputButton)
{
    m_particles.at(0)->setPosition(position);
    
    if(inputButton == E_INPUT_BUTTON_MOUSE_LEFT)
    {
        m_landscape->pressureHeightIn(position, 10.0, true);
    }
}

void CEditableScene::onGestureRecognizerPressed(const glm::ivec2&, E_INPUT_BUTTON)
{
    
}

void CEditableScene::onGestureRecognizerMoved(const glm::ivec2&, E_INPUT_BUTTON)
{
    
}

void CEditableScene::onGestureRecognizerReleased(const glm::ivec2&, E_INPUT_BUTTON)
{

}

