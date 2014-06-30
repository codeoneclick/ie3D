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
#include "CEditableSceneTransition.h"
#include "CSelectionArea.h"
#include "CCollisionMgr.h"
#include "IScreenSpaceRenderAccessor.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CModel.h"

CEditableScene::CEditableScene(IGameTransition* root) :
IScene(root),
m_previousDraggedPoint(glm::ivec2(0, 0)),
m_editableRadius(2)
{
    
}

CEditableScene::~CEditableScene(void)
{
    
}

void CEditableScene::load(void)
{
    assert(m_root != nullptr);
    m_camera = m_root->createCamera(90.0,
                                    0.01,
                                    1024.0,
                                    glm::ivec4(0, 0,
                                               m_root->getWindowWidth(),
                                               m_root->getWindowHeight()));
    
    m_camera->Set_Position(glm::vec3(0.0f, 0.0f, 0.0f));
    m_camera->Set_LookAt(glm::vec3(12.0f, 4.0f, 12.0f));
    m_camera->Set_Distance(32.0f);
    m_camera->Set_Height(32.0f);
    
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
    
    m_model = m_root->createModel("gameobject.model.xml");
    m_model->setScale(glm::vec3(10.0f, 10.0f, 10.0f));
    m_root->addModel(m_model);
    
    m_landscape = m_root->createLandscape("gameobject.landscape.xml");
    m_root->setLandscape(m_landscape);
    
    m_root->addCollisionHandler(shared_from_this());
    
    CEditableSceneTransition* transition = static_cast<CEditableSceneTransition*>(m_root);
    m_selectionArea = transition->createSelectionArea("gameobject.selection.area.xml");
    m_root->addCustomGameObject(m_selectionArea);
    m_selectionArea->setLandscape(m_landscape);
    m_selectionArea->setRadius(m_editableRadius);
    
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
    
    CSharedMaterial material = m_root->getSSRenderAccessor()->getSSOperationMaterial("render.operation.screen.ssao");
    if(material != nullptr &&
       material->getShader() != nullptr)
    {
        material->bind();
        static glm::vec3 randomTable[8] = {
            glm::vec3(-0.5, -0.5, -0.5),
            glm::vec3( 0.5, -0.5, -0.5),
            glm::vec3(-0.5,  0.5, -0.5),
            glm::vec3( 0.5,  0.5, -0.5),
            glm::vec3(-0.5, -0.5,  0.5),
            glm::vec3( 0.5, -0.5,  0.5),
            glm::vec3(-0.5,  0.5,  0.5),
            glm::vec3( 0.5,  0.5,  0.5) };
        
        material->getShader()->setVector3ArrayCustom(randomTable, 8, "randomTable");
    }
    
    glm::vec3 position = m_camera->Get_LookAt();
    position.y = m_landscape->getHeight(position);
    m_model->setPosition(position);
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
    
}

void CEditableScene::onGestureRecognizerPressed(const glm::ivec2& point, E_INPUT_BUTTON inputButton)
{
    if(inputButton == E_INPUT_BUTTON_MOUSE_LEFT)
    {
        m_previousDraggedPoint = point;
    }
}

void CEditableScene::onGestureRecognizerMoved(const glm::ivec2& point)
{
    glm::vec3 position;
    static ui32 index = 0;
    index = 0;
    for(const auto& iterator : m_landscape->getChunks())
    {
        if(iterator != nullptr && CCollisionMgr::isGameObjectIntersected(m_camera, iterator, point, &position))
        {
            m_selectionArea->setPosition(position);
            break;
        }
    }
}

void CEditableScene::onGestureRecognizerDragged(const glm::ivec2& point, E_INPUT_BUTTON inputButton)
{
    if(inputButton == E_INPUT_BUTTON_MOUSE_LEFT)
    {
        assert(m_selectionArea != nullptr);
        m_landscape->pressureHeight(m_selectionArea->getPosition(), m_editableRadius, true, (m_previousDraggedPoint.y - point.y) / 10.0);
        m_previousDraggedPoint = point;
        m_selectionArea->setPosition(m_selectionArea->getPosition());
    }
}

void CEditableScene::onGestureRecognizerReleased(const glm::ivec2&, E_INPUT_BUTTON)
{

}

void CEditableScene::onGestureRecognizerWheelScroll(E_SCROLL_WHEEL_DIRECTION direction)
{
    if(direction == E_SCROLL_WHEEL_DIRECTION_FORWARD &&
       m_editableRadius <= 32.0)
    {
        m_editableRadius++;
        m_editableRadius = m_editableRadius % 2 != 0 ? m_editableRadius + 1 : m_editableRadius;
    }
    else if(direction == E_SCROLL_WHEEL_DIRECTION_BACKWARD &&
            m_editableRadius > 2.0)
    {
        m_editableRadius--;
        m_editableRadius = m_editableRadius % 2 != 0 ? m_editableRadius - 1 : m_editableRadius;
    }
    m_selectionArea->setRadius(m_editableRadius);
}

