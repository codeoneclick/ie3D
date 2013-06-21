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

IGameObject::IGameObject(std::shared_ptr<CResourceAccessor> _resourceFabricator) :
m_resourceFabricator(_resourceFabricator),
m_position(glm::vec3(0.0f, 0.0f, 0.0f)),
m_rotation(glm::vec3(0.0f, 0.0f, 0.0f)),
m_scale(glm::vec3(1.0f, 1.0f, 1.0f)),
m_texcoordDisplacement(glm::vec2(0.0f, 0.0f)),
m_mesh(nullptr),
m_camera(nullptr),
m_light(nullptr),
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
}

i32 IGameObject::_OnQueuePosition(void)
{
    return NULL;
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
}

