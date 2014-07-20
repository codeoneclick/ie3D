//
//  CMEScene.cpp
//  indieEngine-EMap
//
//  Created by Sergey Sergeev on 4/20/14.
//
//

#include "CMEScene.h"
#include "IGameTransition.h"
#include "IGameObject.h"
#include "CCamera.h"
#include "CLandscape.h"
#include "COcean.h"
#include "CSkyBox.h"
#include "CParticleEmitter.h"
#include "CMapDragController.h"
#include "CMESceneTransition.h"
#include "CMELandscapeBrush.h"
#include "CCollisionMgr.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CModel.h"
#include "CMEUIToSceneCommands.h"
#include "CMESceneToUICommands.h"
#include "CResourceAccessor.h"
#include "CTexture.h"

CMEScene::CMEScene(IGameTransition* root) :
IScene(root),
m_landscapeBrush(nullptr),
m_previousDraggedPoint(glm::ivec2(0, 0)),
m_uiToSceneCommands(std::make_shared<CMEUIToSceneCommands>()),
m_sceneToUICommands(nullptr),
m_landscapeMaterial(nullptr)
{
    m_editableSettings.m_brushSize = 4;
    m_editableSettings.m_brushStrength = 1;
    m_editableSettings.m_falloffCoefficient = 0;
    m_editableSettings.m_smoothCoefficient = 0;
    
}

CMEScene::~CMEScene(void)
{
    
}

void CMEScene::load(void)
{
    assert(m_root != nullptr);
    m_camera = m_root->createCamera(45.0, 0.01, 1024.0,
                                    glm::ivec4(0, 0,
                                               m_root->getScreenWidth(),
                                               m_root->getScreenHeight()));
    
    m_camera->Set_Position(glm::vec3(0.0f, 0.0f, 0.0f));
    m_camera->Set_LookAt(glm::vec3(12.0f, 4.0f, 12.0f));
    m_camera->Set_Distance(64.0f);
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
    
    CMESceneTransition* transition = static_cast<CMESceneTransition*>(m_root);
    m_landscapeBrush = transition->createLandscapeBrush("gameobject.selection.area.xml");
    m_root->addCustomGameObject(m_landscapeBrush);
    m_landscapeBrush->setLandscape(m_landscape);
    m_landscapeBrush->setSize(m_editableSettings.m_brushSize);
    
    m_mapDragController = std::make_shared<CMapDragController>(m_camera, 0.1,
                                                               glm::vec3(0.0, 0.0, 0.0),
                                                               glm::vec3(512.0, 0.0, 512.0));
    m_root->addGestureRecognizerHandler(m_mapDragController);
    m_root->addGestureRecognizerHandler(std::dynamic_pointer_cast<IGestureRecognizerHandler>(shared_from_this()));
    
    m_uiToSceneCommands->connectSetBrushSizeCommand(std::bind(&CMEScene::setBrushSize, this, std::placeholders::_1));
    m_uiToSceneCommands->connectSetBrushStrengthCommand(std::bind(&CMEScene::setBrushStrength, this, std::placeholders::_1));
    m_uiToSceneCommands->connectSetFalloffCoefficientCommand(std::bind(&CMEScene::setFalloffCoefficient, this, std::placeholders::_1));
    m_uiToSceneCommands->connectSetSmoothCoefficientCommand(std::bind(&CMEScene::setSmoothCoefficient, this, std::placeholders::_1));
    m_uiToSceneCommands->connectSetTextureSamplerCommand(std::bind(&CMEScene::setTextureSampler, this, std::placeholders::_1, std::placeholders::_2));
    m_uiToSceneCommands->connectSetTillingTexcoordCommand(std::bind(&CMEScene::setTillingTexcoord, this, std::placeholders::_1, std::placeholders::_2));
    
    m_landscape->addConfigurationLoadingCommand(std::bind(&CMEScene::onConfigurationLoaded, this, std::placeholders::_1));
}

void CMEScene::update(f32 deltatime)
{
    m_mapDragController->update(deltatime);
    static f32 angle = 0.0;
    angle += 0.1;
    m_skyBox->setRotation(glm::vec3(0.0, angle, 0.0));
    
    glm::vec3 position = m_camera->Get_LookAt();
    position.y = m_landscape->getHeight(position);
    m_model->setPosition(position);
}

std::vector<ISharedGameObject> CMEScene::colliders(void)
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

void CMEScene::onCollision(const glm::vec3& position, ISharedGameObjectRef, E_INPUT_BUTTON inputButton)
{
    
}

