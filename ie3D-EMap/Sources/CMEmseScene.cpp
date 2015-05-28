//
//  CMEmseScene.cpp
//  indieEngine-EMap
//
//  Created by Sergey Sergeev on 4/20/14.
//
//

#include "CMEmseScene.h"
#include "IGameTransition.h"
#include "IGameObject.h"
#include "CCamera.h"
#include "CGlobalLightSource.h"
#include "CLandscape.h"
#include "COcean.h"
#include "CSkyBox.h"
#include "CAtmosphericScattering.h"
#include "CParticleEmitter.h"
#include "CMapDragController.h"
#include "CMEmseTransition.h"
#include "CMELandscapeBrush.h"
#include "CCollisionMgr.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CModel.h"
#include "CResourceAccessor.h"
#include "CTexture.h"
#include "CEComplexModel.h"
#include "CMEModelBrush.h"
#include "CConfigurationAccessor.h"
#include "ICommand.h"
#include "IUICommands.h"
#include "HUICommands.h"
#include "CMEModelBrush.h"
#include "CECustomModel.h"

CMEmseScene::CMEmseScene(IGameTransition* root) :
IScene(root),
m_landscapeMaterial(nullptr),
m_selectedGameObject(nullptr),
m_selectedBrushElement(nullptr),
m_landscapeBrush(nullptr),
m_previousDraggedPoint2D(glm::ivec2(0, 0)),
m_previousDraggedPoint3D(glm::vec3(0.0f)),
m_isSpaceButtonPressed(false)
{
    m_editableSettings.m_brushSize = 4;
    m_editableSettings.m_brushStrength = 1;
    m_editableSettings.m_falloffCoefficient = 0;
    m_editableSettings.m_smoothCoefficient = 0;
    
    ISharedCommand command = std::make_shared<CCommand<UICommandMSESetBrushSize::COMMAND>>(std::bind(&CMEmseScene::setBrushSize,
                                                                                                     this,
                                                                                                     std::placeholders::_1));
    m_uiToSceneCommands->addCommand(UICommandMSESetBrushSize::GUID,
                                    command);
    
    command = std::make_shared<CCommand<UICommandMSESetBrushStrength::COMMAND>>(std::bind(&CMEmseScene::setBrushStrength,
                                                                                          this,
                                                                                          std::placeholders::_1));
    m_uiToSceneCommands->addCommand(UICommandMSESetBrushStrength::GUID,
                                    command);
    
    command = std::make_shared<CCommand<UICommandMSESetFalloffCoefficient::COMMAND>>(std::bind(&CMEmseScene::setFalloffCoefficient,
                                                                                               this,
                                                                                               std::placeholders::_1));
    m_uiToSceneCommands->addCommand(UICommandMSESetFalloffCoefficient::GUID,
                                    command);
    
    command = std::make_shared<CCommand<UICommandMSESetSmoothCoefficient::COMMAND>>(std::bind(&CMEmseScene::setSmoothCoefficient,
                                                                                              this,
                                                                                              std::placeholders::_1));
    m_uiToSceneCommands->addCommand(UICommandMSESetSmoothCoefficient::GUID,
                                    command);
    
    command = std::make_shared<CCommand<UICommandMSESetTextureFilename::COMMAND>>(std::bind(&CMEmseScene::setTextureFilename,
                                                                                            this,
                                                                                            std::placeholders::_1,
                                                                                            std::placeholders::_2));
    m_uiToSceneCommands->addCommand(UICommandMSESetTextureFilename::GUID,
                                    command);
    
    command = std::make_shared<CCommand<UICommandMSESetTillingTexcoord::COMMAND>>(std::bind(&CMEmseScene::setTillingTexcoord,
                                                                                            this,
                                                                                            std::placeholders::_1,
                                                                                            std::placeholders::_2));
    m_uiToSceneCommands->addCommand(UICommandMSESetTillingTexcoord::GUID,
                                    command);
    
    
    command = std::make_shared<CCommand<UICommandMSEGenerateHeightmap::COMMAND>>(std::bind(&CMEmseScene::generateHeightmap,
                                                                                           this,
                                                                                           std::placeholders::_1,
                                                                                           std::placeholders::_2,
                                                                                           std::placeholders::_3,
                                                                                           std::placeholders::_4));
    m_uiToSceneCommands->addCommand(UICommandMSEGenerateHeightmap::GUID,
                                    command);
    
    command = std::make_shared<CCommand<UICommandMSESetLandscapeEditMode::COMMAND>>(std::bind(&CMEmseScene::setLandscapeEditMode,
                                                                                              this,
                                                                                              std::placeholders::_1));
    m_uiToSceneCommands->addCommand(UICommandMSESetLandscapeEditMode::GUID,
                                    command);
    
    command = std::make_shared<CCommand<UICommandMSEAddGameObjectToScene::COMMAND>>(std::bind(&CMEmseScene::addGameObjectToScene,
                                                                                              this,
                                                                                              std::placeholders::_1));
    m_uiToSceneCommands->addCommand(UICommandMSEAddGameObjectToScene::GUID,
                                    command);

}

