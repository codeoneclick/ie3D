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
#include "CRenderMgr.h"
#include "CSceneUpdateMgr.h"
#include "CResourceAccessor.h"
#include "IScreenSpaceTextureAccessor.h"
#include "CConfigurationGameObjects.h"

IGameObject::IGameObject(CSharedResourceAccessorRef resourceAccessor,
                         ISharedScreenSpaceTextureAccessorRef screenSpaceTextureAccessor) :
m_resourceAccessor(resourceAccessor),
m_screenSpaceTextureAccessor(screenSpaceTextureAccessor),
m_position(glm::vec3(0.0f, 0.0f, 0.0f)),
m_rotation(glm::vec3(0.0f, 0.0f, 0.0f)),
m_scale(glm::vec3(1.0f, 1.0f, 1.0f)),
m_renderQueuePosition(0),
m_mesh(nullptr),
m_configuration(nullptr),
m_camera(nullptr),
m_boundBox(nullptr),
m_renderMgr(nullptr),
m_sceneUpdateMgr(nullptr),
m_materialBindImposer(nullptr),
m_isNeedToRender(false),
m_isNeedToUpdate(false),
m_isBatching(false),
m_status(E_LOADING_STATUS_UNLOADED)
{
    std::for_each(m_lightSources.begin(), m_lightSources.end(), [](CSharedLightSource& lightSource){
        lightSource = nullptr;
    });
}

IGameObject::~IGameObject(void)
{
    m_materials.clear();
}

void IGameObject::onSceneUpdate(f32 deltatime)
{
    m_matrixWorld = m_matrixTranslation * m_matrixRotation * m_matrixScale;
    if(m_boundBox != nullptr)
    {

    }
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
    m_configuration = configuration;
}

i32  IGameObject::getZOrder(void)
{
    return 0;
}

bool IGameObject::checkOcclusion(void)
{
    assert(m_camera != nullptr);
    assert(m_camera->Get_Frustum() != nullptr);
    
    glm::vec3 maxBound = IGameObject::getMaxBound() + m_position;
    glm::vec3 minBound = IGameObject::getMinBound() + m_position;
    return !m_camera->Get_Frustum()->IsBoundBoxInFrustum(maxBound, minBound);
}

ui32 IGameObject::numTriangles(void)
{
    return m_mesh && m_mesh->isLoaded() ? m_mesh->getNumIndices() / 3 : 0;
}

void IGameObject::onBind(const std::string& mode)
{
    assert(m_materials.find(mode) != m_materials.end());
    auto iterator = m_materials.find(mode);
    
    if(!m_isBatching)
    {
        assert(m_mesh != nullptr);
        iterator->second->bind();
        m_mesh->bind(iterator->second->getShader()->getAttributesRef());
    }
}

void IGameObject::onDraw(const std::string& mode)
{
    assert(m_mesh != nullptr);
    m_mesh->draw();
}

void IGameObject::onUnbind(const std::string& mode)
{
    assert(m_materials.find(mode) != m_materials.end());
    auto iterator = m_materials.find(mode);
    
    if(!m_isBatching)
    {
        assert(m_mesh != nullptr);
        iterator->second->unbind();
        m_mesh->unbind(iterator->second->getShader()->getAttributesRef());
    }
}

void IGameObject::onBatch(const std::string& mode)
{
    
}

void IGameObject::setPosition(const glm::vec3& position)
{
    m_position = position;
    m_matrixTranslation = glm::translate(glm::mat4(1.0f), m_position);
}

glm::vec3 IGameObject::getPosition(void) const
{
    return m_position;
}

