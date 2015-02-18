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
#include "CConfigurationAccessor.h"
#include "CCommonOS.h"
#include "ICommand.h"
#include "IUICommands.h"
#include "HUICommands.h"

const std::string kConfigurationBaseMaterialTempFilename = "material.base.goe.temp.xml";
const std::string kConfigurationReflectionMaterialTempFilename = "material.reflection.goe.temp.xml";
const std::string kConfigurationRefractionMaterialTempFilename = "material.refraction.goe.temp.xml";
const std::string kConfigurationShadowingMaterialTempFilename = "material.shadowing.goe.temp.xml";
const std::string kConfigurationModelTempFilename = "gameobject.model.goe.temp.xml";

CMEgoeScene::CMEgoeScene(IGameTransition* root) :
IScene(root),
m_model(nullptr),
m_previousDraggedPoint(0.0f)
{
    ISharedCommand command = std::make_shared<CCommand<UICommandGOECreateConfiguration::COMMAND>>(std::bind(&CMEgoeScene::setMeshFilenameCommand,
                                                                                                            this,
                                                                                                            std::placeholders::_1));
    m_uiToSceneCommands->addCommand(UICommandGOECreateConfiguration::GUID, command);
    
    command = std::make_shared<CCommand<UICommandGOEUpdateConfigurationMaterial::COMMAND>>(std::bind(&CMEgoeScene::updateConfigurationMaterial,
                                                                                                     this,
                                                                                                     std::placeholders::_1));
    m_uiToSceneCommands->addCommand(UICommandGOEUpdateConfigurationMaterial::GUID, command);
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
    
    m_root->addGestureRecognizerHandler(std::dynamic_pointer_cast<IGestureRecognizerHandler>(shared_from_this()));
}

void CMEgoeScene::update(f32)
{
    //static f32 angle = 0.0;
    //angle += 3.0;
    //if(m_model != nullptr)
   // {
    //    m_model->setRotation(glm::vec3(0.0, angle, 0.0));
    //}
}

void CMEgoeScene::onGestureRecognizerPressed(const glm::ivec2& point, E_INPUT_BUTTON)
{
    m_previousDraggedPoint = point;
}

void CMEgoeScene::onGestureRecognizerMoved(const glm::ivec2&)
{
    
}

