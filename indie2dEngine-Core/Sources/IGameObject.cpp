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
#include "CShape.h"
#include "CMesh.h"
#include "CShader.h"
#include "CMaterial.h"
#include "CRenderMgr.h"
#include "CSceneUpdateMgr.h"
#include "CAABoundBox.h"
#include "CFrustum.h"

IGameObject::IGameObject(std::shared_ptr<CResourceAccessor> _resourceFabricator) :
m_resourceFabricator(_resourceFabricator),
m_position(glm::vec3(0.0f, 0.0f, 0.0f)),
m_rotation(glm::vec3(0.0f, 0.0f, 0.0f)),
m_scale(glm::vec3(1.0f, 1.0f, 1.0f)),
m_texcoordDisplacement(glm::vec2(0.0f, 0.0f)),
m_mesh(nullptr),
m_camera(nullptr),
m_light(nullptr),
m_boundBox(nullptr),
m_debugBoundBoxMaterial(nullptr),
m_renderMgr(nullptr),
m_sceneUpdateMgr(nullptr),
m_isLoaded(false),
m_isLazyListening(false)
{

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
    return m_mesh->Get_MaxBound();
}

glm::vec3 IGameObject::Get_MinBound(void)
{
    assert(m_mesh != nullptr);
    return m_mesh->Get_MinBound();
}

void IGameObject::Set_Camera(std::shared_ptr<CCamera> _camera)
{
    assert(_camera != nullptr);
    m_camera = _camera;
};

void IGameObject::Set_Texture(std::shared_ptr<CTexture> _texture, E_SHADER_SAMPLER _sampler, const std::string& _renderMode)
{
    assert(m_materials.find(_renderMode) != m_materials.end());
    auto iterator = m_materials.find(_renderMode);
    iterator->second->Set_Texture(_texture, _sampler);
}

void IGameObject::Set_Clipping(const glm::vec4 &_clipping, const std::string& _renderMode)
{
    assert(m_materials.find(_renderMode) != m_materials.end());
    auto iterator = m_materials.find(_renderMode);
    iterator->second->Set_Clipping(_clipping);
}

std::shared_ptr<CVertexBuffer> IGameObject::Get_VertexBuffer(void)
{
    assert(m_mesh != nullptr);
    assert(m_mesh->Get_VertexBuffer() != nullptr);
    return m_mesh->Get_VertexBuffer();
};

std::shared_ptr<CIndexBuffer> IGameObject::Get_IndexBuffer(void)
{
    assert(m_mesh != nullptr);
    assert(m_mesh->Get_IndexBuffer() != nullptr);
    return m_mesh->Get_IndexBuffer();
};

std::shared_ptr<CVertexBuffer> IGameObject::Get_BoundVertexBuffer(void)
{
    assert(m_boundBox != nullptr);
    assert(m_boundBox->Get_VertexBuffer() != nullptr);
    return m_boundBox->Get_VertexBuffer();
};

std::shared_ptr<CIndexBuffer> IGameObject::Get_BoundIndexBuffer(void)
{
    assert(m_boundBox != nullptr);
    assert(m_boundBox->Get_IndexBuffer() != nullptr);
    return m_boundBox->Get_IndexBuffer();
};

ui32 IGameObject::Get_NumTriangles(void)
{
    return m_mesh != nullptr && m_mesh->Get_IndexBuffer() != nullptr ? m_mesh->Get_NumIndexes() / 3 : 0;
}

void IGameObject::ListenRenderMgr(bool _value)
{
    assert(m_renderMgr != nullptr);
    for(auto iterator : m_materials)
    {
        _value == true ? m_renderMgr->RegisterWorldSpaceRenderHandler(iterator.first, shared_from_this()) :
        m_renderMgr->UnregisterWorldSpaceRenderHandler(iterator.first, shared_from_this());
    }
    m_isLazyListening = _value && !m_materials.size();
}
void IGameObject::_LazyListenRenderMgr(void)
{
    assert(m_renderMgr != nullptr);
    for(auto iterator : m_materials)
    {
        m_renderMgr->RegisterWorldSpaceRenderHandler(iterator.first, shared_from_this());
    }
    m_isLazyListening = false;
}

void IGameObject::ListenSceneUpdateMgr(bool _value)
{
    assert(m_sceneUpdateMgr != nullptr);
    _value == true ? m_sceneUpdateMgr->RegisterSceneUpdateHandler(shared_from_this()) :
    m_sceneUpdateMgr->UnregisterSceneUpdateHandler(shared_from_this());
}

void IGameObject::_OnTemplateLoaded(std::shared_ptr<ITemplate> _template)
{
    assert(false);
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
    
    if(m_mesh != nullptr)
    {
        m_mesh->OnUpdate(_deltatime);
    }
}

i32 IGameObject::_OnQueuePosition(void)
{
    return NULL;
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

void IGameObject::_OnBind(const std::string &_renderMode)
{
    assert(m_materials.find(_renderMode) != m_materials.end());
    auto iterator = m_materials.find(_renderMode);
    iterator->second->Bind();
    
    assert(m_mesh != nullptr);
    m_mesh->Bind(iterator->second->Get_Shader()->Get_Attributes());
}

void IGameObject::_OnDraw(const std::string &_renderMode)
{
    assert(m_mesh != nullptr);
    m_mesh->Draw();
}

void IGameObject::_OnUnbind(const std::string &_renderMode)
{
    assert(m_materials.find(_renderMode) != m_materials.end());
    auto iterator = m_materials.find(_renderMode);
    iterator->second->Unbind();
    
    assert(m_mesh != nullptr);
    m_mesh->Unbind(iterator->second->Get_Shader()->Get_Attributes());
    
    if(m_boundBox != nullptr)
    {
        m_boundBox->Draw();
    }
}

void IGameObject::_OnDebugDraw(const std::string &_renderMode)
{
    assert(m_materials.find(_renderMode) != m_materials.end());
    auto iterator = m_materials.find(_renderMode);
    if(m_boundBox != nullptr && iterator->second->Get_IsDebug())
    {
        assert(m_debugBoundBoxMaterial != nullptr);
        assert(m_debugBoundBoxMaterial->Get_Shader() != nullptr);
        m_debugBoundBoxMaterial->Bind();
        m_debugBoundBoxMaterial->Get_Shader()->Set_Matrix4x4(glm::mat4x4(1.0f), E_SHADER_UNIFORM_MATRIX_WORLD);
        m_debugBoundBoxMaterial->Get_Shader()->Set_Matrix4x4(m_camera->Get_ProjectionMatrix(), E_SHADER_UNIFORM_MATRIX_PROJECTION);
        m_debugBoundBoxMaterial->Get_Shader()->Set_Matrix4x4(m_camera->Get_ViewMatrix(), E_SHADER_UNIFORM_MATRIX_VIEW);
        
        m_boundBox->Bind(m_debugBoundBoxMaterial->Get_Shader()->Get_Attributes());
        m_boundBox->Draw();
        m_boundBox->Unbind(m_debugBoundBoxMaterial->Get_Shader()->Get_Attributes());
        m_debugBoundBoxMaterial->Unbind();
    }
}

