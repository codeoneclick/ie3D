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
#include "CConfigurationGameObjects.h"
#include "CResourceAccessor.h"

CMaterialCachedParameters::CMaterialCachedParameters(void) :
m_shader(nullptr)
{
    std::for_each(m_textures.begin(), m_textures.end(), [](CSharedTexture& iterator){
        iterator = nullptr;
    });
}

CMaterialCachedParameters::~CMaterialCachedParameters(void)
{
    
}

CSharedMaterialCachedParameters CMaterial::m_cachedParameters = nullptr;

CSharedMaterialCachedParameters CMaterial::getCachedParameters(void)
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


CMaterial::CMaterial(void) :
m_parameters(std::make_shared<CMaterialCachedParameters>())
{

}

CMaterial::~CMaterial(void)
{
    
}

bool CMaterial::isCulling(void) const
{
    assert(m_parameters != nullptr);
    return m_parameters->m_isCulling;
}

GLenum CMaterial::getCullingMode(void) const
{
    assert(m_parameters != nullptr);
    return m_parameters->m_cullingMode;
}

bool CMaterial::isBlending(void) const
{
    assert(m_parameters != nullptr);
    return m_parameters->m_isBlending;
}

GLenum CMaterial::getBlendingFunctionSource(void) const
{
    assert(m_parameters != nullptr);
    return m_parameters->m_blendingFunctionSource;
}

GLenum CMaterial::getBlendingFunctionDestination(void) const
{
    assert(m_parameters != nullptr);
    return m_parameters->m_blendingFunctionDestination;
}

bool CMaterial::isDepthTest(void) const
{
    assert(m_parameters != nullptr);
    return m_parameters->m_isDepthTest;
}

bool CMaterial::isDepthMask(void) const
{
    assert(m_parameters != nullptr);
    return m_parameters->m_isDepthMask;
}

bool CMaterial::isClipping(void) const
{
    assert(m_parameters != nullptr);
    return m_parameters->m_isClipping;
}

glm::vec4 CMaterial::getClippingPlane(void) const
{
    assert(m_parameters != nullptr);
    return m_parameters->m_clippingPlane;
}

bool CMaterial::isReflecting(void) const
{
    assert(m_parameters != nullptr);
    return m_parameters->m_isReflecting;
}

bool CMaterial::isShadowing(void) const
{
    assert(m_parameters != nullptr);
    return m_parameters->m_isShadowing;
}

bool CMaterial::isDebugging(void) const
{
    assert(m_parameters != nullptr);
    return m_parameters->m_isDebugging;
}

CSharedShader CMaterial::getShader(void) const
{
    assert(m_parameters != nullptr);
    return m_parameters->m_shader;
}

CSharedTexture CMaterial::getTexture(E_SHADER_SAMPLER sampler) const
{
    assert(m_parameters != nullptr);
    return m_parameters->m_textures.at(sampler);
}

E_SHADER_SAMPLER CMaterial::getSamplerIndex(CSharedTextureRef texture) const
{
    assert(m_parameters != nullptr);
    for(ui32 i = 0; i < m_parameters->m_textures.size(); ++i)
    {
        if(texture == m_parameters->m_textures.at(i))
        {
            return static_cast<E_SHADER_SAMPLER>(i);
        }
    }
    assert(false);
    return E_SHADER_SAMPLER_01;
}

void CMaterial::setCulling(bool value)
{
    assert(m_parameters != nullptr);
    m_parameters->m_isCulling = value;
}

void CMaterial::setCullingMode(GLenum value)
{
    assert(m_parameters != nullptr);
    m_parameters->m_cullingMode = value;
}

void CMaterial::setBlending(bool value)
{
    assert(m_parameters != nullptr);
    m_parameters->m_isBlending = value;
}

void CMaterial::setBlendingFunctionSource(GLenum value)
{
    assert(m_parameters != nullptr);
    m_parameters->m_blendingFunctionSource = value;
}

void CMaterial::setBlendingFunctionDestination(GLenum value)
{
    assert(m_parameters != nullptr);
    m_parameters->m_blendingFunctionDestination = value;
}

void CMaterial::setDepthTest(bool value)
{
    assert(m_parameters != nullptr);
    m_parameters->m_isDepthTest = value;
}

void CMaterial::setDepthMask(bool value)
{
    assert(m_parameters != nullptr);
    m_parameters->m_isDepthMask = value;
}

void CMaterial::setClipping(bool value)
{
    assert(m_parameters != nullptr);
    m_parameters->m_isClipping = value;
}

void CMaterial::setClippingPlane(const glm::vec4& value)
{
    assert(m_parameters != nullptr);
    m_parameters->m_clippingPlane = value;
}

void CMaterial::setReflecting(bool value)
{
    assert(m_parameters != nullptr);
    m_parameters->m_isReflecting = value;
}

void CMaterial::setShadowing(bool value)
{
    assert(m_parameters != nullptr);
    m_parameters->m_isShadowing = value;
}

void CMaterial::setDebugging(bool value)
{
    assert(m_parameters != nullptr);
    m_parameters->m_isDebugging = value;
}

void CMaterial::setShader(CSharedShaderRef shader)
{
    assert(m_parameters != nullptr);
    m_parameters->m_shader = shader;
}

void CMaterial::setTexture(CSharedTextureRef texture,
                 E_SHADER_SAMPLER sampler)
{
    assert(m_parameters != nullptr);
    m_parameters->m_textures.at(sampler) = texture;
}

