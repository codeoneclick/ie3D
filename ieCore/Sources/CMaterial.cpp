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

I_RO_Material::I_RO_Material(void)
{
    
}

I_RO_Material::~I_RO_Material(void)
{
    
}

I_WO_Material::I_WO_Material(void)
{
    
}

I_WO_Material::~I_WO_Material(void)
{
    
}

CMaterialCachedParameters::CMaterialCachedParameters(void) :
m_shader(nullptr)
{
    std::for_each(m_textures.begin(), m_textures.end(), [](std::shared_ptr<CTexture> _iterator){
        _iterator = nullptr;
    });
}

CMaterialCachedParameters::~CMaterialCachedParameters(void)
{
    
}

std::shared_ptr<CMaterialCachedParameters> CMaterial::m_cachedParameters = nullptr;

std::shared_ptr<CMaterialCachedParameters> CMaterial::Get_CachedParameters(void)
{
    if(m_cachedParameters == nullptr)
    {
        m_cachedParameters = std::make_shared<CMaterialCachedParameters>();
        m_cachedParameters->m_isDepthTest = true;
        m_cachedParameters->m_isDepthMask = true;
        m_cachedParameters->m_isCulling = false;
        m_cachedParameters->m_isBlending = false;
    }
    return m_cachedParameters;
}


CMaterial::CMaterial(const std::string& _guid, bool _isCustom) :
IResource(E_RESOURCE_CLASS_MATERIAL, _guid)
{
    if(_isCustom)
    {
        m_parameters = std::make_shared<CMaterialCachedParameters>();
    }
}

CMaterial::~CMaterial(void)
{
    
}

bool CMaterial::Get_IsCulling(void) const
{
    assert(m_parameters != nullptr);
    return m_parameters->m_isCulling;
}

GLenum CMaterial::Get_CullingMode(void) const
{
    assert(m_parameters != nullptr);
    return m_parameters->m_cullingMode;
}

bool CMaterial::Get_IsBlending(void) const
{
    assert(m_parameters != nullptr);
    return m_parameters->m_isBlending;
}

GLenum CMaterial::Get_BlendingFunctionSource(void) const
{
    assert(m_parameters != nullptr);
    return m_parameters->m_blendingFunctionSource;
}

GLenum CMaterial::Get_BlendingFunctionDestination(void) const
{
    assert(m_parameters != nullptr);
    return m_parameters->m_blendingFunctionDestination;
}

bool CMaterial::Get_IsDepthTest(void) const
{
    assert(m_parameters != nullptr);
    return m_parameters->m_isDepthTest;
}

bool CMaterial::Get_IsDepthMask(void) const
{
    assert(m_parameters != nullptr);
    return m_parameters->m_isDepthMask;
}

bool CMaterial::Get_IsClipping(void) const
{
    assert(m_parameters != nullptr);
    return m_parameters->m_isClipping;
}

glm::vec4 CMaterial::Get_ClippingPlane(void) const
{
    assert(m_parameters != nullptr);
    return m_parameters->m_clippingPlane;
}

bool CMaterial::Get_IsReflecting(void) const
{
    assert(m_parameters != nullptr);
    return m_parameters->m_isReflecting;
}

bool CMaterial::Get_IsShadowing(void) const
{
    assert(m_parameters != nullptr);
    return m_parameters->m_isShadowing;
}

bool CMaterial::Get_IsDebugging(void) const
{
    assert(m_parameters != nullptr);
    return m_parameters->m_isDebugging;
}

std::shared_ptr<CShader> CMaterial::Get_Shader(void) const
{
    assert(m_parameters != nullptr);
    return m_parameters->m_shader;
}

std::shared_ptr<CTexture> CMaterial::Get_Texture(E_SHADER_SAMPLER _sampler) const
{
    assert(m_parameters != nullptr);
    return m_parameters->m_textures.at(_sampler);
}

E_SHADER_SAMPLER CMaterial::Get_SamplerId(const std::shared_ptr<CTexture>& _texture) const
{
    assert(m_parameters != nullptr);
    for(ui32 i = 0; i < m_parameters->m_textures.size(); ++i)
    {
        if(_texture == m_parameters->m_textures.at(i))
        {
            return static_cast<E_SHADER_SAMPLER>(i);
        }
    }
    assert(false);
    return E_SHADER_SAMPLER_01;
}

void CMaterial::Set_IsCulling(bool _value)
{
    assert(m_parameters != nullptr);
    m_parameters->m_isCulling = _value;
}

void CMaterial::Set_CullingMode(GLenum _value)
{
    assert(m_parameters != nullptr);
    m_parameters->m_cullingMode = _value;
}