CMEmseScene::~CMEmseScene(void)
{
    
}

void CMEmseScene::load(void)
{
    assert(m_root != nullptr);
    m_camera = m_root->createCamera(45.0, 0.01, 768.0,
                                    glm::ivec4(0, 0,
                                               m_root->getScreenWidth(),
                                               m_root->getScreenHeight()));
    
    m_camera->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    m_camera->setLookAt(glm::vec3(12.0f, 4.0f, 12.0f));
    m_camera->setDistanceToLookAt(glm::vec3(64.0, 16.0, 64.0));
    m_root->setCamera(m_camera);
    
    m_globalLightSource = m_root->createGlobalLightSource(45.0, 0.01, 768.0);
    m_root->setGlobalLightSource(m_globalLightSource);
   
    std::shared_ptr<COcean> ocean = m_root->createOcean("gameobject.ocean.xml");
    m_root->setOcean(ocean);
    
    m_skybox = m_root->createSkybox("gameobject.skybox.xml");
    m_root->setSkybox(m_skybox);
    
    m_landscape = m_root->createLandscape("gameobject.landscape.xml");
    m_root->setLandscape(m_landscape);
    
    m_root->addCollisionHandler(shared_from_this());
    
    CMEmseTransition* transition = static_cast<CMEmseTransition *>(m_root);
    
    m_landscapeBrush = transition->createLandscapeBrush("gameobject.landscape.brush.xml");
    m_root->addCustomGameObject(m_landscapeBrush);
    m_landscapeBrush->setLandscape(m_landscape);
    m_landscapeBrush->setSize(m_editableSettings.m_brushSize);
    m_landscapeBrush->setVisible(false);
    
    m_gameObjectBrush = transition->createModelBrush("gameobject.gameobject.brush.xml");
    m_root->addCustomGameObject(m_gameObjectBrush);
    m_gameObjectBrush->setLandscape(m_landscape);
    m_gameObjectBrush->setVisible(false);
    
    m_mapDragController = std::make_shared<CMapDragController>(m_camera, m_landscape, 2.0,
                                                               glm::vec3(0.0, 0.0, 0.0),
                                                               glm::vec3(512.0, 0.0, 512.0));
    m_root->addGestureRecognizerHandler(m_mapDragController);
    m_root->addGestureRecognizerHandler(std::static_pointer_cast<IGestureRecognizerHandler>(shared_from_this()));
    
    m_landscape->addConfigurationLoadedCallback(std::bind(&CMEmseScene::onConfigurationLoaded, this, std::placeholders::_1));
    
    m_globalLightSource->setAngle(3.0);
    m_globalLightSource->setDistanceToSun(512.0);
    m_globalLightSource->setDistanceToLookAt(32.0);
    m_globalLightSource->setRotationCenter(glm::vec3(256.0, 0.0, 256.0));
    m_globalLightSource->setLookAt(glm::vec3(256.0, 0.0, 256.0));
}

void CMEmseScene::update(f32 deltatime)
{
    m_mapDragController->update(deltatime);
    static f32 angle = 0.0;
    angle += 0.1;
    m_skybox->setRotation(glm::vec3(0.0, angle, 0.0));
}

