//
//  CMaterial.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CMaterial.h"
#include "CShader.h"

CMaterial::CMaterial(std::shared_ptr<CShader> _shader) :
m_shader(_shader)
{
    for(ui32 i = 0; i < E_SHADER_SAMPLER_MAX; ++i)
    {
        m_textures[i] = nullptr;
    }
    
    m_states[E_RENDER_STATE_DEPTH_TEST] = true;
    m_states[E_RENDER_STATE_DEPTH_MASK] = true;
}

CMaterial::~CMaterial(void)
{
    
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

void CMaterial::Set_Texture(std::shared_ptr<CTexture> _texture, E_SHADER_SAMPLER _sampler)
{
    m_textures[_sampler] = _texture;
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
    
    if(m_states[E_RENDER_STATE_DEPTH_TEST])
    {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
    
    
    if(m_states[E_RENDER_STATE_DEPTH_MASK])
    {
        glDepthMask(GL_TRUE);
    }
    else
    {
        glDepthMask(GL_FALSE);
    }
    
    if(m_states[E_RENDER_STATE_CULL_MODE])
    {
        glEnable(GL_CULL_FACE);
        glCullFace(m_cullFaceMode);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }
    
    if(m_states[E_RENDER_STATE_BLEND_MODE])
    {
        glEnable(GL_BLEND);
        glBlendFunc(m_blendFunctionSource, m_blendFunctionDest);
    }
    else
    {
        glDisable(GL_BLEND);
    }
}

void CMaterial::Unbind(void)
{
    m_shader->Unbind();
}