void CMaterial::Set_IsBlending(bool _value)
{
    assert(m_parameters != nullptr);
    m_parameters->m_isBlending = _value;
}

void CMaterial::Set_BlendingFunctionSource(GLenum _value)
{
    assert(m_parameters != nullptr);
    m_parameters->m_blendingFunctionSource = _value;
}

void CMaterial::Set_BlendingFunctionDestination(GLenum _value)
{
    assert(m_parameters != nullptr);
    m_parameters->m_blendingFunctionDestination = _value;
}

void CMaterial::Set_IsDepthTest(bool _value)
{
    assert(m_parameters != nullptr);
    m_parameters->m_isDepthTest = _value;
}

void CMaterial::Set_IsDepthMask(bool _value)
{
    assert(m_parameters != nullptr);
    m_parameters->m_isDepthMask =_value;
}

void CMaterial::Set_IsClipping(bool _value)
{
    assert(m_parameters != nullptr);
    m_parameters->m_isClipping = _value;
}

void CMaterial::Set_ClippingPlane(const glm::vec4& _value)
{
    assert(m_parameters != nullptr);
    m_parameters->m_clippingPlane = _value;
}

void CMaterial::Set_IsReflecting(bool _value)
{
    assert(m_parameters != nullptr);
    m_parameters->m_isReflecting = _value;
}

void CMaterial::Set_IsShadowing(bool _value)
{
    assert(m_parameters != nullptr);
    m_parameters->m_isShadowing = _value;
}

void CMaterial::Set_IsDebugging(bool _value)
{
    assert(m_parameters != nullptr);
    m_parameters->m_isDebugging = _value;
}

void CMaterial::Set_Shader(const std::shared_ptr<CShader>& _shader)
{
    assert(m_parameters != nullptr);
    m_parameters->m_shader = _shader;
}

void CMaterial::Set_Texture(const std::shared_ptr<CTexture>& _texture,
                 E_SHADER_SAMPLER _sampler)
{
    assert(m_parameters != nullptr);
    m_parameters->m_textures.at(_sampler) = _texture;
}

void CMaterial::Serialize(const std::shared_ptr<I_RO_TemplateCommon>& _template,
               const std::shared_ptr<CResourceAccessor>& _resourceAccessor,
               const std::shared_ptr<IScreenSpaceTextureAccessor>& _screenSpaceTextureAccessor,
               const std::shared_ptr<IResourceLoadingHandler>& _handler)
{
    assert(_template != nullptr);
    assert(_screenSpaceTextureAccessor != nullptr);
	assert(_resourceAccessor != nullptr);
    
    std::shared_ptr<CMaterialTemplate> materialTemplate = std::static_pointer_cast<CMaterialTemplate>(_template);
    assert(materialTemplate != nullptr);
    
    m_parameters = std::make_shared<CMaterialCachedParameters>();
    
    m_parameters->m_isCulling = materialTemplate->Get_IsCulling();
    m_parameters->m_cullingMode = materialTemplate->Get_CullingMode();
    
    m_parameters->m_isBlending = materialTemplate->Get_IsBlending();
    m_parameters->m_blendingFunctionSource = materialTemplate->Get_BlendingFunctionSource();
    m_parameters->m_blendingFunctionDestination = materialTemplate->Get_BlendingFunctionDestination();
    
    m_parameters->m_isDepthTest = materialTemplate->Get_IsDepthTest();
    m_parameters->m_isDepthMask = materialTemplate->Get_IsDepthMask();
    
    m_parameters->m_isClipping = materialTemplate->Get_IsClipping();
    m_parameters->m_clippingPlane = materialTemplate->Get_ClippingPlane();
    
    m_parameters->m_isReflecting = materialTemplate->Get_IsReflecting();
    m_parameters->m_isShadowing = materialTemplate->Get_IsShadowing();
    m_parameters->m_isDebugging = materialTemplate->Get_IsDebugging();
    
    for(const auto& iterator : materialTemplate->Get_TexturesTemplates())
    {
        std::shared_ptr<CTextureTemplate> textureTemplate = std::static_pointer_cast<CTextureTemplate>(iterator);
        assert(textureTemplate != nullptr);
        
        std::shared_ptr<CTexture> texture = textureTemplate->Get_Filename().length() != 0 ?
        _resourceAccessor->CreateTexture(textureTemplate->Get_Filename()) :
        _screenSpaceTextureAccessor->Get_RenderOperationTexture(textureTemplate->Get_RenderOperationName());
        if(_handler != nullptr)
        {
            texture->Register_LoadingHandler(_handler);
        }
        assert(texture != nullptr);
        texture->Set_WrapMode(textureTemplate->Get_WrapMode());
        assert(textureTemplate->Get_SamplerId() >= 0 && textureTemplate->Get_SamplerId() < E_SHADER_SAMPLER_MAX);
        CMaterial::Set_Texture(texture, static_cast<E_SHADER_SAMPLER>(textureTemplate->Get_SamplerId()));
    }
    
    std::shared_ptr<CShaderTemplate> shaderTemplate = std::static_pointer_cast<CShaderTemplate>(materialTemplate->Get_ShaderTemplate());
    assert(shaderTemplate != nullptr);
    std::shared_ptr<CShader> shader = _resourceAccessor->CreateShader(shaderTemplate->Get_VSFilename(),
                                                                      shaderTemplate->Get_FSFilename());
    assert(shader != nullptr);
    CMaterial::Set_Shader(shader);
    if(_handler != nullptr)
    {
        shader->Register_LoadingHandler(_handler);
    }
    
    m_status |= E_RESOURCE_STATUS_LOADED;
    m_status |= E_RESOURCE_STATUS_COMMITED;
}


