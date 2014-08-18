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
#include "CConfigurationGameObjects.h"

IGameObject::IGameObject(CSharedResourceAccessorRef resourceAccessor,
                         ISharedRenderTechniqueAccessorRef renderTechniqueAccessor) :
m_resourceAccessor(resourceAccessor),
m_position(glm::vec3(0.0f, 0.0f, 0.0f)),
m_rotation(glm::vec3(0.0f, 0.0f, 0.0f)),
m_scale(glm::vec3(1.0f, 1.0f, 1.0f)),
m_zOrder(0),
m_mesh(nullptr),
m_configuration(nullptr),
m_camera(nullptr),
m_boundBox(nullptr),
m_renderTechniqueAccessor(renderTechniqueAccessor),
m_renderTechniqueImporter(nullptr),
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
    CSharedConfigurationGameObject gameObjectConfiguration = std::static_pointer_cast<CConfigurationGameObject>(configuration);
    for(const auto& iterator : gameObjectConfiguration->getMaterialsConfigurations())
    {
        CSharedConfigurationMaterial materialConfiguration = std::static_pointer_cast<CConfigurationMaterial>(iterator);
        CSharedMaterial material = std::make_shared<CMaterial>();
        CMaterial::setupMaterial(material, materialConfiguration, m_resourceAccessor, m_renderTechniqueAccessor, shared_from_this());
        m_materials.insert(std::make_pair(materialConfiguration->getRenderOperationName(), material));
    }
}

i32  IGameObject::zOrder(void)
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

CSharedMaterial IGameObject::getMaterial(const std::string& renderTechique) const
{
    const auto& material = m_materials.find(renderTechique);
    assert(material != m_materials.end());
    return material->second;
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
                             const std::string& renderTechnique)
{
    if(renderTechnique.length() != 0)
    {
        assert(m_materials.find(renderTechnique) != m_materials.end());
        auto iterator = m_materials.find(renderTechnique);
        iterator->second->setTexture(texture, sampler);
    }
    else
    {
        for(const auto& iterator : m_materials)
        {
            iterator.second->setTexture(texture, sampler);
        }
    }
    texture->addLoadingHandler(shared_from_this());
}

void IGameObject::removeLoadingDependencies(void)
{
    for(const auto& iterator : m_materials)
    {
        for(ui32 i = E_SHADER_SAMPLER_01; i < E_SHADER_SAMPLER_MAX; ++i)
        {
            CSharedTexture texture = iterator.second->getTexture(static_cast<E_SHADER_SAMPLER>(i));
            if(texture != nullptr)
            {
                texture->removeLoadingHandler(shared_from_this());
            }
            
            CSharedShader shader = iterator.second->getShader();
            if(shader != nullptr)
            {
                shader->removeLoadingHandler(shared_from_this());
            }
        }
    }
    if(m_mesh != nullptr)
    {
        m_mesh->removeLoadingHandler(shared_from_this());
    }
}

void IGameObject::setClippingPlane(const glm::vec4& clippingPlane,
                      const std::string& renderTechnique)
{
    assert(m_materials.find(renderTechnique) != m_materials.end());
    auto iterator = m_materials.find(renderTechnique);
    iterator->second->setClippingPlane(clippingPlane);
}

void IGameObject::setRenderTechniqueImporter(ISharedRenderTechniqueImporterRef techniqueImporter)
{
    m_renderTechniqueImporter = techniqueImporter;
}

void IGameObject::setRenderTechniqueAccessor(ISharedRenderTechniqueAccessorRef techniqueAccessor)
{
    m_renderTechniqueAccessor = techniqueAccessor;
}

void IGameObject::setSceneUpdateMgr(CSharedSceneUpdateMgrRef sceneUpdateMgr)
{
    m_sceneUpdateMgr = sceneUpdateMgr;
}

void IGameObject::enableRender(bool value)
{
    for(const auto& iterator : m_materials)
    {
		if(m_renderTechniqueImporter != nullptr)
		{
			value == true ? m_renderTechniqueImporter->addRenderTechniqueHandler(iterator.first, shared_from_this()) :
			m_renderTechniqueImporter->removeRenderTechniqueHandler(iterator.first, shared_from_this());
		}
    }
	m_isNeedToRender = value;
}

void IGameObject::enableUpdate(bool value)
{
    if(m_sceneUpdateMgr != nullptr)
    {
        value == true ? m_sceneUpdateMgr->RegisterSceneUpdateHandler(shared_from_this()) :
        m_sceneUpdateMgr->UnregisterSceneUpdateHandler(shared_from_this());
    }
    m_isNeedToUpdate = value;
}