void IGameObject::setRotation(const glm::vec3& rotation)
{
    m_rotation = rotation;
    m_matrixRotation = glm::rotate(glm::mat4(1.0f), m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    m_matrixRotation = glm::rotate(m_matrixRotation, m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    m_matrixRotation = glm::rotate(m_matrixRotation, m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 IGameObject::getRotation(void) const
{
    return m_rotation;
}

void IGameObject::setScale(const glm::vec3& scale)
{
    m_scale = scale;
    m_matrixScale = glm::scale(glm::mat4(1.0f), m_scale);
}

glm::vec3 IGameObject::getScale(void) const
{
    return m_scale;
}

glm::mat4x4 IGameObject::getWorldMatrix(void) const
{
    return m_matrixWorld;
}

glm::vec3 IGameObject::getMaxBound(void) const
{
    return m_mesh && m_mesh->isLoaded() ? m_mesh->getMaxBound() * m_scale : glm::vec3(0, 0, 0);
}

glm::vec3 IGameObject::getMinBound(void) const
{
    return m_mesh && m_mesh->isLoaded() ? m_mesh->getMinBound() * m_scale : glm::vec3(0, 0, 0);
}

void IGameObject::setCamera(CSharedCameraRef camera)
{
    m_camera = camera;
}

void IGameObject::setLightSource(CSharedLightSourceRef lightSource,
                            E_LIGHT_SOURCE index)
{
    m_lightSources.at(index) = lightSource;
}

CSharedVertexBuffer IGameObject::getVertexBuffer(void) const
{
    return m_mesh && m_mesh->isCommited() ? m_mesh->getVertexBuffer() : nullptr;
}

CSharedIndexBuffer IGameObject::getIndexBuffer(void) const
{
    return m_mesh && m_mesh->isCommited() ? m_mesh->getIndexBuffer() : nullptr;
}

CSharedVertexBuffer IGameObject::getCollisionVertexBuffer(void) const
{
    return nullptr;
}

CSharedIndexBuffer IGameObject::getCollisionIndexBuffer(void) const
{
    return nullptr;
}

void IGameObject::setTexture(CSharedTextureRef texture,
                E_SHADER_SAMPLER sampler,
                const std::string& mode)
{
    assert(m_materials.find(mode) != m_materials.end());
    auto iterator = m_materials.find(mode);
    iterator->second->setTexture(texture, sampler);
}

void IGameObject::setClippingPlane(const glm::vec4& clippingPlane,
                      const std::string& mode)
{
    assert(m_materials.find(mode) != m_materials.end());
    auto iterator = m_materials.find(mode);
    iterator->second->setClippingPlane(clippingPlane);
}

void IGameObject::setRenderMgr(CSharedRenderMgrRef renderMgr)
{
    m_renderMgr = renderMgr;
}

void IGameObject::setSceneUpdateMgr(CSharedSceneUpdateMgrRef sceneUpdateMgr)
{
    m_sceneUpdateMgr = sceneUpdateMgr;
}

void IGameObject::listenRenderMgr(bool value)
{
    if(value)
	{
		assert(m_renderMgr != nullptr);
	}
    
    for(const auto& iterator : m_materials)
    {
		if(m_renderMgr != nullptr)
		{
			value == true ? m_renderMgr->RegisterWorldSpaceRenderHandler(iterator.first, shared_from_this()) :
			m_renderMgr->UnregisterWorldSpaceRenderHandler(iterator.first, shared_from_this());
		}
    }
	m_isNeedToRender = value;
}

void IGameObject::listenSceneUpdateMgr(bool value)
{
    if(m_sceneUpdateMgr != nullptr)
    {
        value == true ? m_sceneUpdateMgr->RegisterSceneUpdateHandler(shared_from_this()) :
        m_sceneUpdateMgr->UnregisterSceneUpdateHandler(shared_from_this());
    }
    m_isNeedToUpdate = value;
}

void IGameObject::setupMaterial(CSharedMaterialRef material,
                                CSharedConfigurationMaterialRef configuration)
{
    assert(configuration != nullptr);
    assert(m_screenSpaceTextureAccessor != nullptr);
	assert(m_resourceAccessor != nullptr);
    
    material->setCulling(configuration->isCulling());
    material->setCullingMode(configuration->getCullingMode());
    
    material->setBlending(configuration->isBlending());
    material->setBlendingFunctionSource(configuration->getBlendingFunctionSource());
    material->setBlendingFunctionDestination(configuration->getBlendingFunctionDestination());
    
    material->setDepthTest(configuration->isDepthTest());
    material->setDepthMask(configuration->isDepthMask());
    
    material->setClipping(configuration->isClipping());
    material->setClippingPlane(configuration->getClippingPlane());
    
    material->setReflecting(configuration->isReflecting());
    material->setShadowing(configuration->isShadowing());
    material->setDebugging(configuration->isDebugging());
    
    for(const auto& iterator : configuration->getTexturesConfigurations())
    {
        CSharedConfigurationTexture textureConfiguration = std::static_pointer_cast<CConfigurationTexture>(iterator);
        assert(textureConfiguration != nullptr);
        
        CSharedTexture texture = textureConfiguration->getFilename().length() != 0 ?
        m_resourceAccessor->getTexture(textureConfiguration->getFilename()) :
        m_screenSpaceTextureAccessor->Get_RenderOperationTexture(textureConfiguration->getRenderOperationName());
        texture->registerLoadingHandler(shared_from_this());
        assert(texture != nullptr);
        texture->setWrapMode(textureConfiguration->getWrapMode());
        assert(textureConfiguration->getSamplerIndex() >= 0 &&
               textureConfiguration->getSamplerIndex() < E_SHADER_SAMPLER_MAX);
        material->setTexture(texture, static_cast<E_SHADER_SAMPLER>(textureConfiguration->getSamplerIndex()));
    }
    
    CSharedConfigurationShader shaderConfiguration = std::static_pointer_cast<CConfigurationShader>(configuration->getShaderConfiguration());
    assert(shaderConfiguration != nullptr);
    CSharedShader shader = m_resourceAccessor->getShader(shaderConfiguration->getVSFilename(),
                                                         shaderConfiguration->getFSFilename());
    assert(shader != nullptr);
    material->setShader(shader);
    shader->registerLoadingHandler(shared_from_this());
}