void CMEScene::onGestureRecognizerPressed(const glm::ivec2& point, E_INPUT_BUTTON inputButton)
{
    if(inputButton == E_INPUT_BUTTON_MOUSE_LEFT)
    {
        m_previousDraggedPoint = point;
    }
}

void CMEScene::onGestureRecognizerMoved(const glm::ivec2& point)
{
    glm::vec3 position;
    static ui32 index = 0;
    index = 0;
    for(const auto& iterator : m_landscape->getChunks())
    {
        if(iterator != nullptr && CCollisionMgr::isGameObjectIntersected(m_camera, iterator, point, &position))
        {
            m_landscapeBrush->setPosition(position);
            break;
        }
    }
}

void CMEScene::onGestureRecognizerDragged(const glm::ivec2& point, E_INPUT_BUTTON inputButton)
{
    if(inputButton == E_INPUT_BUTTON_MOUSE_LEFT)
    {
        assert(m_landscapeBrush != nullptr);
        m_landscape->pressureHeight(m_landscapeBrush->getPosition(), (m_previousDraggedPoint.y - point.y));
        m_previousDraggedPoint = point;
        m_landscapeBrush->setPosition(m_landscapeBrush->getPosition());
    }
}

void CMEScene::onGestureRecognizerReleased(const glm::ivec2&, E_INPUT_BUTTON)
{
    
}

void CMEScene::onGestureRecognizerWheelScroll(E_SCROLL_WHEEL_DIRECTION direction)
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
    CMEScene::setBrushSize(m_editableSettings.m_brushSize);
    if(m_sceneToUICommands != nullptr)
    {
        m_sceneToUICommands->executeSetBrushSizeCommand(m_editableSettings.m_brushSize);
    }
}

CSharedMEUIToSceneCommands CMEScene::getUIToSceneCommands(void) const
{
    assert(m_uiToSceneCommands != nullptr);
    return m_uiToSceneCommands;
}

void CMEScene::setSceneToUICommands(CSharedMESceneToUICommandsRef commands)
{
    m_sceneToUICommands = commands;
}

void CMEScene::setBrushSize(ui32 value)
{
    m_editableSettings.m_brushSize = value;
    m_landscapeBrush->setSize(m_editableSettings.m_brushSize);
    m_landscape->setEditableSize(m_editableSettings.m_brushSize);
}

void CMEScene::setBrushStrength(ui32 value)
{
    m_editableSettings.m_brushStrength = value;
    m_landscape->setEditableStrength(value);
}

void CMEScene::setFalloffCoefficient(ui32 value)
{
    m_editableSettings.m_falloffCoefficient = value;
    m_landscape->setEditableFalloffCoefficient(value);
}

void CMEScene::setSmoothCoefficient(ui32 value)
{
    m_editableSettings.m_smoothCoefficient = value;
    m_landscape->setEditableSmoothCoefficient(value);
}

void CMEScene::setTextureSampler(const std::string& filename, E_SHADER_SAMPLER sampler)
{
    CSharedTexture texture = m_root->getResourceAccessor()->getTexture(filename);
    assert(texture != nullptr);
    m_landscape->setTexture(texture, sampler);
    m_sceneToUICommands->executeSetTextureSampler(texture, sampler);
}

void CMEScene::setTillingTexcoord(f32 value, E_SHADER_SAMPLER sampler)
{
    m_landscape->setTillingTexcoord(value, sampler);
}

void CMEScene::onConfigurationLoaded(ISharedConfigurationRef)
{
    m_landscape->addResourceLoadingCommand(std::bind(&CMEScene::onResourceLoaded, this, std::placeholders::_1));
    m_landscapeMaterial = m_landscape->getMaterial("render.operation.world.base");
}

void CMEScene::onResourceLoaded(ISharedResourceRef resource)
{
    assert(m_landscapeMaterial != nullptr);
    if(m_landscapeMaterial->getTexture(E_SHADER_SAMPLER_01) == resource)
    {
        CSharedTexture texture = std::static_pointer_cast<CTexture>(resource);
        m_sceneToUICommands->executeSetTextureSampler(texture, E_SHADER_SAMPLER_01);
    }
    else if(m_landscapeMaterial->getTexture(E_SHADER_SAMPLER_02) == resource)
    {
        CSharedTexture texture = std::static_pointer_cast<CTexture>(resource);
        m_sceneToUICommands->executeSetTextureSampler(texture, E_SHADER_SAMPLER_02);
    }
    else if(m_landscapeMaterial->getTexture(E_SHADER_SAMPLER_03) == resource)
    {
        CSharedTexture texture = std::static_pointer_cast<CTexture>(resource);
        m_sceneToUICommands->executeSetTextureSampler(texture, E_SHADER_SAMPLER_03);
    }
}
