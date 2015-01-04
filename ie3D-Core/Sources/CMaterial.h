//
//  CMaterial.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CMaterial_h
#define CMaterial_h

#include "HCommon.h"
#include "HEnums.h"
#include "HDeclaration.h"

class CMaterialCachedParameters
{
private:
    
    friend class CMaterial;
    
protected:
    
    bool m_isCulling;
    GLenum m_cullingMode;
    
    bool m_isBlending;
    GLenum m_blendingFunctionSource;
    GLenum m_blendingFunctionDestination;
    
    bool m_isDepthTest;
    bool m_isDepthMask;
    
    bool m_isClipping;
    glm::vec4 m_clippingPlane;
    
    bool m_isReflecting;
    bool m_isShadowing;
    bool m_isDebugging;
    
    CSharedShader m_shader;
    std::array<CSharedTexture, E_SHADER_SAMPLER_MAX> m_textures;
    
public:
    
    CMaterialCachedParameters(void);
    ~CMaterialCachedParameters(void);
};


class CMaterial
{
private:
    
protected:
    
    std::map<std::string, CSharedShaderUniform> m_customShaderUniforms;
    CSharedMaterialCachedParameters m_parameters;
    static CSharedMaterialCachedParameters m_cachedParameters;
    static CSharedMaterialCachedParameters getCachedParameters(void);
    
public:
    
    CMaterial(void);
    ~CMaterial(void);
    
    static void initializeMaterial(CSharedMaterialRef material,
                                   CSharedConfigurationMaterialRef configuration,
                                   CSharedResourceAccessor resourceAccessor,
                                   ISharedRenderTechniqueAccessor renderTechniqueAccessor,
                                   ISharedResourceLoadingHandlerRef handler = nullptr);
    
    bool isCulling(void) const;
    GLenum getCullingMode(void) const;
    
    bool isBlending(void) const;
    GLenum getBlendingFunctionSource(void) const;
    GLenum getBlendingFunctionDestination(void) const;
    
    bool isDepthTest(void) const;
    bool isDepthMask(void) const;
    
    bool isClipping(void) const;
    glm::vec4 getClippingPlane(void) const;
    
    bool isReflecting(void) const;
    bool isShadowing(void) const;
    bool isDebugging(void) const;
    
    CSharedShader getShader(void) const;
    CSharedTexture getTexture(E_SHADER_SAMPLER sampler) const;
    E_SHADER_SAMPLER getSamplerIndex(CSharedTextureRef texture) const;
    
    void setCulling(bool value);
    void setCullingMode(GLenum value);
    
    void setBlending(bool value);
    void setBlendingFunctionSource(GLenum value);
    void setBlendingFunctionDestination(GLenum value);
    
    void setDepthTest(bool value);
    void setDepthMask(bool value);
    
    void setClipping(bool value);
    void setClippingPlane(const glm::vec4& value);
    
    void setReflecting(bool value);
    void setShadowing(bool value);
    void setDebugging(bool value);
    
    void setShader(CSharedShaderRef shader);
    void setTexture(CSharedTextureRef texture,
                    E_SHADER_SAMPLER sampler);
    
    void setCustomShaderUniform(const glm::mat4x4& matrix, const std::string& uniform);
    void setCustomShaderUniform(const glm::mat3x3& matrix, const std::string& uniform);
    void setCustomShaderUniform(const glm::vec4& vector, const std::string& uniform);
    void setCustomShaderUniform(const glm::vec3& vector, const std::string& uniform);
    void setCustomShaderUniform(const glm::vec2& vector, const std::string& uniform);
    void setCustomShaderUniform(f32 value, const std::string& uniform);
    void setCustomShaderUniform(i32 value, const std::string& uniform);
    
    const std::map<std::string, CSharedShaderUniform>& getCustomUniforms(void) const;
    
    bool isLoaded(void) const;
    bool isCommited(void) const;
    
    void bind(void);
    void unbind(void);
};

#endif 