std::vector<ISharedGameObject> CMEmseScene::colliders(void)
{
    std::vector<ISharedGameObject> colliders;
    for(const auto& it : m_landscape->getChunks())
    {
        if(it != nullptr)
        {
            colliders.push_back(it);
        }
    }
    if(m_landscapeEditMode == E_LANDSCAPE_EDIT_MODE_GAMEOBJECTS)
    {
        for(const auto& it : m_models)
        {
            colliders.push_back(it);
        }
        if(m_gameObjectBrush->isVisible())
        {
            for(const auto& it : m_gameObjectBrush->getArrows())
            {
                colliders.push_back(it);
            }
            for(const auto& it : m_gameObjectBrush->getPlanes())
            {
                colliders.push_back(it);
            }
        }
        colliders.push_back(m_gameObjectBrush->getSphere());
    }
    return colliders;
}

void CMEmseScene::onGestureRecognizerPressed(const glm::ivec2& point, E_INPUT_BUTTON inputButton)
{
    if(inputButton == E_INPUT_BUTTON_MOUSE_LEFT)
    {
        m_previousDraggedPoint2D = point;
        bool isIntersectedWithGameObjectBrush = false;
        if(m_gameObjectBrush->isVisible())
        {
            glm::vec3 pressedPoint3D;
            if(m_gameObjectBrush->getMode() == E_GAMEOBJECT_BRUSH_MODE_ROTATE &&
               CCollisionMgr::isGameObjectIntersected(m_camera, m_gameObjectBrush->getSphere(), point, &pressedPoint3D, true, true))
            {
                isIntersectedWithGameObjectBrush = true;
                m_previousDraggedPoint3D = pressedPoint3D;
                m_selectedBrushElement =  m_gameObjectBrush->getSphere();
            }
            
            if(m_gameObjectBrush->getMode() == E_GAMEOBJECT_BRUSH_MODE_TRANSLATE &&
               !isIntersectedWithGameObjectBrush)
            {
                for(const auto& it : m_gameObjectBrush->getArrows())
                {
                    if(CCollisionMgr::isGameObjectBoundIntersected(m_camera, it, point, true, true))
                    {
                        isIntersectedWithGameObjectBrush = true;
                        glm::ray ray;
                        CCollisionMgr::unproject(point, m_camera->getVMatrix(),
                                                 m_camera->getPMatrix(),
                                                 m_camera->getViewport(),
                                                 &ray);
                        m_previousDraggedPoint3D = ray.getDirection() * m_camera->getFar();
                        m_selectedBrushElement = it;
                        break;
                    }
                }
            }
            if(m_gameObjectBrush->getMode() == E_GAMEOBJECT_BRUSH_MODE_TRANSLATE &&
               !isIntersectedWithGameObjectBrush)
            {
                for(const auto& it : m_gameObjectBrush->getPlanes())
                {
                    if(CCollisionMgr::isGameObjectBoundIntersected(m_camera, it, point, true, true))
                    {
                        isIntersectedWithGameObjectBrush = true;
                        CCollisionMgr::isGameObjectIntersected(m_camera, it, point, &pressedPoint3D, true, true);
                        m_previousDraggedPoint3D = pressedPoint3D;
                        m_selectedBrushElement = it;
                        break;
                    }
                }
            }
        }
        
        if(!isIntersectedWithGameObjectBrush)
        {
            m_selectedGameObject = nullptr;
            m_selectedBrushElement = nullptr;
            for(const auto& it : m_models)
            {
                if(CCollisionMgr::isGameObjectBoundIntersected(m_camera, it, point, true, true))
                {
                    m_selectedGameObject = it;
                }
            }
            if(m_selectedGameObject)
            {
                m_gameObjectBrush->setVisible(true);
                m_gameObjectBrush->setPosition(m_selectedGameObject->getPosition());
            }
            else
            {
                m_gameObjectBrush->setVisible(false);
            }
        }
    }
}

void CMEmseScene::onGestureRecognizerMoved(const glm::ivec2& point)
{
    glm::vec3 position;
    static ui32 index = 0;
    index = 0;
    for(const auto& iterator : m_landscape->getChunks())
    {
        if(iterator != nullptr && CCollisionMgr::isGameObjectIntersected(m_camera, iterator, point, &position, false))
        {
            assert(m_landscapeBrush != nullptr);
            m_landscapeBrush->setPosition(position);
            
            break;
        }
    }
}

