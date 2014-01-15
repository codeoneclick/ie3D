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
#include "IResource.h"

class CShader;
class CTexture;
class IScreenSpaceTextureAccessor;
class CResourceAccessor;
class IResourceLoadingHandler;
class I_RO_TemplateCommon;

class I_RO_Material
{
private:
    
protected:
    
public:
    
    I_RO_Material(void);
    ~I_RO_Material(void);
    
    virtual bool Get_IsCulling(void) const = 0;
    virtual GLenum Get_CullingMode(void) const = 0;
    
    virtual bool Get_IsBlending(void) const = 0;
    virtual GLenum Get_BlendingFunctionSource(void) const = 0;
    virtual GLenum Get_BlendingFunctionDestination(void) const = 0;
    
    virtual bool Get_IsDepthTest(void) const = 0;
    virtual bool Get_IsDepthMask(void) const = 0;
    
    virtual bool Get_IsClipping(void) const = 0;
    virtual glm::vec4 Get_ClippingPlane(void) const = 0;
    
    virtual bool Get_IsReflecting(void) const = 0;
    virtual bool Get_IsShadowing(void) const = 0;
    virtual bool Get_IsDebugging(void) const = 0;
    
    virtual std::shared_ptr<CShader> Get_Shader(void) const = 0;
    virtual std::shared_ptr<CTexture> Get_Texture(E_SHADER_SAMPLER _sampler) const = 0;
    virtual E_SHADER_SAMPLER Get_SamplerId(const std::shared_ptr<CTexture>& _texture) const = 0;
};

class I_WO_Material
{
private:
    
protected:
    
public:
    
    I_WO_Material(void);
    ~I_WO_Material(void);
    
    virtual void Set_IsCulling(bool _value) = 0;
    virtual void Set_CullingMode(GLenum _value) = 0;
    
    virtual void Set_IsBlending(bool _value) = 0;
    virtual void Set_BlendingFunctionSource(GLenum _value) = 0;
    virtual void Set_BlendingFunctionDestination(GLenum _value) = 0;
    
    virtual void Set_IsDepthTest(bool _value) = 0;
    virtual void Set_IsDepthMask(bool _value) = 0;
    
    virtual void Set_IsClipping(bool _value) = 0;
    virtual void Set_ClippingPlane(const glm::vec4& _value) = 0;
    
    virtual void Set_IsReflecting(bool _value) = 0;
    virtual void Set_IsShadowing(bool _value) = 0;
    virtual void Set_IsDebugging(bool _value) = 0;
    
    virtual void Set_Shader(const std::shared_ptr<CShader>& _shader) = 0;
    virtual void Set_Texture(const std::shared_ptr<CTexture>& _texture,
                             E_SHADER_SAMPLER _sampler) = 0;
    
    virtual void Serialize(const std::shared_ptr<I_RO_TemplateCommon>& _template,
                           const std::shared_ptr<CResourceAccessor>& _resourceAccessor,
                           const std::shared_ptr<IScreenSpaceTextureAccessor>& _screenSpaceTextureAccessor,
                           const std::shared_ptr<IResourceLoadingHandler>& _handler = nullptr) = 0;
};

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
    
    std::shared_ptr<CShader> m_shader;
    std::array<std::shared_ptr<CTexture>, E_SHADER_SAMPLER_MAX> m_textures;
    
public:
    
    CMaterialCachedParameters(void);
    ~CMaterialCachedParameters(void);
};


class CMaterial :
public IResource,
public I_RO_Material,
public I_WO_Material
{
private:
    
protected:
    
    std::shared_ptr<CMaterialCachedParameters> m_parameters;
    static std::shared_ptr<CMaterialCachedParameters> m_cachedParameters;
    static std::shared_ptr<CMaterialCachedParameters> Get_CachedParameters(void);
    
public:
    
    CMaterial(const std::string& _guid, bool _isCustom = false);
    ~CMaterial(void);
    
    bool Get_IsCulling(void) const;
    GLenum Get_CullingMode(void) const;
    
    bool Get_IsBlending(void) const;
    GLenum Get_BlendingFunctionSource(void) const;
    GLenum Get_BlendingFunctionDestination(void) const;
    
    bool Get_IsDepthTest(void) const;
    bool Get_IsDepthMask(void) const;
    
    bool Get_IsClipping(void) const;
    glm::vec4 Get_ClippingPlane(void) const;
    
    bool Get_IsReflecting(void) const;
    bool Get_IsShadowing(void) const;
    bool Get_IsDebugging(void) const;
    
    std::shared_ptr<CShader> Get_Shader(void) const;
    std::shared_ptr<CTexture> Get_Texture(E_SHADER_SAMPLER _sampler) const;
    E_SHADER_SAMPLER Get_SamplerId(const std::shared_ptr<CTexture>& _texture) const;
    
    void Set_IsCulling(bool _value);
    void Set_CullingMode(GLenum _value);
    
    void Set_IsBlending(bool _value);
    void Set_BlendingFunctionSource(GLenum _value);
    void Set_BlendingFunctionDestination(GLenum _value);
    
    void Set_IsDepthTest(bool _value);
    void Set_IsDepthMask(bool _value);
    
    void Set_IsClipping(bool _value);
    void Set_ClippingPlane(const glm::vec4& _value);
    
    void Set_IsReflecting(bool _value);
    void Set_IsShadowing(bool _value);
    void Set_IsDebugging(bool _value);
    
    void Set_Shader(const std::shared_ptr<CShader>& _shader);
    void Set_Texture(const std::shared_ptr<CTexture>& _texture,
                             E_SHADER_SAMPLER _sampler);
    
    void Serialize(const std::shared_ptr<I_RO_TemplateCommon>& _template,
                   const std::shared_ptr<CResourceAccessor>& _resourceAccessor,
                   const std::shared_ptr<IScreenSpaceTextureAccessor>& _screenSpaceTextureAccessor,
                   const std::shared_ptr<IResourceLoadingHandler>& _handler = nullptr);
    
    
    bool IsLoaded(void) const;
    bool IsCommited(void) const;
    
    void Bind(void);
    void Unbind(void);
};

#endif 