void CMEgoeScene::onGestureRecognizerDragged(const glm::ivec2& point, E_INPUT_BUTTON inputButton)
{
    glm::vec2 draggingDelta = glm::vec2(fabsf(m_previousDraggedPoint.x - point.x),
                                        fabsf(m_previousDraggedPoint.y - point.y));
    
    if(inputButton == E_INPUT_BUTTON_MOUSE_LEFT && m_model)
    {
        glm::vec3 position = m_model->getPosition();
        
        if(m_previousDraggedPoint.x > point.x)
        {
            position.z -= 0.01f * draggingDelta.x;
        }
        if(m_previousDraggedPoint.x < point.x)
        {
            position.z += 0.01f * draggingDelta.x;
        }
        if(m_previousDraggedPoint.y > point.y)
        {
            position.x += 0.01f * draggingDelta.y;
        }
        if(m_previousDraggedPoint.y < point.y)
        {
            position.x -= 0.01f * draggingDelta.y;
        }
        position.z = glm::clamp(position.z, -1.0f, 5.0f);
        position.x = glm::clamp(position.x, -1.0f, 5.0f);
        m_model->setPosition(position);
        
    }
    else if(inputButton == E_INPUT_BUTTON_MOUSE_RIGHT && m_model)
    {
        glm::vec3 rotation = m_model->getRotation();
        if(m_previousDraggedPoint.x > point.x)
        {
            rotation.y -= 0.25f * draggingDelta.x;
        }
        if(m_previousDraggedPoint.x < point.x)
        {
            rotation.y += 0.25f * draggingDelta.x;
        }
        m_model->setRotation(rotation);
    }
    m_previousDraggedPoint = point;
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

void CMEgoeScene::onConfigurationLoaded(ISharedConfigurationRef configuration)
{
    CSharedConfigurationModel configurationModel = std::static_pointer_cast<CConfigurationModel>(configuration);
    std::vector<CSharedConfigurationMaterial> configurationsMaterials;
    for(ui32 i = 0; i < configurationModel->getMaterialsConfigurations().size(); ++i)
    {
        configurationsMaterials.push_back(std::static_pointer_cast<CConfigurationMaterial>(configurationModel->getMaterialsConfigurations().at(i)));
    }
    assert(m_sceneToUICommands != nullptr);
    if (m_sceneToUICommands != nullptr)
    {
        m_sceneToUICommands->execute<UICommandGOEUpdateConfigurationsMaterials::COMMAND>(UICommandGOEUpdateConfigurationsMaterials::GUID,
                                                                                         configurationsMaterials);
    }
}

CSharedConfigurationModel CMEgoeScene::createTempConfigurationModel(const std::string& filename)
{
    CSharedConfigurationModel configurationModel = std::make_shared<CConfigurationModel>();
    configurationModel->setMeshFilename(filename);
    configurationModel->setZOrder(2);
    configurationModel->setBatching(false);
    
    CSharedConfigurationMaterial configurationMaterial = std::make_shared<CConfigurationMaterial>();
    configurationMaterial->serialize(kConfigurationBaseMaterialTempFilename);
    configurationModel->addMaterialsConfigurations(configurationMaterial);
    
    configurationMaterial = std::make_shared<CConfigurationMaterial>();
    configurationMaterial->serialize(kConfigurationReflectionMaterialTempFilename);
    configurationModel->addMaterialsConfigurations(configurationMaterial);
    
    configurationMaterial = std::make_shared<CConfigurationMaterial>();
    configurationMaterial->serialize(kConfigurationRefractionMaterialTempFilename);
    configurationModel->addMaterialsConfigurations(configurationMaterial);
    
    configurationMaterial = std::make_shared<CConfigurationMaterial>();
    configurationMaterial->serialize(kConfigurationShadowingMaterialTempFilename);
    configurationModel->addMaterialsConfigurations(configurationMaterial);
    
    std::string path = executablepath();
    
    configurationModel->setFilename(path + kConfigurationModelTempFilename);
    configurationModel->deserialize(path + kConfigurationModelTempFilename);
    
    return configurationModel;
}

void CMEgoeScene::setMeshFilenameCommand(const std::string& filename)
{
    CSharedConfigurationModel configurationModel = CMEgoeScene::createTempConfigurationModel(filename);
    assert(configurationModel != nullptr);
    
    if(m_model != nullptr)
    {
        m_root->removeModel(m_model);
    }
    
    if(configurationModel != nullptr)
    {
        std::string path = executablepath();
        m_model = m_root->createModel(path + kConfigurationModelTempFilename);
        m_root->addModel(m_model);
        m_model->setPosition(glm::vec3(2.0f, 0.0f, 2.0f));
        m_model->addConfigurationLoadedCallback(std::bind(&CMEgoeScene::onConfigurationLoaded, this, std::placeholders::_1));
    }
}

void CMEgoeScene::updateConfigurationMaterial(CSharedConfigurationMaterialRef configuration)
{
    assert(configuration != nullptr);
    assert(m_model != nullptr);
    
    CSharedMaterial material = m_model->getMaterial(configuration->getRenderTechniqueName());
    assert(material != nullptr);
    
    material->setCulling(configuration->getCulling());
    material->setCullingMode(configuration->getCullingMode());
    material->setDepthTest(configuration->getDepthTest());
    material->setDepthMask(configuration->getDepthMask());
    material->setBlending(configuration->getBlending());
    material->setBlendingFunctionSource(configuration->getBlendingFunctionSource());
    material->setBlendingFunctionDestination(configuration->getBlendingFunctionDestination());
    material->setClipping(configuration->getClipping());
    material->setClippingPlane(glm::vec4(configuration->getClippingX(),
                                         configuration->getClippingY(),
                                         configuration->getClippingZ(),
                                         configuration->getClippingW()));
    material->setReflecting(configuration->getReflecting());
    material->setShadowing(configuration->getShadowing());
    material->setDebugging(configuration->getDebugging());
    material->setEnabled(configuration->getEnabled());
    
    for (ui32 i = 0; i < configuration->getTexturesConfigurations().size(); ++i)
    {
        CSharedConfigurationTexture configurationTexture = std::static_pointer_cast<CConfigurationTexture>(configuration->getTexturesConfigurations().at(i));
        CSharedTexture texture = m_root->getResourceAccessor()->getTexture(configurationTexture->getTextureFilename());
        material->setTexture(texture, static_cast<E_SHADER_SAMPLER>(i));
    }
}
