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
#include "CMaterial.h"
#include "CRenderMgr.h"
#include "CSceneUpdateMgr.h"

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
    
}

void IGameObject::onResourceLoaded(ISharedResourceRef resource,
                                   bool success)
{
    
}

void IGameObject::onConfigurationLoaded(ISharedConfigurationRef configuration,
                                        bool success)
{
    
}

i32  IGameObject::getZOrder(void)
{
    
}

bool IGameObject::checkOcclusion(void)
{
    
}

ui32 IGameObject::numTriangles(void)
{
    return m_mesh && m_mesh->isLoaded() ? m_mesh->getNumIndices() / 3 : 0;
}

void IGameObject::onBind(const std::string& mode)
{
    
}

void IGameObject::onDraw(const std::string& mode)
{
    
}

void IGameObject::onUnbind(const std::string& mode)
{
    
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
    
}

void IGameObject::setSceneUpdateMgr(CSharedSceneUpdateMgrRef sceneUpdateMgr)
{
    
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
	m_isNeedToRender = _value;
}

void IGameObject::listenSceneUpdateMgr(bool value)
{
    
}

void IGameObject::ListenRenderMgr(bool _value)
{
	
}

void IGameObject::ListenSceneUpdateMgr(bool _value)
{
    if(m_sceneUpdateMgr != nullptr)
    {
        _value == true ? m_sceneUpdateMgr->RegisterSceneUpdateHandler(shared_from_this()) :
        m_sceneUpdateMgr->UnregisterSceneUpdateHandler(shared_from_this());
    }
    m_isNeedToUpdate = _value;
}

void IGameObject::_OnTemplateLoaded(std::shared_ptr<I_RO_TemplateCommon> _template)
{
    m_template = _template;
    for(TEMPLATE_LOADING_HANDLER handler : m_templateLoadingHandlers)
    {
        (*handler)(m_template);
    }
}

void IGameObject::_OnResourceLoaded(std::shared_ptr<IResource> _resource, bool _success)
{
    std::string resourcename = "resource";
    switch (_resource->Get_Class())
    {
        case E_RESOURCE_CLASS_SHADER:
        {
            m_status |= E_LOADING_STATUS_SHADER_LOADED;
            resourcename = "shader";
        }
            break;
        case E_RESOURCE_CLASS_MESH:
        {
            m_status |= E_LOADING_STATUS_MESH_LOADED;
            resourcename = "mesh";
        }
            break;
        case E_RESOURCE_CLASS_SKELETON:
        {
            m_status |= E_LOADING_STATUS_SKELETON_LOADED;
            resourcename = "skeleton";
        }
            break;
        case E_RESOURCE_CLASS_SEQUENCE:
        {
            m_status |= E_LOADING_STATUS_SEQUENCE_LOADED;
            resourcename = "sequence";
        }
            break;
        case E_RESOURCE_CLASS_TEXTURE:
        {
            resourcename = "texture";
        }
            break;
        case E_RESOURCE_CLASS_MATERIAL:
        {
            resourcename = "material";
        }
            break;

        default:
            break;
    }
    
    std::set<RESOURCE_LOADING_HANDLER> handlers = m_resourceLoadingHandlers[_resource->Get_Class()];
    for(RESOURCE_LOADING_HANDLER handler : handlers)
    {
        (*handler)(_resource);
    }
    std::cout<<"Loaded: "<<resourcename<<". Guid: "<<_resource->Get_Guid()<<std::endl;
}

void IGameObject::_OnSceneUpdate(f32 _deltatime)
{
    m_matrixRotation = glm::rotate(glm::mat4(1.0f), m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    m_matrixRotation = glm::rotate(m_matrixRotation, m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    m_matrixRotation = glm::rotate(m_matrixRotation, m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    m_matrixTranslation = glm::translate(glm::mat4(1.0f), m_position);
    m_matrixScale = glm::scale(glm::mat4(1.0f), m_scale);
    m_matrixWorld = m_matrixTranslation * m_matrixRotation * m_matrixScale;
    
    if(m_boundBox != nullptr)
    {
        m_boundBox->Update(m_matrixWorld);
    }
}

i32 IGameObject::_OnQueuePosition(void)
{
    return 0;
}

bool IGameObject::_OnOcclusion(void)
{
    assert(m_camera != nullptr);
    assert(m_camera->Get_Frustum() != nullptr);
    
    glm::vec3 maxBound = IGameObject::Get_MaxBound() + m_position;
    glm::vec3 minBound = IGameObject::Get_MinBound() + m_position;
    return !m_camera->Get_Frustum()->IsBoundBoxInFrustum(maxBound, minBound);
}

ui32 IGameObject::_OnGet_NumTriangles(void)
{
    return IGameObject::Get_NumTriangles();
}

void IGameObject::_OnBatch(const std::string& _mode)
{
    
}

void IGameObject::_OnBind(const std::string &_mode)
{
    assert(m_materials.find(_mode) != m_materials.end());
    auto iterator = m_materials.find(_mode);

    if(!m_isBatching)
    {
        assert(m_mesh != nullptr);
        iterator->second->Bind();
        m_mesh->bind(iterator->second->Get_Shader()->getAttributesRef());
    }
}

void IGameObject::_OnDraw(const std::string &_mode)
{
    assert(m_mesh != nullptr);
    m_mesh->draw();
}

void IGameObject::_OnUnbind(const std::string &_mode)
{
    assert(m_materials.find(_mode) != m_materials.end());
    auto iterator = m_materials.find(_mode);
    
    if(!m_isBatching)
    {
        assert(m_mesh != nullptr);
        iterator->second->Unbind();
        m_mesh->unbind(iterator->second->Get_Shader()->getAttributesRef());
    }
}

void IGameObject::_OnDebugDraw(const std::string &_mode)
{

}