bool CMaterial::IsLoaded(void) const
{
    bool value = false;
    if(m_parameters != nullptr)
    {
        for(const auto& texture : m_parameters->m_textures)
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
        value = m_parameters->m_shader->IsLoaded();
    }
    return value;
}

bool CMaterial::IsCommited(void) const
{
    bool value = false;
    if(m_parameters != nullptr)
    {
        for(const auto& texture : m_parameters->m_textures)
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
        value = m_parameters->m_shader->IsCommited();
    }
    return value;
}

void CMaterial::Bind(void)
{
    assert(m_parameters != nullptr);
    assert(m_parameters->m_shader != nullptr);
    
    m_parameters->m_shader->Bind();
    for(ui32 i = 0; i < E_SHADER_SAMPLER_MAX; ++i)
    {
        if(m_parameters->m_textures[i] != nullptr)
        {
            m_parameters->m_shader->Set_Texture(m_parameters->m_textures[i], static_cast<E_SHADER_SAMPLER>(i));
        }
    }
    
    if(m_parameters->m_isDepthTest &&
       Get_CachedParameters()->m_isDepthTest != m_parameters->m_isDepthTest)
    {
        glEnable(GL_DEPTH_TEST);
        Get_CachedParameters()->m_isDepthTest = m_parameters->m_isDepthTest;
    }
    else if(Get_CachedParameters()->m_isDepthTest != m_parameters->m_isDepthTest)
    {
        glDisable(GL_DEPTH_TEST);
        Get_CachedParameters()->m_isDepthTest = m_parameters->m_isDepthTest;
    }
    
    
    if(m_parameters->m_isDepthMask &&
       Get_CachedParameters()->m_isDepthMask != m_parameters->m_isDepthMask)
    {
        glDepthMask(GL_TRUE);
        Get_CachedParameters()->m_isDepthMask = m_parameters->m_isDepthMask;
    }
    else if(Get_CachedParameters()->m_isDepthMask != m_parameters->m_isDepthMask)
    {
        glDepthMask(GL_FALSE);
        Get_CachedParameters()->m_isDepthMask = m_parameters->m_isDepthMask;
    }
    
    if(m_parameters->m_isCulling &&
       Get_CachedParameters()->m_isCulling != m_parameters->m_isCulling)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(m_parameters->m_cullingMode);
        Get_CachedParameters()->m_isCulling = m_parameters->m_isCulling;
    }
    else if(Get_CachedParameters()->m_isCulling != m_parameters->m_isCulling)
    {
        glDisable(GL_CULL_FACE);
        Get_CachedParameters()->m_isCulling = m_parameters->m_isCulling;
    }
    
    if(m_parameters->m_isBlending &&
       Get_CachedParameters()->m_isBlending != m_parameters->m_isBlending)
    {
        glEnable(GL_BLEND);
        glBlendFunc(m_parameters->m_blendingFunctionSource, m_parameters->m_blendingFunctionDestination);
        Get_CachedParameters()->m_isBlending = m_parameters->m_isBlending;
    }
    else if(Get_CachedParameters()->m_isBlending != m_parameters->m_isBlending)
    {
        glDisable(GL_BLEND);
        Get_CachedParameters()->m_isBlending = m_parameters->m_isBlending;
    }
    
}

void CMaterial::Unbind(void)
{
    assert(m_parameters != nullptr);
    assert(m_parameters->m_shader != nullptr);
    m_parameters->m_shader->Unbind();
}
