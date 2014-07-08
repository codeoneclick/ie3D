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
#include "CEditableBrush.h"
#include "CCollisionMgr.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CModel.h"
#include "CMEUIToSceneCommands.h"
#include "CMESceneToUICommands.h"

CEditableScene::CEditableScene(IGameTransition* root) :
IScene(root),
m_previousDraggedPoint(glm::ivec2(0, 0)),
m_uiToSceneCommands(std::make_shared<CMEUIToSceneCommands>()),
m_sceneToUICommands(nullptr)
{
    m_editableSettings.m_brushSize = 4;
    m_editableSettings.m_brushStrength = 1;
    m_editableSettings.m_falloffCoefficient = 0;
    m_editableSettings.m_smoothCoefficient = 0;
    
}

CEditableScene::~CEditableScene(void)
{
    
}

void CEditableScene::load(void)
{
    assert(m_root != nullptr);
    m_camera = m_root->createCamera(90.0, 0.01, 1024.0,
                                    glm::ivec4(0, 0,
                                               m_root->getScreenWidth(),
                                               m_root->getScreenHeight()));
    
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
    m_editableBrush = transition->createEditableBrush("gameobject.selection.area.xml");
    m_root->addCustomGameObject(m_editableBrush);
    m_editableBrush->setLandscape(m_landscape);
    m_editableBrush->setSize(m_editableSettings.m_brushSize);
    
    m_mapDragController = std::make_shared<CMapDragController>(m_camera, 0.1,
                                                               glm::vec3(0.0, 0.0, 0.0),
                                                               glm::vec3(512.0, 0.0, 512.0));
    m_root->addGestureRecognizerHandler(m_mapDragController);
    m_root->addGestureRecognizerHandler(std::dynamic_pointer_cast<IGestureRecognizerHandler>(shared_from_this()));
    
    m_uiToSceneCommands->connectSetBrushSizeCommand(std::bind(&CEditableScene::setBrushSize, this, std::placeholders::_1));
    m_uiToSceneCommands->connectSetBrushStrengthCommand(std::bind(&CEditableScene::setBrushStrength, this, std::placeholders::_1));
    m_uiToSceneCommands->connectSetFalloffCoefficientCommand(std::bind(&CEditableScene::setFalloffCoefficient, this, std::placeholders::_1));
    m_uiToSceneCommands->connectSetSmoothCoefficientCommand(std::bind(&CEditableScene::setSmoothCoefficient, this, std::placeholders::_1));
}

void CEditableScene::update(f32 deltatime)
{
    m_mapDragController->update(deltatime);
    static f32 angle = 0.0;
    angle += 0.1;
    m_skyBox->setRotation(glm::vec3(0.0, angle, 0.0));
    
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
            m_editableBrush->setPosition(position);
            break;
        }
    }
}

void CEditableScene::onGestureRecognizerDragged(const glm::ivec2& point, E_INPUT_BUTTON inputButton)
{
    if(inputButton == E_INPUT_BUTTON_MOUSE_LEFT)
    {
        assert(m_editableBrush != nullptr);
        m_landscape->pressureHeight(m_editableBrush->getPosition(), (m_previousDraggedPoint.y - point.y));
        m_previousDraggedPoint = point;
        m_editableBrush->setPosition(m_editableBrush->getPosition());
    }
}

void CEditableScene::onGestureRecognizerReleased(const glm::ivec2&, E_INPUT_BUTTON)
{

}

void CEditableScene::onGestureRecognizerWheelScroll(E_SCROLL_WHEEL_DIRECTION direction)
{
    if(direction == E_SCROLL_WHEEL_DIRECTION_FORWARD &&
       m_editableSettings.m_brushSize < 32.0)
    {
        m_editableSettings.m_brushSize++;
        m_editableSettings.m_brushSize = m_editableSettings.m_brushSize % 2 != 0 ? m_editableSettings.m_brushSize + 1 : m_editableSettings.m_brushSize;
    }
    else if(direction == E_SCROLL_WHEEL_DIRECTION_BACKWARD &&
            m_editableSettings.m_brushSize > 4.0)
    {
        m_editableSettings.m_brushSize--;
        m_editableSettings.m_brushSize = m_editableSettings.m_brushSize % 2 != 0 ? m_editableSettings.m_brushSize - 1 : m_editableSettings.m_brushSize;
    }
    CEditableScene::setBrushSize(m_editableSettings.m_brushSize);
    if(m_sceneToUICommands != nullptr)
    {
        m_sceneToUICommands->executeSetBrushSizeCommand(m_editableSettings.m_brushSize);
    }
}

CSharedMEUIToSceneCommands CEditableScene::getUIToSceneCommands(void) const
{
    assert(m_uiToSceneCommands != nullptr);
    return m_uiToSceneCommands;
}

void CEditableScene::setSceneToUICommands(CSharedMESceneToUICommandsRef commands)
{
    m_sceneToUICommands = commands;
}

void CEditableScene::setBrushSize(ui32 value)
{
    m_editableSettings.m_brushSize = value;
    m_editableBrush->setSize(m_editableSettings.m_brushSize);
    m_landscape->setEditableSize(m_editableSettings.m_brushSize);
}

void CEditableScene::setBrushStrength(ui32 value)
{
    m_editableSettings.m_brushStrength = value;
    m_landscape->setEditableStrength(value);
}

void CEditableScene::setFalloffCoefficient(ui32 value)
{
    m_editableSettings.m_falloffCoefficient = value;
    m_landscape->setEditableFalloffCoefficient(value);
}

void CEditableScene::setSmoothCoefficient(ui32 value)
{
    m_editableSettings.m_smoothCoefficient = value;
    m_landscape->setEditableSmoothCoefficient(value);
}