void CMEmseScene::onGestureRecognizerDragged(const glm::ivec2& point, E_INPUT_BUTTON inputButton)
{
    if(inputButton == E_INPUT_BUTTON_MOUSE_LEFT &&
       m_landscapeEditMode == E_LANDSCAPE_EDIT_MODE_HEIGHTMAP)
    {
        assert(m_landscapeBrush != nullptr);
        m_landscape->pressureHeight(m_landscapeBrush->getPosition(), (m_previousDraggedPoint2D.y - point.y));
        m_landscapeBrush->setPosition(m_landscapeBrush->getPosition());
    }
    else if(inputButton == E_INPUT_BUTTON_MOUSE_LEFT &&
            m_landscapeEditMode == E_LANDSCAPE_EDIT_MODE_GAMEOBJECTS)
    {
        bool isIntersectedWithGameObjectBrush = false;
        if(m_gameObjectBrush->isVisible() && m_selectedBrushElement)
        {
            ui32 index = 0;
            glm::vec3 draggedPoint3D;
            
            if(m_gameObjectBrush->getMode() == E_GAMEOBJECT_BRUSH_MODE_ROTATE &&
               CCollisionMgr::isGameObjectIntersected(m_camera, m_gameObjectBrush->getSphere(), point, &draggedPoint3D, true, true))
            {
                isIntersectedWithGameObjectBrush = true;
                glm::vec3 rotation = m_selectedGameObject->getRotation();
                rotation.y += (point.x - m_previousDraggedPoint2D.x);
                glm::vec3 direction = glm::normalize(m_selectedGameObject->getPosition() - m_camera->getPosition());
                rotation.x += (point.y - m_previousDraggedPoint2D.y) * -direction.z;
                rotation.z += (point.y - m_previousDraggedPoint2D.y) * direction.x;
                m_selectedGameObject->setRotation(rotation);
                m_previousDraggedPoint3D = draggedPoint3D;
            }
            if(m_gameObjectBrush->getMode() == E_GAMEOBJECT_BRUSH_MODE_TRANSLATE &&
               !isIntersectedWithGameObjectBrush)
            {
                for(const auto& it : m_gameObjectBrush->getArrows())
                {
                    if(m_selectedBrushElement == it)
                    {
                        isIntersectedWithGameObjectBrush = true;
                        glm::ray ray;
                        CCollisionMgr::unproject(point, m_camera->getVMatrix(),
                                                 m_camera->getPMatrix(),
                                                 m_camera->getViewport(),
                                                 &ray);
                        draggedPoint3D = ray.getDirection() * m_camera->getFar();
                        switch (index)
                        {
                            case E_MODEL_BRUSH_ARROW_X:
                            {
                                assert(m_selectedGameObject);
                                glm::vec3 position = m_selectedGameObject->getPosition();
                                position.x += (m_previousDraggedPoint3D.x - draggedPoint3D.x) / 50.0;
                                m_selectedGameObject->setPosition(position);
                                m_gameObjectBrush->setPosition(position);
                            }
                                break;
                                
                            case E_MODEL_BRUSH_ARROW_Y:
                            {
                                assert(m_selectedGameObject);
                                glm::vec3 position = m_selectedGameObject->getPosition();
                                position.y += (m_previousDraggedPoint3D.y - draggedPoint3D.y) / 50.0;
                                m_selectedGameObject->setPosition(position);
                                m_gameObjectBrush->setPosition(position);
                            }
                                break;
                                
                            case E_MODEL_BRUSH_ARROW_Z:
                            {
                                assert(m_selectedGameObject);
                                glm::vec3 position = m_selectedGameObject->getPosition();
                                position.z += (m_previousDraggedPoint3D.z - draggedPoint3D.z) / 50.0;
                                m_selectedGameObject->setPosition(position);
                                m_gameObjectBrush->setPosition(position);
                            }
                                break;
                                
                            default:
                                break;
                        }
                        m_previousDraggedPoint3D = draggedPoint3D;
                        break;
                    }
                    index++;
                }
            }
            if(m_gameObjectBrush->getMode() == E_GAMEOBJECT_BRUSH_MODE_TRANSLATE &&
               !isIntersectedWithGameObjectBrush)
            {
                index = 0;
                for(const auto& it : m_gameObjectBrush->getPlanes())
                {
                    if(CCollisionMgr::isGameObjectIntersected(m_camera, it, point, &draggedPoint3D, true, true))
                    {
                        isIntersectedWithGameObjectBrush = true;
                        switch (index)
                        {
                            case E_MODEL_BRUSH_PLANE_YZ:
                            {
                                assert(m_selectedGameObject);
                                glm::vec3 position = m_selectedGameObject->getPosition();
                                
                                position.x += (draggedPoint3D.x - m_previousDraggedPoint3D.x);
                                position.y += (draggedPoint3D.y - m_previousDraggedPoint3D.y);
                                m_selectedGameObject->setPosition(position);
                                m_gameObjectBrush->setPosition(position);
                            }
                                break;
                                
                            case E_MODEL_BRUSH_PLANE_XZ:
                            {
                                assert(m_selectedGameObject);
                                glm::vec3 position = m_selectedGameObject->getPosition();
                                
                                position.x += (draggedPoint3D.x - m_previousDraggedPoint3D.x);
                                position.z += (draggedPoint3D.z - m_previousDraggedPoint3D.z);
                                
                                m_selectedGameObject->setPosition(position);
                                m_gameObjectBrush->setPosition(position);
                            }
                                break;
                                
                            case E_MODEL_BRUSH_PLANE_XY:
                            {
                                assert(m_selectedGameObject);
                                glm::vec3 position = m_selectedGameObject->getPosition();
                                position.y += (draggedPoint3D.y - m_previousDraggedPoint3D.y);
                                position.z += (draggedPoint3D.z - m_previousDraggedPoint3D.z);
                                
                                m_selectedGameObject->setPosition(position);
                                m_gameObjectBrush->setPosition(position);
                            }
                                break;
                                
                            default:
                                break;
                        }
                        m_previousDraggedPoint3D = draggedPoint3D;
                        break;
                    }
                    index++;
                }
            }
        }
    }
    m_previousDraggedPoint2D = point;
}

