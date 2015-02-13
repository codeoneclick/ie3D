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
#include "CCubemapTexture.h"
#include "IRenderTechniqueImporter.h"
#include "IRenderTechniqueAccessor.h"
#include "CConfigurationAccessor.h"
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
#if defined(__EDITOR__)

m_isEnabled(true),

#endif

m_parameters(std::make_shared<CMaterialCachedParameters>())
{

}

CMaterial::~CMaterial(void)
{
    
}

CSharedMaterial CMaterial::constructCustomMaterial(CSharedConfigurationMaterialRef configuration,
                                                   CSharedResourceAccessor resourceAccessor,
                                                   ISharedRenderTechniqueAccessor renderTechniqueAccessor,
                                                   ISharedResourceLoadingHandlerRef handler)
{
    CSharedMaterial material = std::make_shared<CMaterial>();
    assert(configuration != nullptr);
    assert(renderTechniqueAccessor != nullptr);
    assert(resourceAccessor != nullptr);
    
    material->setCulling(configuration->getCulling());
    material->setCullingMode(configuration->getCullingMode());
    
    material->setBlending(configuration->getBlending());
    material->setBlendingFunctionSource(configuration->getBlendingFunctionSource());
    material->setBlendingFunctionDestination(configuration->getBlendingFunctionDestination());
    
    material->setDepthTest(configuration->getDepthTest());
    material->setDepthMask(configuration->getDepthMask());
    
    material->setClipping(configuration->getClipping());
    material->setClippingPlane(glm::vec4(configuration->getClippingX(),
                                         configuration->getClippingY(),
                                         configuration->getClippingZ(),
                                         configuration->getClippingW()));
    
    material->setReflecting(configuration->getReflecting());
    material->setShadowing(configuration->getShadowing());
    material->setDebugging(configuration->getDebugging());
    
    for(const auto& iterator : configuration->getTexturesConfigurations())
    {
        CSharedConfigurationTexture textureConfiguration = std::static_pointer_cast<CConfigurationTexture>(iterator);
        assert(textureConfiguration != nullptr);
        CSharedTexture texture;
        if(textureConfiguration->getCubemap())
        {
            CSharedTexture xpositiveTexture = resourceAccessor->getTexture(textureConfiguration->getTextureFilenamePositiveX());
            CSharedTexture xnegativeTexture = resourceAccessor->getTexture(textureConfiguration->getTextureFilenameNegativeX());
            CSharedTexture ypositiveTexture = resourceAccessor->getTexture(textureConfiguration->getTextureFilenamePositiveY());
            CSharedTexture ynegativeTexture = resourceAccessor->getTexture(textureConfiguration->getTextureFilenameNegativeY());
            CSharedTexture zpositiveTexture = resourceAccessor->getTexture(textureConfiguration->getTextureFilenamePositiveZ());
            CSharedTexture znegativeTexture = resourceAccessor->getTexture(textureConfiguration->getTextureFilenameNegativeZ());
            
            texture = CCubemapTexture::constructCustomCubemapTexture("skybox.cubemap.texture",
                                                                     xpositiveTexture,
                                                                     xnegativeTexture,
                                                                     ypositiveTexture,
                                                                     ynegativeTexture,
                                                                     zpositiveTexture,
                                                                     znegativeTexture);
        }
        else
        {
            texture = textureConfiguration->getTextureFilename().length() != 0 ?
            resourceAccessor->getTexture(textureConfiguration->getTextureFilename()) :
            renderTechniqueAccessor->getTechniqueTexture(textureConfiguration->getRenderTechniqueTextureName());
        }
        assert(texture != nullptr);
        texture->setWrapMode(textureConfiguration->getWrapMode());
        texture->setMagFilter(textureConfiguration->getMagFilter());
        texture->setMinFilter(textureConfiguration->getMinFilter());
        assert(textureConfiguration->getSamplerIndex() >= 0 &&
               textureConfiguration->getSamplerIndex() < E_SHADER_SAMPLER_MAX);
        material->setTexture(texture, static_cast<E_SHADER_SAMPLER>(textureConfiguration->getSamplerIndex()));
        
        if(handler != nullptr)
        {
            texture->addLoadingHandler(handler);
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
        shader->addLoadingHandler(handler);
    }
    return material;
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

void CMaterial::setCustomShaderUniform(const glm::mat4x4& matrix, const std::string& uniform)
{
    const auto& iterator = m_customShaderUniforms.find(uniform);
    CSharedShaderUniform currentUniform = nullptr;
    if(iterator != m_customShaderUniforms.end())
    {
        currentUniform = iterator->second;
    }
    else
    {
        currentUniform = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_MAT4X4);
        m_customShaderUniforms.insert(std::make_pair(uniform, currentUniform));
    }
    currentUniform->setMatrix4x4(matrix);
}

void CMaterial::setCustomShaderUniform(const glm::mat3x3& matrix, const std::string& uniform)
{
    const auto& iterator = m_customShaderUniforms.find(uniform);
    CSharedShaderUniform currentUniform = nullptr;
    if(iterator != m_customShaderUniforms.end())
    {
        currentUniform = iterator->second;
    }
    else
    {
        currentUniform = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_MAT3X3);
        m_customShaderUniforms.insert(std::make_pair(uniform, currentUniform));
    }
    currentUniform->setMatrix3x3(matrix);
}

