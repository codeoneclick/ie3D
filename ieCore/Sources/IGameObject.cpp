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
#include "CAABoundBox.h"

IGameObject::IGameObject(const std::shared_ptr<CResourceAccessor>& _resourceAccessor, const std::shared_ptr<IScreenSpaceTextureAccessor>& _screenSpaceTextureAccessor) :
m_resourceAccessor(_resourceAccessor),
m_screenSpaceTextureAccessor(_screenSpaceTextureAccessor),
m_position(glm::vec3(0.0f, 0.0f, 0.0f)),
m_rotation(glm::vec3(0.0f, 0.0f, 0.0f)),
m_scale(glm::vec3(1.0f, 1.0f, 1.0f)),
m_renderQueuePosition(0),
m_mesh(nullptr),
m_template(nullptr),
m_camera(nullptr),
m_boundBox(nullptr),
m_debugBoundBoxMaterial(nullptr),
m_renderMgr(nullptr),
m_sceneUpdateMgr(nullptr),
m_materialImposer(nullptr),
m_isNeedToRender(false),
m_isNeedToUpdate(false),
m_isBatching(false),
m_status(E_LOADING_STATUS_UNLOADED)
{
    std::for_each(m_lights.begin(), m_lights.end(), [](std::shared_ptr<CLight> _light)
                  {
                      _light = nullptr;
                  });
}

IGameObject::~IGameObject(void)
{
    m_materials.clear();
}

bool IGameObject::_IsBoundBoxInFrustum(void)
{
    assert(m_camera != nullptr);
    assert(m_camera->Get_Frustum() != nullptr);
    assert(m_mesh != nullptr);
    i32 result =  m_camera->Get_Frustum()->IsBoundBoxInFrustum(m_mesh->Get_MaxBound() + m_position, m_mesh->Get_MinBound() + m_position);
    return result == E_FRUSTUM_BOUND_RESULT_OUTSIDE ? false : true;
}

glm::vec3 IGameObject::Get_MaxBound(void)
{
    assert(m_mesh != nullptr);
    return m_mesh->Get_MaxBound() * m_scale;
}

glm::vec3 IGameObject::Get_MinBound(void)
{
    assert(m_mesh != nullptr);
    return m_mesh->Get_MinBound() * m_scale;
}

void IGameObject::Set_Camera(std::shared_ptr<CCamera> _camera)
{
    m_camera = _camera;
}

void IGameObject::Set_Texture(std::shared_ptr<CTexture> _texture, E_SHADER_SAMPLER _sampler, const std::string& _renderMode)
{
    assert(m_materials.find(_renderMode) != m_materials.end());
    auto iterator = m_materials.find(_renderMode);
    iterator->second->Set_Texture(_texture, _sampler);
}

void IGameObject::Set_ClippingPlane(const glm::vec4 &_clippingPlane, const std::string& _renderMode)
{
    assert(m_materials.find(_renderMode) != m_materials.end());
    auto iterator = m_materials.find(_renderMode);
    iterator->second->Set_ClippingPlane(_clippingPlane);
}

std::shared_ptr<CVertexBuffer> IGameObject::Get_HardwareVertexBuffer(void)
{
    assert(m_mesh != nullptr);
    assert(m_mesh->Get_VertexBuffer() != nullptr);
    return m_mesh->Get_VertexBuffer();
}

std::shared_ptr<CIndexBuffer> IGameObject::Get_HardwareIndexBuffer(void)
{
    assert(m_mesh != nullptr);
    assert(m_mesh->Get_IndexBuffer() != nullptr);
    return m_mesh->Get_IndexBuffer();
}

std::shared_ptr<CVertexBuffer> IGameObject::Get_BoundVertexBuffer(void)
{
    assert(m_boundBox != nullptr);
    assert(m_boundBox->Get_VertexBuffer() != nullptr);
    return m_boundBox->Get_VertexBuffer();
}

std::shared_ptr<CIndexBuffer> IGameObject::Get_BoundIndexBuffer(void)
{
    assert(m_boundBox != nullptr);
    assert(m_boundBox->Get_IndexBuffer() != nullptr);
    return m_boundBox->Get_IndexBuffer();
}

ui32 IGameObject::Get_NumTriangles(void)
{
    return m_mesh != nullptr && m_mesh->Get_IndexBuffer() != nullptr ? m_mesh->Get_NumIndexes() / 3 : 0;
}

void IGameObject::ListenRenderMgr(bool _value)
{
	if(_value)
	{
		assert(m_renderMgr != nullptr);
	}

    for(const auto& iterator : m_materials)
    {
		if(m_renderMgr != nullptr)
		{
			_value == true ? m_renderMgr->RegisterWorldSpaceRenderHandler(iterator.first, shared_from_this()) :
			m_renderMgr->UnregisterWorldSpaceRenderHandler(iterator.first, shared_from_this());
		}
    }
	m_isNeedToRender = _value;
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
        m_mesh->Bind(iterator->second->Get_Shader()->Get_Attributes());
    }
}

void IGameObject::_OnDraw(const std::string &_mode)
{
    assert(m_mesh != nullptr);
    m_mesh->Draw();
}

void IGameObject::_OnUnbind(const std::string &_mode)
{
    assert(m_materials.find(_mode) != m_materials.end());
    auto iterator = m_materials.find(_mode);
    
    if(!m_isBatching)
    {
        assert(m_mesh != nullptr);
        iterator->second->Unbind();
        m_mesh->Unbind(iterator->second->Get_Shader()->Get_Attributes());
    }
}

void IGameObject::_OnDebugDraw(const std::string &_mode)
{

}

