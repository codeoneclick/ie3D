//
//  CMaterial.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CMaterial.h"
#include "CShader.h"
#include "CTexture.h"
#include "CRenderMgr.h"
#include "CTemplateGameObjects.h"
#include "CResourceAccessor.h"
#include "IResourceLoadingHandler.h"

static bool s_states[E_RENDER_STATE_MAX] =
{
    false,
    false,
    true,
    true
};

CMaterial::CMaterial(std::shared_ptr<CShader> _shader, const std::string& _guid) :
IResource(E_RESOURCE_CLASS_MATERIAL, _guid),
m_shader(_shader),
m_guid(_guid),
m_isDebug(false),
m_isReflected(false),
m_isBatching(false)
{
    std::for_each(m_textures.begin(), m_textures.end(), [](std::shared_ptr<CTexture> _texture)
                  {
                      _texture = nullptr;
                  });
    
    m_states[E_RENDER_STATE_DEPTH_TEST] = true;
    m_states[E_RENDER_STATE_DEPTH_MASK] = true;
}

CMaterial::~CMaterial(void)
{
    
}

void CMaterial::Serialize(const std::shared_ptr<I_RO_TemplateCommon>& _template, const std::shared_ptr<CResourceAccessor>& _resourceAccessor, const std::shared_ptr<IScreenSpaceTextureAccessor>& _screenSpaceTextureAccessor, const std::shared_ptr<IResourceLoadingHandler>& _handler)
{
    assert(_template != nullptr);
    assert(_screenSpaceTextureAccessor != nullptr);
	assert(_resourceAccessor != nullptr);
    
    
    std::shared_ptr<CTemplateMaterial> materialTemplate = std::static_pointer_cast<CTemplateMaterial>(_template);
    CMaterial::Set_RenderState(E_RENDER_STATE_CULL_MODE, materialTemplate->Get_IsCullFace());
    CMaterial::Set_RenderState(E_RENDER_STATE_DEPTH_TEST, materialTemplate->Get_IsDepthTest());
    CMaterial::Set_RenderState(E_RENDER_STATE_DEPTH_MASK, materialTemplate->Get_IsDepthMask());
    CMaterial::Set_RenderState(E_RENDER_STATE_BLEND_MODE, materialTemplate->Get_IsBlending());
    
    CMaterial::Set_CullFaceMode(materialTemplate->Get_CullFaceMode());
    CMaterial::Set_BlendFunctionSource(materialTemplate->Get_BlendingFunctionSource());
    CMaterial::Set_BlendFunctionDest(materialTemplate->Get_BlendingFunctionDestination());
    
    CMaterial::Set_Clipping(materialTemplate->Get_IsClipping() ? materialTemplate->Get_ClippingPlane() : glm::vec4(FLT_MAX));
    CMaterial::Set_IsReflected(materialTemplate->Get_IsReflecting());
    CMaterial::Set_IsDebug(materialTemplate->Get_IsDebugging());
    
    for(const auto& iterator : materialTemplate->Get_TexturesTemplates())
    {
        std::shared_ptr<CTemplateTexture> textureTemplate = std::static_pointer_cast<CTemplateTexture>(iterator);
        
        std::shared_ptr<CTexture> texture = textureTemplate->Get_Filename().length() != 0 ? _resourceAccessor->CreateTexture(textureTemplate->Get_Filename()) : _screenSpaceTextureAccessor->Get_RenderOperationTexture(textureTemplate->Get_RenderOperationName());
        if(_handler != nullptr)
        {
            texture->Register_LoadingHandler(_handler);
        }
        assert(texture != nullptr);
        texture->Set_Wrap(textureTemplate->Get_WrapMode());
        assert(textureTemplate->Get_SamplerId() >= 0 && textureTemplate->Get_SamplerId() < E_SHADER_SAMPLER_MAX);
        CMaterial::Set_Texture(texture, static_cast<E_SHADER_SAMPLER>(textureTemplate->Get_SamplerId()));
    }
    
    m_status |= E_RESOURCE_STATUS_LOADED;
    m_status |= E_RESOURCE_STATUS_COMMITED;
}

bool CMaterial::IsLoaded(void) const
{
    bool value = false;
    for(const auto& texture : m_textures)
    {
        if(texture != nullptr)
        {
            value = texture->IsLoaded();
            if(!value)
            {
                return value;
            }
        }
    }
    value = m_shader->IsLoaded();
    return value;
}

bool CMaterial::IsCommited(void) const
{
    bool value = false;
    for(const auto& texture : m_textures)
    {
        if(texture != nullptr)
        {
            value = texture->IsCommited();
            if(!value)
            {
                return value;
            }
        }
    }
    value = m_shader->IsCommited();
    return value;
}

void CMaterial::Set_CullFaceMode(GLenum _mode)
{
    m_cullFaceMode = _mode;
}

