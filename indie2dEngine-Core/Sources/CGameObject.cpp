//
//  CGameObject.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/24/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGameObject.h"
#include "CCamera.h"
#include "CFrustum.h"
#include "CShape.h"
#include "CShader.h"
#include "CMaterial.h"
#include "CRenderMgr.h"
#include "CSceneUpdateMgr.h"

CGameObject::CGameObject(void) :
m_position(glm::vec3(0.0f, 0.0f, 0.0f)),
m_rotation(glm::vec3(0.0f, 0.0f, 0.0f)),
m_scale(glm::vec3(1.0f, 1.0f, 1.0f)),
m_texcoordDisplacement(glm::vec2(0.0f, 0.0f)),
m_shape(nullptr),
m_camera(nullptr),
m_light(nullptr),
m_renderMgr(nullptr),
m_sceneUpdateMgr(nullptr)
{

}

CGameObject::~CGameObject(void)
{
    m_materials.clear();
}

bool CGameObject::_IsBoundBoxInFrustum(void)
{
    assert(m_camera != nullptr);
    assert(m_camera->Get_Frustum() != nullptr);
    assert(m_shape != nullptr);
    i32 result =  m_camera->Get_Frustum()->IsBoundBoxInFrustum(m_shape->Get_MaxBound() + m_position, m_shape->Get_MinBound() + m_position);
    return result == E_FRUSTUM_BOUND_RESULT_OUTSIDE ? false : true;
}

glm::vec3 CGameObject::Get_MaxBound(void)
{
    assert(m_shape != nullptr);
    return m_shape->Get_MaxBound();
}

glm::vec3 CGameObject::Get_MinBound(void)
{
    assert(m_shape != nullptr);
    return m_shape->Get_MinBound();
}

void CGameObject::Set_Texture(std::shared_ptr<CTexture> _texture, E_SHADER_SAMPLER _sampler, const std::string& _renderMode)
{
    assert(m_materials.find(_renderMode) != m_materials.end());
    auto iterator = m_materials.find(_renderMode);
    iterator->second->Set_Texture(_texture, _sampler);
}

void CGameObject::Set_Clipping(const glm::vec4 &_clipping, const std::string& _renderMode)
{
    assert(m_materials.find(_renderMode) != m_materials.end());
    auto iterator = m_materials.find(_renderMode);
    iterator->second->Set_Clipping(_clipping);
}

void CGameObject::ListenRenderMgr(bool _value)
{
    assert(m_renderMgr != nullptr);
    for(auto iterator : m_materials)
    {
        IRenderHandler* handler = static_cast<IRenderHandler*>(this);
        _value == true ? m_renderMgr->RegisterWorldSpaceRenderHandler(iterator.first, handler->shared_from_this()) :
        m_renderMgr->UnregisterWorldSpaceRenderHandler(iterator.first, handler->shared_from_this());
    }
}

void CGameObject::ListenSceneUpdateMgr(bool _value)
{
    assert(m_sceneUpdateMgr != nullptr);
    ISceneUpdateHandler* handler = static_cast<ISceneUpdateHandler*>(this);
    _value == true ? m_sceneUpdateMgr->RegisterSceneUpdateHandler(handler->shared_from_this()) :
    m_sceneUpdateMgr->UnregisterSceneUpdateHandler(handler->shared_from_this());
}


void CGameObject::_OnSceneUpdate(f32 _deltatime)
{
    m_matrixRotation = glm::rotate(glm::mat4(1.0f), m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    m_matrixRotation = glm::rotate(m_matrixRotation, m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    m_matrixRotation = glm::rotate(m_matrixRotation, m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    m_matrixTranslation = glm::translate(glm::mat4(1.0f), m_position);
    m_matrixScale = glm::scale(glm::mat4(1.0f), m_scale);
    m_matrixWorld = m_matrixTranslation * m_matrixRotation * m_matrixScale;
}

i32 CGameObject::_OnQueuePosition(void)
{
    return NULL;
}

void CGameObject::_OnBind(const std::string &_renderMode)
{
    assert(m_materials.find(_renderMode) != m_materials.end());
    auto iterator = m_materials.find(_renderMode);
    iterator->second->Bind();
    
    assert(m_shape != nullptr);
    m_shape->Bind(iterator->second->Get_Shader()->Get_Attributes());
}

void CGameObject::_OnDraw(const std::string &_renderMode)
{
    assert(m_shape != nullptr);
    m_shape->Draw();
}

void CGameObject::_OnUnbind(const std::string &_renderMode)
{
    assert(m_materials.find(_renderMode) != m_materials.end());
    auto iterator = m_materials.find(_renderMode);
    iterator->second->Unbind();
    
    assert(m_shape != nullptr);
    m_shape->Unbind(iterator->second->Get_Shader()->Get_Attributes());
}