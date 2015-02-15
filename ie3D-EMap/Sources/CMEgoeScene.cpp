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

CMEgoeScene::CMEgoeScene(IGameTransition* root) :
IScene(root),
m_model(nullptr)
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
}

void CMEgoeScene::update(f32)
{
    static f32 angle = 0.0;
    angle += 3.0;
    if(m_model != nullptr)
    {
        m_model->setRotation(glm::vec3(0.0, angle, 0.0));
    }
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

CSharedConfigurationShader CMEgoeScene::createTempConfigurationShader(void)
{
    CSharedConfigurationShader configurationShader = std::make_shared<CConfigurationShader>();
    configurationShader->setVSFilename("shaderModel.vert");
    configurationShader->setFSFilename("shaderModel.frag");
    
    return configurationShader;
}

CSharedConfigurationTexture CMEgoeScene::createTempConfigurationTexture(ui32 sampleIndex)
{
    CSharedConfigurationTexture configurationTexture = std::make_shared<CConfigurationTexture>();
    configurationTexture->setCubemap(false);
    configurationTexture->setTextureFilename("gameobject.goe.temp.png");
    configurationTexture->setTextureFilenameNegativeX("");
    configurationTexture->setTextureFilenameNegativeY("");
    configurationTexture->setTextureFilenameNegativeZ("");
    configurationTexture->setTextureFilenamePositiveX("");
    configurationTexture->setTextureFilenamePositiveY("");
    configurationTexture->setTextureFilenamePositiveZ("");
    configurationTexture->setRenderTechniqueTextureName("");
    configurationTexture->setSamplerIndex(sampleIndex);
    configurationTexture->setWrapMode(GL_REPEAT);
    configurationTexture->setMagFilter(GL_LINEAR);
    configurationTexture->setMinFilter(GL_LINEAR);
    
    return configurationTexture;
}

CSharedConfigurationMaterial CMEgoeScene::createTempConfigurationMaterial(const std::string& techniqueName)
{
    CSharedConfigurationShader configurationShader = CMEgoeScene::createTempConfigurationShader();
    CSharedConfigurationTexture configurationTexture = CMEgoeScene::createTempConfigurationTexture(0);
    
    CSharedConfigurationMaterial configurationMaterial = std::make_shared<CConfigurationMaterial>();
    configurationMaterial->setRenderTechniqueName(techniqueName);
    configurationMaterial->setCulling(false);
    configurationMaterial->setCullingMode(GL_FRONT);
    configurationMaterial->setDepthTest(true);
    configurationMaterial->setDepthMask(true);
    configurationMaterial->setBlending(false);
    configurationMaterial->setBlendingFunctionSource(GL_SRC_ALPHA);
    configurationMaterial->setBlendingFunctionDestination(GL_ONE_MINUS_SRC_ALPHA);
    configurationMaterial->setClipping(false);
    configurationMaterial->setClippingX(0.0);
    configurationMaterial->setClippingY(0.0);
    configurationMaterial->setClippingZ(0.0);
    configurationMaterial->setClippingW(0.0);
    configurationMaterial->setReflecting(false);
    configurationMaterial->setShadowing(false);
    configurationMaterial->setDebugging(false);
    configurationMaterial->setShaderConfiguration(configurationShader);
    configurationMaterial->addTexturesConfigurations(configurationTexture);
    
    std::string path = executablepath();
    
    configurationMaterial->setFilename(path + "material." + techniqueName + ".goe.temp.xml");
    configurationMaterial->deserialize(path + "material." + techniqueName + ".goe.temp.xml");
    
    return configurationMaterial;
}

CSharedConfigurationModel CMEgoeScene::createTempConfigurationModel(const std::string& filename)
{
    CSharedConfigurationModel configurationModel = std::make_shared<CConfigurationModel>();
    configurationModel->setMeshFilename(filename);
    configurationModel->setBatching(false);
    
    CSharedConfigurationMaterial configurationMaterial = CMEgoeScene::createTempConfigurationMaterial("ws.base");
    configurationModel->addMaterialsConfigurations(configurationMaterial);
    configurationMaterial = CMEgoeScene::createTempConfigurationMaterial("ws.reflection");
    configurationMaterial->setEnabled(false);
    configurationModel->addMaterialsConfigurations(configurationMaterial);
    configurationMaterial = CMEgoeScene::createTempConfigurationMaterial("ws.refraction");
    configurationMaterial->setEnabled(false);
    configurationModel->addMaterialsConfigurations(configurationMaterial);
    configurationMaterial = CMEgoeScene::createTempConfigurationMaterial("ws.shadowing");
    configurationMaterial->setEnabled(false);
    configurationModel->addMaterialsConfigurations(configurationMaterial);
    
    std::string path = executablepath();
    
    configurationModel->setFilename(path + "gameobject.model.goe.temp.xml");
    configurationModel->deserialize(path + "gameobject.model.goe.temp.xml");
    
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
        m_model = m_root->createModel(path + "gameobject.model.goe.temp.xml");
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
