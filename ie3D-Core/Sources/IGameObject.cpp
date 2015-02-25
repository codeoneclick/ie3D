
//
//  IGameObject.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/24/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IGameObject.h"
#include "CCamera.h"
#include "CFrustum.h"
#include "CMesh.h"
#include "CShader.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "IRenderTechniqueImporter.h"
#include "IRenderTechniqueAccessor.h"
#include "CSceneUpdateMgr.h"
#include "CResourceAccessor.h"
#include "CGlobalLightSource.h"
#include "CBatchingMgr.h"
#include "CBoundingBox.h"
#include "HShaders.h"
#include "CComponentTransformation.h"
#include "CComponentRendering.h"
#include "CComponentDebugRendering.h"
#include "CComponentSceneUpdate.h"
#include "CConfigurationAccessor.h"
#include "CResourceAccessor.h"

IGameObject::IGameObject(CSharedResourceAccessorRef resourceAccessor,
                         ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
m_resourceAccessor(resourceAccessor),
m_mesh(nullptr),
m_camera(nullptr),
m_globalLightSource(nullptr),
m_renderTechniqueAccessor(renderTechniqueAccessor),
m_renderTechniqueImporter(nullptr),
m_sceneUpdateMgr(nullptr),
m_materialBindImposer(nullptr),
m_status(E_LOADING_STATUS_UNLOADED),
m_isVisible(true)
{
    m_materialBindImposer = [this](CSharedMaterialRef material)
    {
        bindBaseShaderUniforms(material);
        bindCustomShaderUniforms(material);
    };
    
    CSharedComponentTransformation componentTransformation = std::make_shared<CComponentTransformation>();
    IGameObject::addComponent(componentTransformation);
}

IGameObject::~IGameObject(void)
{
    IGameObject::removeComponents();
}

void IGameObject::addComponent(ISharedComponentRef component)
{
    assert(component != nullptr && component->getClass() != E_COMPONENT_CLASS_UNDEFINED);
    m_components.insert(std::make_pair(component->getClass(), component));
}

void IGameObject::removeComponent(ISharedComponentRef component)
{
    assert(component != nullptr && component->getClass() != E_COMPONENT_CLASS_UNDEFINED);
    auto iterator = m_components.find(component->getClass());
    assert(iterator != m_components.end());
    m_components.erase(iterator);
}

void IGameObject::removeComponents(void)
{
    std::map<E_COMPONENT_CLASS, ISharedComponent> eraser;
    m_components.swap(eraser);
}

bool IGameObject::isComponentExist(E_COMPONENT_CLASS componentClass) const
{
    assert(componentClass != E_COMPONENT_CLASS_UNDEFINED);
    auto iterator = m_components.find(componentClass);
    return iterator != m_components.end();
}

ISharedComponent IGameObject::getComponent(E_COMPONENT_CLASS componentClass) const
{
    assert(componentClass != E_COMPONENT_CLASS_UNDEFINED);
    auto iterator = m_components.find(componentClass);
    return iterator != m_components.end() ? iterator->second : nullptr;
}

void IGameObject::addComponentRendering(void)
{
    if(m_renderTechniqueImporter && m_configuration && !IGameObject::isComponentExist(E_COMPONENT_CLASS_RENDERING))
    {
        ISharedConfigurationGameObject configurationGameObject = std::static_pointer_cast<IConfigurationGameObject>(m_configuration);
        CSharedComponentRendering componentRendering = std::make_shared<CComponentRendering>(configurationGameObject,
                                                                                             m_resourceAccessor,
                                                                                             m_renderTechniqueAccessor,
                                                                                             shared_from_this(),
                                                                                             m_cameraFrustum);
        componentRendering->setDrawCommand(std::bind(&IGameObject::onDraw, this, std::placeholders::_1));
        componentRendering->setCheckInCameraFrustumCommand(std::bind(&IGameObject::isInCameraFrustum, this, std::placeholders::_1));
        IGameObject::addComponent(componentRendering);
        for(const auto& iterator : configurationGameObject->getMaterialsConfigurations())
        {
            CSharedConfigurationMaterial configurationMaterial = std::static_pointer_cast<CConfigurationMaterial>(iterator);
            if(m_renderTechniqueImporter->isSupportingRenderTechnique(configurationMaterial->getRenderTechniqueName()))
            {
                m_renderTechniqueImporter->addRenderTechniqueHandler(configurationMaterial->getRenderTechniqueName(), shared_from_this());
            }
        };
    }
}

void IGameObject::removeComponentRendering(void)
{
    if(m_renderTechniqueImporter && m_configuration)
    {
        ISharedConfigurationGameObject configurationGameObject = std::static_pointer_cast<IConfigurationGameObject>(m_configuration);
        for(const auto& iterator : configurationGameObject->getMaterialsConfigurations())
        {
            CSharedConfigurationMaterial configurationMaterial = std::static_pointer_cast<CConfigurationMaterial>(iterator);
            if(m_renderTechniqueImporter->isSupportingRenderTechnique(configurationMaterial->getRenderTechniqueName()))
            {
                m_renderTechniqueImporter->removeRenderTechniqueHandler(configurationMaterial->getRenderTechniqueName(), shared_from_this());
            }
        };
    }
    if(IGameObject::isComponentExist(E_COMPONENT_CLASS_RENDERING))
    {
        IGameObject::removeComponent(IGameObject::getComponent(E_COMPONENT_CLASS_RENDERING));
    }
}

void IGameObject::addComponentDebugRendering(bool force)
{
    if(m_renderTechniqueImporter && !IGameObject::isComponentExist(E_COMPONENT_CLASS_DEBUG_RENDERING))
    {
        CSharedComponentRendering componentRendering = IGameObject::getComponentRendering();
        if(((componentRendering &&
             componentRendering->getMaterial("ws.base") &&
             componentRendering->getMaterial("ws.base")->isDebugging()) || force) &&
           m_mesh && m_mesh->isLoaded())
        {
            CSharedComponentDebugRendering componentDebugRendering = std::make_shared<CComponentDebugRendering>(m_resourceAccessor,
                                                                                                                m_renderTechniqueAccessor,
                                                                                                                m_cameraFrustum,
                                                                                                                m_mesh->getMinBound(),
                                                                                                                m_mesh->getMaxBound(),
                                                                                                                m_materialBindImposer);
            componentDebugRendering->setCheckInCameraFrustumCommand(std::bind(&IGameObject::isInCameraFrustum, this, std::placeholders::_1));
            IGameObject::addComponent(componentDebugRendering);
        }
    }
}

void IGameObject::removeComponentDebugRendering(void)
{
    if(IGameObject::isComponentExist(E_COMPONENT_CLASS_DEBUG_RENDERING))
    {
        IGameObject::removeComponent(IGameObject::getComponent(E_COMPONENT_CLASS_DEBUG_RENDERING));
    }
}

void IGameObject::addComponentSceneUpdate(void)
{
    if(m_sceneUpdateMgr && !IGameObject::isComponentExist(E_COMPONENT_CLASS_SCENE_UPDATE))
    {
        CSharedComponentSceneUpdate componentSceneUpdate = std::make_shared<CComponentSceneUpdate>();
        componentSceneUpdate->setSceneUpdateCommand(std::bind(&IGameObject::onSceneUpdate, this, std::placeholders::_1));
        IGameObject::addComponent(componentSceneUpdate);
        
        m_sceneUpdateMgr->RegisterSceneUpdateHandler(shared_from_this());
    }
}

void IGameObject::removeComponentSceneUpdate(void)
{
    if(m_sceneUpdateMgr)
    {
        m_sceneUpdateMgr->UnregisterSceneUpdateHandler(shared_from_this());
    }
    if(IGameObject::isComponentExist(E_COMPONENT_CLASS_SCENE_UPDATE))
    {
        IGameObject::removeComponent(IGameObject::getComponent(E_COMPONENT_CLASS_SCENE_UPDATE));
    }
}

void IGameObject::onSceneUpdate(f32 deltatime)
{

}

void IGameObject::onResourceLoaded(ISharedResourceRef resource,
                                   bool success)
{
    IResourceLoadingHandler::onResourceLoaded(resource, success);
}

void IGameObject::onConfigurationLoaded(ISharedConfigurationRef configuration,
                                        bool success)
{
    IConfigurationLoadingHandler::onConfigurationLoaded(configuration, success);
    IGameObject::addComponentRendering();
    
    if(m_sceneUpdateMgr)
    {
        m_sceneUpdateMgr->RegisterSceneUpdateHandler(shared_from_this());
    }
}

void IGameObject::onBind(CSharedMaterialRef material)
{
    material->bind();
    m_mesh->bind(material->getShader()->getAttributes());
    m_materialBindImposer(material);
}

void IGameObject::onUnbind(CSharedMaterialRef material)
{
    material->unbind();
    m_mesh->unbind(material->getShader()->getAttributes());
}

void IGameObject::onDraw(CSharedMaterialRef material)
{
    assert(m_mesh != nullptr);
    assert(m_camera != nullptr);
    assert(m_globalLightSource != nullptr);
    
    if(material->getShader()->isLoaded()
#if defined(__EDITOR__)
       && material->getEnabled()
#endif
       )
    {
        onBind(material);
        m_mesh->draw();
        onUnbind(material);
    }
}

bool IGameObject::isInCameraFrustum(CSharedFrustumRef cameraFrustum)
{
    assert(cameraFrustum != nullptr);
    glm::vec3 maxBound = IGameObject::getMaxBound() + IGameObject::getPosition();
    glm::vec3 minBound = IGameObject::getMinBound() + IGameObject::getPosition();
    return cameraFrustum->isBoundBoxInFrustum(maxBound, minBound) && m_isVisible;
}

CSharedComponentRendering IGameObject::getComponentRendering(void) const
{
    CSharedComponentRendering componentRendering = nullptr;
    if(IGameObject::isComponentExist(E_COMPONENT_CLASS_RENDERING))
    {
        componentRendering = std::static_pointer_cast<CComponentRendering>(m_components.at(E_COMPONENT_CLASS_RENDERING));
    }
    return componentRendering;
}

CSharedComponentDebugRendering IGameObject::getComponentDebugRendering(void) const
{
    CSharedComponentDebugRendering componentDebugRendering = nullptr;
    if(IGameObject::isComponentExist(E_COMPONENT_CLASS_DEBUG_RENDERING))
    {
        componentDebugRendering = std::static_pointer_cast<CComponentDebugRendering>(IGameObject::getComponent(E_COMPONENT_CLASS_DEBUG_RENDERING));
    }
    return componentDebugRendering;
}

void IGameObject::bindBaseShaderUniforms(CSharedMaterialRef material)
{
    assert(material != nullptr);
    
    // base matrices
    material->getShader()->setMatrix4x4(IGameObject::getMMatrix(), E_SHADER_UNIFORM_MATRIX_M);
    material->getShader()->setMatrix4x4(m_camera->getPMatrix(), E_SHADER_UNIFORM_MATRIX_P);
    material->getShader()->setMatrix4x4(!material->isReflecting() ? m_camera->getVMatrix() : m_camera->getIVMatrix(), E_SHADER_UNIFORM_MATRIX_V);
    material->getShader()->setMatrix4x4(!material->isReflecting() ? m_camera->getVPMatrix() : m_camera->getIVPMatrix(), E_SHADER_UNIFORM_MATRIX_VP);
    material->getShader()->setMatrix4x4(!material->isReflecting() ? IGameObject::getMVPMatrix() : IGameObject::getIMVPMatrix(), E_SHADER_UNIFORM_MATRIX_MVP);
    material->getShader()->setMatrix4x4(m_camera->getNMatrix(), E_SHADER_UNIFORM_MATRIX_N);
    
    // camera base parameters
    material->getShader()->setVector3(m_camera->getPosition(), E_SHADER_UNIFORM_VECTOR_CAMERA_POSITION);
    material->getShader()->setFloat(m_camera->getNear(), E_SHADER_UNIFORM_FLOAT_CAMERA_NEAR);
    material->getShader()->setFloat(m_camera->getFar(), E_SHADER_UNIFORM_FLOAT_CAMERA_FAR);
    material->getShader()->setVector4(material->getClippingPlane(), E_SHADER_UNIFORM_VECTOR_CLIP_PLANE);
    
    // global light parameters
    material->getShader()->setVector3(m_globalLightSource->getPosition(),
                                      E_SHADER_UNIFORM_VECTOR_GLOBAL_LIGHT_POSITION);
    material->getShader()->setMatrix4x4(m_globalLightSource->getProjectionMatrix(),
                                        E_SHADER_UNIFORM_MATRIX_GLOBAL_LIGHT_PROJECTION);
    material->getShader()->setMatrix4x4(m_globalLightSource->getViewMatrix(),
                                        E_SHADER_UNIFORM_MATRIX_GLOBAL_LIGHT_VIEW);
}

void IGameObject::bindCustomShaderUniforms(CSharedMaterialRef material)
{
    assert(material != nullptr);
    std::map<std::string, CSharedShaderUniform> customShaderUniforms = material->getCustomUniforms();
    CSharedShaderUniform currentUniform = nullptr;
    for(const auto& iterator : customShaderUniforms)
    {
        currentUniform = iterator.second;
        assert(currentUniform != nullptr);
        switch (currentUniform->getClass())
        {
            case E_UNIFORM_CLASS_MAT4X4:
            {
                material->getShader()->setMatrix4x4Custom(currentUniform->getMatrix4x4(), iterator.first);
            }
                break;
                
            case E_UNIFORM_CLASS_MAT3X3:
            {
                material->getShader()->setMatrix3x3Custom(currentUniform->getMatrix3x3(), iterator.first);
            }
                break;
                
            case E_UNIFORM_CLASS_VECTOR4:
            {
                material->getShader()->setVector4Custom(currentUniform->getVector4(), iterator.first);
            }
                break;
                
            case E_UNIFORM_CLASS_VECTOR3:
            {
                 material->getShader()->setVector3Custom(currentUniform->getVector3(), iterator.first);
            }
                break;
                
            case E_UNIFORM_CLASS_VECTOR2:
            {
                material->getShader()->setVector2Custom(currentUniform->getVector2(), iterator.first);
            }
                break;
                
            case E_UNIFORM_CLASS_FLOAT:
            {
                material->getShader()->setFloatCustom(currentUniform->getFloat(), iterator.first);
            }
                break;
                
            case E_UNIFORM_CLASS_INT:
            {
                material->getShader()->setIntCustom(currentUniform->getInt(), iterator.first);
            }
                break;
                
            default:
                break;
        }
    }
}

void IGameObject::setPosition(const glm::vec3& position)
{
    CSharedComponentTransformation component = std::static_pointer_cast<CComponentTransformation>(m_components.at(E_COMPONENT_CLASS_TRANSFORMATION));
    component->setPosition(position);
}

glm::vec3 IGameObject::getPosition(void) const
{
    CSharedComponentTransformation component = std::static_pointer_cast<CComponentTransformation>(m_components.at(E_COMPONENT_CLASS_TRANSFORMATION));
    return component->getPosition();
}

void IGameObject::setRotation(const glm::vec3& rotation)
{
    CSharedComponentTransformation component = std::static_pointer_cast<CComponentTransformation>(m_components.at(E_COMPONENT_CLASS_TRANSFORMATION));
    component->setRotation(rotation);
}

glm::vec3 IGameObject::getRotation(void) const
{
    CSharedComponentTransformation component = std::static_pointer_cast<CComponentTransformation>(m_components.at(E_COMPONENT_CLASS_TRANSFORMATION));
    return component->getRotation();
}

void IGameObject::setScale(const glm::vec3& scale)
{
    CSharedComponentTransformation component = std::static_pointer_cast<CComponentTransformation>(m_components.at(E_COMPONENT_CLASS_TRANSFORMATION));
    component->setScale(scale);
}

glm::vec3 IGameObject::getScale(void) const
{
    CSharedComponentTransformation component = std::static_pointer_cast<CComponentTransformation>(m_components.at(E_COMPONENT_CLASS_TRANSFORMATION));
    return component->getScale();
}

glm::mat4x4 IGameObject::getMMatrix(void) const
{
    CSharedComponentTransformation component = std::static_pointer_cast<CComponentTransformation>(m_components.at(E_COMPONENT_CLASS_TRANSFORMATION));
    return CComponentTransformation::getMMatrix(component);
}

glm::mat4x4 IGameObject::getMVPMatrix(void) const
{
    CSharedComponentTransformation component = std::static_pointer_cast<CComponentTransformation>(m_components.at(E_COMPONENT_CLASS_TRANSFORMATION));
    return CComponentTransformation::getMVPMatrix(component, m_camera);
}

glm::mat4  IGameObject::getIMVPMatrix(void) const
{
    CSharedComponentTransformation component = std::static_pointer_cast<CComponentTransformation>(m_components.at(E_COMPONENT_CLASS_TRANSFORMATION));
    return CComponentTransformation::getIMVPMatrix(component, m_camera);
}

glm::vec3 IGameObject::getMaxBound(void) const
{
    return m_mesh && m_mesh->isLoaded() ? m_mesh->getMaxBound() : std::move(glm::vec3(0.0f));
}

glm::vec3 IGameObject::getMinBound(void) const
{
    return m_mesh && m_mesh->isLoaded() ? m_mesh->getMinBound() : std::move(glm::vec3(0.0f));
}

void IGameObject::setVisible(bool value)
{
    m_isVisible = value;
}

bool IGameObject::isVisible(void) const
{
    return m_isVisible;
}

void IGameObject::setCamera(CSharedCameraRef camera)
{
    m_camera = camera;
}

void IGameObject::setCameraFrustum(CSharedFrustumRef frustum)
{
    m_cameraFrustum = frustum;
}

void IGameObject::setGlobalLightSource(CSharedGlobalLightSourceRef lightSource)
{
    m_globalLightSource = lightSource;
}

CSharedVertexBuffer IGameObject::getVertexBuffer(void) const
{
    return m_mesh && m_mesh->isCommited() ? m_mesh->getVertexBuffer() : nullptr;
}

CSharedIndexBuffer IGameObject::getIndexBuffer(void) const
{
    return m_mesh && m_mesh->isCommited() ? m_mesh->getIndexBuffer() : nullptr;
}

CSharedMaterial IGameObject::getMaterial(const std::string& techniqueName) const
{
    CSharedMaterial material = nullptr;
    CSharedComponentRendering componentRendering = IGameObject::getComponentRendering();
    if (componentRendering)
    {
        material = componentRendering->getMaterial(techniqueName);
    }
    return material;
}

CSharedVertexBuffer IGameObject::getCollisionVertexBuffer(void) const
{
    return m_mesh ? m_mesh->getVertexBuffer() : nullptr;
}

CSharedIndexBuffer IGameObject::getCollisionIndexBuffer(void) const
{
    return m_mesh ? m_mesh->getIndexBuffer() : nullptr;
}

void IGameObject::setTexture(CSharedTextureRef texture, E_SHADER_SAMPLER sampler, const std::string& techniqueName)
{
    CSharedComponentRendering componentRendering = IGameObject::getComponentRendering();
    if (componentRendering)
    {
        componentRendering->setTexture(texture, sampler, shared_from_this(), techniqueName);
    }
}

void IGameObject::onAddedToScene(ISharedRenderTechniqueImporterRef techniqueImporter,
                                 CSharedSceneUpdateMgrRef sceneUpdateMgr)
{
    assert(techniqueImporter);
    assert(sceneUpdateMgr);
    
    m_renderTechniqueImporter = techniqueImporter;
    m_sceneUpdateMgr = sceneUpdateMgr;
    
    IGameObject::addComponentRendering();
    IGameObject::addComponentDebugRendering();
    IGameObject::addComponentSceneUpdate();
}

void IGameObject::onRemovedFromScene(void)
{
    IGameObject::removeComponentRendering();
    IGameObject::removeComponentDebugRendering();
    IGameObject::removeComponentSceneUpdate();
    
    assert(m_resourceAccessor);
    m_resourceAccessor->removeLoadingDependecies(shared_from_this());
    
    m_renderTechniqueImporter = nullptr;
    m_sceneUpdateMgr = nullptr;
}