void CMEmseScene::onGestureRecognizerReleased(const glm::ivec2&, E_INPUT_BUTTON)
{
    
}

void CMEmseScene::onGestureRecognizerWheelScroll(E_SCROLL_WHEEL_DIRECTION direction)
{
    if(!m_isSpaceButtonPressed &&
       m_landscapeEditMode == E_LANDSCAPE_EDIT_MODE_HEIGHTMAP)
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
        CMEmseScene::setBrushSize(m_editableSettings.m_brushSize);
        if(m_sceneToUICommands != nullptr)
        {
            m_sceneToUICommands->execute<UICommandMSESetBrushSize::COMMAND>(UICommandMSESetBrushSize::GUID,
                                                                            m_editableSettings.m_brushSize);
        }
    }
}

void CMEmseScene::onKeyUp(i32 key)
{
    switch (key)
    {
        case 32:
        {
            m_isSpaceButtonPressed = false;
        }
            break;
            
        default:
            break;
    }
}

void CMEmseScene::onKeyDown(i32 key)
{
    switch (key)
    {
        case 32:
        {
            m_isSpaceButtonPressed = true;
        }
            break;
            
        default:
            break;
    }
}
void CMEmseScene::setBrushSize(ui32 value)
{
    m_editableSettings.m_brushSize = value;
    m_landscapeBrush->setSize(m_editableSettings.m_brushSize);
    m_landscape->setEditableSize(m_editableSettings.m_brushSize);
}

void CMEmseScene::setBrushStrength(ui32 value)
{
    m_editableSettings.m_brushStrength = value;
    m_landscape->setEditableStrength(value);
}

void CMEmseScene::setFalloffCoefficient(ui32 value)
{
    m_editableSettings.m_falloffCoefficient = value;
    m_landscape->setEditableFalloffCoefficient(value);
}

void CMEmseScene::setSmoothCoefficient(ui32 value)
{
    m_editableSettings.m_smoothCoefficient = value;
    m_landscape->setEditableSmoothCoefficient(value);
}

void CMEmseScene::setTextureFilename(const std::string& filename, E_SHADER_SAMPLER sampler)
{
    CSharedTexture texture = m_root->getResourceAccessor()->getTexture(filename);
    assert(texture != nullptr);
    m_landscape->setTexture(texture, sampler);
}

void CMEmseScene::setTillingTexcoord(f32 value, E_SHADER_SAMPLER sampler)
{
    m_landscape->setTillingTexcoord(value, sampler);
}