void CMaterial::setCustomShaderUniform(const glm::vec4& vector, const std::string& uniform)
{
    const auto& iterator = m_customShaderUniforms.find(uniform);
    CSharedShaderUniform currentUniform = nullptr;
    if(iterator != m_customShaderUniforms.end())
    {
        currentUniform = iterator->second;
    }
    else
    {
        currentUniform = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_VECTOR4);
        m_customShaderUniforms.insert(std::make_pair(uniform, currentUniform));
    }
    currentUniform->setVector4(vector);
}

void CMaterial::setCustomShaderUniform(const glm::vec3& vector, const std::string& uniform)
{
    const auto& iterator = m_customShaderUniforms.find(uniform);
    CSharedShaderUniform currentUniform = nullptr;
    if(iterator != m_customShaderUniforms.end())
    {
        currentUniform = iterator->second;
    }
    else
    {
        currentUniform = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_VECTOR3);
        m_customShaderUniforms.insert(std::make_pair(uniform, currentUniform));
    }
    currentUniform->setVector3(vector);
}

void CMaterial::setCustomShaderUniform(const glm::vec2& vector, const std::string& uniform)
{
    const auto& iterator = m_customShaderUniforms.find(uniform);
    CSharedShaderUniform currentUniform = nullptr;
    if(iterator != m_customShaderUniforms.end())
    {
        currentUniform = iterator->second;
    }
    else
    {
        currentUniform = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_VECTOR2);
        m_customShaderUniforms.insert(std::make_pair(uniform, currentUniform));
    }
    currentUniform->setVector2(vector);
}

void CMaterial::setCustomShaderUniform(f32 value, const std::string& uniform)
{
    const auto& iterator = m_customShaderUniforms.find(uniform);
    CSharedShaderUniform currentUniform = nullptr;
    if(iterator != m_customShaderUniforms.end())
    {
        currentUniform = iterator->second;
    }
    else
    {
        currentUniform = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_FLOAT);
        m_customShaderUniforms.insert(std::make_pair(uniform, currentUniform));
    }
    currentUniform->setFloat(value);
}

void CMaterial::setCustomShaderUniform(i32 value, const std::string& uniform)
{
    const auto& iterator = m_customShaderUniforms.find(uniform);
    CSharedShaderUniform currentUniform = nullptr;
    if(iterator != m_customShaderUniforms.end())
    {
        currentUniform = iterator->second;
    }
    else
    {
        currentUniform = std::make_shared<CShaderUniform>(E_UNIFORM_CLASS_INT);
        m_customShaderUniforms.insert(std::make_pair(uniform, currentUniform));
    }
    currentUniform->setInt(value);
}

const std::map<std::string, CSharedShaderUniform>& CMaterial::getCustomUniforms(void) const
{
    return m_customShaderUniforms;
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
        if(m_parameters->m_textures[i] != nullptr &&
           getCachedParameters()->m_textures.at(i) != m_parameters->m_textures[i])
        {
            m_parameters->m_shader->setTexture(m_parameters->m_textures[i], static_cast<E_SHADER_SAMPLER>(i));
            getCachedParameters()->m_textures.at(i) = m_parameters->m_textures[i];
        }
    }
    
    if(m_parameters->m_isDepthTest &&
       getCachedParameters()->m_isDepthTest != m_parameters->m_isDepthTest)
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
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

#if defined(__EDITOR__)

void CMaterial::setEnabled(bool value)
{
    m_isEnabled = value;
}

bool CMaterial::getEnabled(void) const
{
    return m_isEnabled;
}

#endif