void CMaterial::setupMaterial(CSharedMaterialRef material,
                              CSharedConfigurationMaterialRef configuration,
                              CSharedResourceAccessor resourceAccessor,
                              ISharedScreenSpaceTextureAccessor screenSpaceTextureAccessor,
                              ISharedResourceLoadingHandlerRef handler)
{
    assert(configuration != nullptr);
    assert(screenSpaceTextureAccessor != nullptr);
	assert(resourceAccessor != nullptr);
    
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
        resourceAccessor->getTexture(textureConfiguration->getFilename()) :
        screenSpaceTextureAccessor->Get_RenderOperationTexture(textureConfiguration->getRenderOperationName());
        assert(texture != nullptr);
        texture->setWrapMode(textureConfiguration->getWrapMode());
        assert(textureConfiguration->getSamplerIndex() >= 0 &&
               textureConfiguration->getSamplerIndex() < E_SHADER_SAMPLER_MAX);
        material->setTexture(texture, static_cast<E_SHADER_SAMPLER>(textureConfiguration->getSamplerIndex()));
        
        if(handler != nullptr)
        {
            texture->registerLoadingHandler(handler);
        }
    }
    
    CSharedConfigurationShader shaderConfiguration = std::static_pointer_cast<CConfigurationShader>(configuration->getShaderConfiguration());
    assert(shaderConfiguration != nullptr);
    CSharedShader shader = resourceAccessor->getShader(shaderConfiguration->getVSFilename(),
                                                       shaderConfiguration->getFSFilename());
    assert(shader != nullptr);
    material->setShader(shader);
    if(handler != nullptr)
    {
        shader->registerLoadingHandler(handler);
    }
}

bool CMaterial::isLoaded(void) const
{
    bool value = false;
    if(m_parameters != nullptr)
    {
        for(const auto& texture : m_parameters->m_textures)
        {
            if(texture != nullptr)
            {
                value = texture->isLoaded();
                if(!value)
                {
                    return value;
                }
            }
        }
        value = m_parameters->m_shader->isLoaded();
    }
    return value;
}

bool CMaterial::isCommited(void) const
{
    bool value = false;
    if(m_parameters != nullptr)
    {
        for(const auto& texture : m_parameters->m_textures)
        {
            if(texture != nullptr)
            {
                value = texture->isCommited();
                if(!value)
                {
                    return value;
                }
            }
        }
        value = m_parameters->m_shader->isCommited();
    }
    return value;
}

void CMaterial::bind(void)
{
    assert(m_parameters != nullptr);
    assert(m_parameters->m_shader != nullptr);
    
    m_parameters->m_shader->bind();
    for(ui32 i = 0; i < E_SHADER_SAMPLER_MAX; ++i)
    {
        if(m_parameters->m_textures[i] != nullptr)
        {
            m_parameters->m_shader->setTexture(m_parameters->m_textures[i], static_cast<E_SHADER_SAMPLER>(i));
        }
    }
    
    if(m_parameters->m_isDepthTest &&
       getCachedParameters()->m_isDepthTest != m_parameters->m_isDepthTest)
    {
        glEnable(GL_DEPTH_TEST);
        getCachedParameters()->m_isDepthTest = m_parameters->m_isDepthTest;
    }
    else if(getCachedParameters()->m_isDepthTest != m_parameters->m_isDepthTest)
    {
        glDisable(GL_DEPTH_TEST);
        getCachedParameters()->m_isDepthTest = m_parameters->m_isDepthTest;
    }
    
    if(m_parameters->m_isDepthMask &&
       getCachedParameters()->m_isDepthMask != m_parameters->m_isDepthMask)
    {
        glDepthMask(GL_TRUE);
        getCachedParameters()->m_isDepthMask = m_parameters->m_isDepthMask;
    }
    else if(getCachedParameters()->m_isDepthMask != m_parameters->m_isDepthMask)
    {
        glDepthMask(GL_FALSE);
        getCachedParameters()->m_isDepthMask = m_parameters->m_isDepthMask;
    }
    
    if(m_parameters->m_isCulling &&
       getCachedParameters()->m_isCulling != m_parameters->m_isCulling)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(m_parameters->m_cullingMode);
        getCachedParameters()->m_isCulling = m_parameters->m_isCulling;
    }
    else if(getCachedParameters()->m_isCulling != m_parameters->m_isCulling)
    {
        glDisable(GL_CULL_FACE);
        getCachedParameters()->m_isCulling = m_parameters->m_isCulling;
    }
    
    if(m_parameters->m_isBlending &&
       getCachedParameters()->m_isBlending != m_parameters->m_isBlending)
    {
        glEnable(GL_BLEND);
        glBlendFunc(m_parameters->m_blendingFunctionSource, m_parameters->m_blendingFunctionDestination);
        getCachedParameters()->m_isBlending = m_parameters->m_isBlending;
    }
    else if(getCachedParameters()->m_isBlending != m_parameters->m_isBlending)
    {
        glDisable(GL_BLEND);
        getCachedParameters()->m_isBlending = m_parameters->m_isBlending;
    }
}

void CMaterial::unbind(void)
{
    assert(m_parameters != nullptr);
    assert(m_parameters->m_shader != nullptr);
    m_parameters->m_shader->unbind();
}