void CMaterial::Set_BlendFunctionSource(GLenum _blendFunction)
{
    m_blendFunctionSource = _blendFunction;
}

void CMaterial::Set_BlendFunctionDest(GLenum _blendFunction)
{
    m_blendFunctionDest = _blendFunction;
}

void CMaterial::Set_Clipping(const glm::vec4& _clipping)
{
    m_clipping = _clipping;
}

glm::vec4 CMaterial::Get_Clipping(void)
{
    return m_clipping;
}

void CMaterial::Set_RenderState(E_RENDER_STATE _state, bool _value)
{
    m_states[_state] = _value;
}

void CMaterial::Set_Texture(const std::shared_ptr<CTexture>& _texture, E_SHADER_SAMPLER _sampler)
{
    m_textures.at(_sampler) = _texture;
}

std::shared_ptr<CTexture> CMaterial::Get_Texture(E_SHADER_SAMPLER _sampler)
{
    return m_textures.at(_sampler);
}


E_SHADER_SAMPLER CMaterial::Get_Sampler(const std::shared_ptr<CTexture> &_texture)
{
    for(ui32 i = 0; i < m_textures.size(); ++i)
    {
        if(_texture == m_textures.at(i))
        {
            return static_cast<E_SHADER_SAMPLER>(i);
        }
    }
    assert(false);
    return E_SHADER_SAMPLER_01;
}

void CMaterial::Bind(void)
{
    assert(m_shader != nullptr);
    
    m_shader->Bind();
    
    for(ui32 i = 0; i < E_SHADER_SAMPLER_MAX; ++i)
    {
        if(m_textures[i] != nullptr)
        {
            m_shader->Set_Texture(m_textures[i], static_cast<E_SHADER_SAMPLER>(i));
        }
    }
    
    if(m_states[E_RENDER_STATE_DEPTH_TEST] && s_states[E_RENDER_STATE_DEPTH_TEST] != m_states[E_RENDER_STATE_DEPTH_TEST])
    {
        glEnable(GL_DEPTH_TEST);
        s_states[E_RENDER_STATE_DEPTH_TEST] = m_states[E_RENDER_STATE_DEPTH_TEST];
    }
    else if(s_states[E_RENDER_STATE_DEPTH_TEST] != m_states[E_RENDER_STATE_DEPTH_TEST])
    {
        glDisable(GL_DEPTH_TEST);
        s_states[E_RENDER_STATE_DEPTH_TEST] = m_states[E_RENDER_STATE_DEPTH_TEST];
    }
    
    if(m_states[E_RENDER_STATE_DEPTH_MASK] && s_states[E_RENDER_STATE_DEPTH_MASK] != m_states[E_RENDER_STATE_DEPTH_MASK])
    {
        glDepthMask(GL_TRUE);
        s_states[E_RENDER_STATE_DEPTH_MASK] = m_states[E_RENDER_STATE_DEPTH_MASK];
    }
    else if(s_states[E_RENDER_STATE_DEPTH_MASK] != m_states[E_RENDER_STATE_DEPTH_MASK])
    {
        glDepthMask(GL_FALSE);
        s_states[E_RENDER_STATE_DEPTH_MASK] = m_states[E_RENDER_STATE_DEPTH_MASK];
    }
    
    if(m_states[E_RENDER_STATE_CULL_MODE] && s_states[E_RENDER_STATE_CULL_MODE] != m_states[E_RENDER_STATE_CULL_MODE])
    {
        glEnable(GL_CULL_FACE);
        glCullFace(m_cullFaceMode);
        s_states[E_RENDER_STATE_CULL_MODE] = m_states[E_RENDER_STATE_CULL_MODE];
    }
    else if(s_states[E_RENDER_STATE_CULL_MODE] != m_states[E_RENDER_STATE_CULL_MODE])
    {
        glDisable(GL_CULL_FACE);
        s_states[E_RENDER_STATE_CULL_MODE] = m_states[E_RENDER_STATE_CULL_MODE];
    }
    
    if(m_states[E_RENDER_STATE_BLEND_MODE] && s_states[E_RENDER_STATE_BLEND_MODE] != m_states[E_RENDER_STATE_BLEND_MODE])
    {
        glEnable(GL_BLEND);
        glBlendFunc(m_blendFunctionSource, m_blendFunctionDest);
        s_states[E_RENDER_STATE_BLEND_MODE] = m_states[E_RENDER_STATE_BLEND_MODE];
    }
    else if(s_states[E_RENDER_STATE_BLEND_MODE] != m_states[E_RENDER_STATE_BLEND_MODE])
    {
        glDisable(GL_BLEND);
        s_states[E_RENDER_STATE_BLEND_MODE] = m_states[E_RENDER_STATE_BLEND_MODE];
    }
}

void CMaterial::Unbind(void)
{
    m_shader->Unbind();
}