void CMEmseScene::generateHeightmap(const glm::ivec2& size, f32 frequency, i32 octaves, ui32 seed)
{
    assert(m_landscape != nullptr);
    m_landscape->generateVertecesData(size, frequency, octaves, seed);
}

void CMEmseScene::onConfigurationLoaded(ISharedConfigurationRef configuration)
{
    m_landscapeMaterial = m_landscape->getMaterial("ws.base");
    m_landscape->addResourceLoadingCommand(std::bind(&CMEmseScene::onResourceLoaded, this, std::placeholders::_1));
}

void CMEmseScene::onResourceLoaded(ISharedResourceRef resource)
{
    return;
    assert(m_landscapeMaterial != nullptr);
    if(m_landscapeMaterial->getTexture(E_SHADER_SAMPLER_01) == resource)
    {
        CSharedTexture texture = std::static_pointer_cast<CTexture>(resource);
        m_sceneToUICommands->execute<UICommandMSESetTexture::COMMAND>(UICommandMSESetTexture::GUID,
                                                                      texture, E_SHADER_SAMPLER_01);
        m_sceneToUICommands->execute<UICommandMSESetTillingTexcoord::COMMAND>(UICommandMSESetTillingTexcoord::GUID,
                                                                              m_landscape->getTillingTexcoord(E_SHADER_SAMPLER_01),
                                                                              E_SHADER_SAMPLER_01);
    }
    else if(m_landscapeMaterial->getTexture(E_SHADER_SAMPLER_02) == resource)
    {
        CSharedTexture texture = std::static_pointer_cast<CTexture>(resource);
        m_sceneToUICommands->execute<UICommandMSESetTexture::COMMAND>(UICommandMSESetTexture::GUID,
                                                                      texture, E_SHADER_SAMPLER_02);
        m_sceneToUICommands->execute<UICommandMSESetTillingTexcoord::COMMAND>(UICommandMSESetTillingTexcoord::GUID,
                                                                              m_landscape->getTillingTexcoord(E_SHADER_SAMPLER_02),
                                                                              E_SHADER_SAMPLER_02);
    }
    else if(m_landscapeMaterial->getTexture(E_SHADER_SAMPLER_03) == resource)
    {
        CSharedTexture texture = std::static_pointer_cast<CTexture>(resource);
        m_sceneToUICommands->execute<UICommandMSESetTexture::COMMAND>(UICommandMSESetTexture::GUID,
                                                                      texture, E_SHADER_SAMPLER_03);
        m_sceneToUICommands->execute<UICommandMSESetTillingTexcoord::COMMAND>(UICommandMSESetTillingTexcoord::GUID,
                                                                              m_landscape->getTillingTexcoord(E_SHADER_SAMPLER_03),
                                                                              E_SHADER_SAMPLER_03);
    }
}

void CMEmseScene::setLandscapeEditMode(E_LANDSCAPE_EDIT_MODE mode)
{
    switch (mode)
    {
        case E_LANDSCAPE_EDIT_MODE_HEIGHTMAP:
        {
            m_landscapeBrush->setVisible(true);
            m_landscapeBrush->setPosition(m_camera->getLookAt());
            m_gameObjectBrush->setVisible(false);
        }
            break;
            
        case E_LANDSCAPE_EDIT_MODE_TEXTURES:
        {
            m_landscapeBrush->setVisible(false);
            m_gameObjectBrush->setVisible(false);
        }
            break;
            
        case E_LANDSCAPE_EDIT_MODE_GAMEOBJECTS:
        {
            m_landscapeBrush->setVisible(false);
            m_gameObjectBrush->setVisible(false);
        }
            break;
            
        default:
            break;
    }
    m_landscapeEditMode = mode;
}

void CMEmseScene::addGameObjectToScene(const std::string& configurationFilename)
{
    CSharedModel model = m_root->createModel(configurationFilename);
    m_root->addModel(model);
    glm::vec3 position = m_camera->getLookAt();
    model->setPosition(glm::vec3(position.x,
                                m_landscape->getHeight(position),
                                position.z));
    m_gameObjectBrush->setVisible(true);
    m_gameObjectBrush->setPosition(model->getPosition());
    m_models.push_back(model);
    m_selectedGameObject = model;
}
